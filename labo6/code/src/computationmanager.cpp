//    ___  _________    ___  ___  ___  ___  //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  |/ _ \ //
//  / ___/ /__/ /_/ / / __// // / __// // / //
// /_/   \___/\____/ /____/\___/____/\___/  //
//                                          //
// Auteurs : Cassandre Wojciechowski et Gabriel Roch

#include "computationmanager.h"

#define COMPUTATION_TYPE_SIZE 3

ComputationManager::ComputationManager(int maxQueueSize)
    : MAX_TOLERATED_QUEUE_SIZE(maxQueueSize),
      stopped(false),
      inputNotEmpty(COMPUTATION_TYPE_SIZE),
      inputNotFull(COMPUTATION_TYPE_SIZE),
      queueInput(COMPUTATION_TYPE_SIZE),
      inputNotEmptyNbThreadWaiting(COMPUTATION_TYPE_SIZE),
      inputNotFullNbThreadWaiting(COMPUTATION_TYPE_SIZE),
      outputNotEmpty(),
      queueOutput(),
      abortedSet()
{ }

/**
 * @brief requestComputation Request a computation c
 * @param c The computation to be done
 * @return The assigned id (should follow the order of the requests)
 */
int ComputationManager::requestComputation(Computation c) {
    monitorIn();

    // On calcule l'identifiant de calcul
    // ( À faire avant de se mettre en attente, car si notre buffer est plein
    //   ce n'est pas forcément le cas pour d'autres ComputationType et on pourrait se
    //   faire passer devant par un autre type ce qui casserait l'ordre de rendu au client
    // )
    int id = nextId;
    ++nextId;

    // Si l'utilisateur n'a pas appuyé sur le bouton stop
    // ET Si la file pour ce type de Computation n'est pas pleine
    // → On attend qu'il y ait de la place dans la file.
    if(not stopped and queueInput[(size_t) c.computationType].size() >= MAX_TOLERATED_QUEUE_SIZE) {
        ++inputNotFullNbThreadWaiting[(size_t) c.computationType];
        wait(inputNotFull[(size_t) c.computationType]);
        --inputNotFullNbThreadWaiting[(size_t) c.computationType];
    }

    // Si l'utilisateur a appuyé sur le bouton stop
    // → On termine le thread proprement
    if(stopped) {
        monitorOut();
        throwStopException();
    }

    // On insert la Computation dans la file 
    queueInput[(size_t) c.computationType].push(Request(c, id));

    // On signale qu'il y a au moins 1 élément dans la file.
    signal(inputNotEmpty[(size_t) c.computationType]);

    monitorOut();
    return id;
}

/**
 * @brief abortComputation Allows the client to abort a computation
 * This should remove the Request/Result corresponding to the id in
 * the ComputationManager (buffer) if there is one and stop a compute
 * engine working on it if there was one.
 * @param id the id of the computation to be aborted
 */
void ComputationManager::abortComputation(int id) {
    monitorIn();

    // On insert l'id annulé dans les deux structures qui les gèrent
    abortedSet.insert(id);
    nextAbortResult.push(id);

    // On réveille le thread de lecture pour permettre de passer au calcul suivant
    // si le calcul qu'il voulait récupérer vient d'être annulé.
    signal(outputNotEmpty);

    monitorOut();
}

/**
 * @brief getNextResult Method that provides the next result.
 * The order of the results must follow the order of the requests.
 * (order of ids of the results should follow the order of the request ids
 * which should follow the order of the requests).
 * @return The next Result of the requested computations
 */
Result ComputationManager::getNextResult() {
    monitorIn();

    // Tant que l'utilisateur n'a pas appuyé sur le bouton stop
    // et que soit la file de sortie est vide
    //        soit l'élément sur le dessus de la pile n'est pas celui que l'on attend
    // → Supprimer l'élément si c'est un calcul annulé
    // → Sinon Attendre qu'un nouvel élément arrive dans la file
    while(not stopped and
          (queueOutput.empty()
            or queueOutput.top().getId() != nextIdOutput
          ))
    {
        if(not nextAbortResult.empty() and nextAbortResult.top() == nextIdOutput) {
            nextAbortResult.pop();
            ++nextIdOutput;
        } else {
          wait(outputNotEmpty);
        }
    }

    // Si l'utilisateur a appuyé sur le bouton stop
    // → On termine le thread proprement
    if(stopped) {
        monitorOut();
        throwStopException();
    }

    // On récupére l'élément au sommet de la pile, qui est celui que l'on attendait
    // Et on le supprime de la pile
    ++nextIdOutput;
    Result ret = queueOutput.top();
    queueOutput.pop();

    monitorOut();
    return ret;
}

/**
 * @brief getWork Method that provides the computation to be done
 * @param computationType type of the computation to be done
 * @return The Request of the computation to be done
 */
Request ComputationManager::getWork(ComputationType computationType) {
    monitorIn();

    // Si l'utilisateur n'a pas appuyé sur le bouton stop
    // ET Si la file pour ce type de Computation est vide
    // → On attend qu'il y ait des calculs à récupérer
    if(not stopped and queueInput[(size_t) computationType].empty()) {
        ++inputNotEmptyNbThreadWaiting[(size_t) computationType];
        wait(inputNotEmpty[(size_t) computationType]);
        --inputNotEmptyNbThreadWaiting[(size_t) computationType];
    }

    // Si l'utilisateur a appuyé sur le bouton stop
    // → On termine le thread proprement
    if(stopped) {
        monitorOut();
        throwStopException();
    }

    // On récupère le calcul au sommet de la pile et le supprime de la pile
    Request front = queueInput[(size_t) computationType].front();
    queueInput[(size_t) computationType].pop();

    // On signale qu'il y a au moins de la place pour 1 calcul de ce type
    signal(inputNotFull[(size_t) computationType]);
    monitorOut();
    return front;
}

/**
 * @brief continueWork Method that indicates if the work should continue
 * based on if the computation was aborted or not
 * @param id the id of the computation to be continued or not
 * @return true if computation not aborted, false otherwise
 */
bool ComputationManager::continueWork(int id) {
    monitorIn();

    // Si le calcul avec l'identifiant id est dans le set des calculs annulés
    // → On le supprime de la liste et on retourne faux
    auto localFind = abortedSet.find(id);
    if(localFind != abortedSet.end()) {
        abortedSet.erase(localFind);

        monitorOut();
        return false;
    }

    monitorOut();

    // On retourn true si l'utilisateur n'a pas appuyer sur le bouton stop
    return true and not stopped;
}

/**
 * @brief provideResult Method that prints result id and pushes result in output queue
 * @param result the result to be pushed in the queue
 */
void ComputationManager::provideResult(Result result) {
    monitorIn();

    // On ajout le resultat dans la file d'outputs
    queueOutput.push(result);

    // On signale qu'il y a un nouvel élément dans la file d'ouptuts
    signal(outputNotEmpty);

    monitorOut();
}

/**
 * @brief stop Method that wakes up every waiting thread and sets stopped
 * variable with true value (to be tested in every other method above)
 */
void ComputationManager::stop() {
    monitorIn();

    // On arrête
    stopped = true;

    // On réveille tous les threads qui ont peuvent être dans un wait()
    signal(outputNotEmpty);
    for (size_t i = 0; i < COMPUTATION_TYPE_SIZE; ++i) {

        for(int j = 0; j <= inputNotEmptyNbThreadWaiting[i]; ++j)
            signal(inputNotEmpty[i]);

        for(int j = 0; j <= inputNotFullNbThreadWaiting[i]; ++j)
            signal(inputNotFull[i]);
    }

    monitorOut();
}
