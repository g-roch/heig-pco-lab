//    ___  _________    ___  ___  ___  ___  //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  |/ _ \ //
//  / ___/ /__/ /_/ / / __// // / __// // / //
// /_/   \___/\____/ /____/\___/____/\___/  //
//                                          //
// Auteurs : Cassandre Wojciechowski et Gabriel Roch


// A vous de remplir les méthodes, vous pouvez ajouter des attributs ou méthodes pour vous aider
// déclarez les dans ComputationManager.h et définissez les méthodes ici.
// Certaines fonctions ci-dessous ont déjà un peu de code, il est à remplacer, il est là temporairement
// afin de faire attendre les threads appelants et aussi afin que le code compile.

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

    // On calcul l'identifiant de calcul
    // ( À faire avant de se mettre en attente, car si notre buffer est plein
    //   ce n'est pas forcement le cas pour d'autre ComputationType, et on pourrait se
    //   faire passer devant par un autre type ce qui casserait l'ordre de rendu au client
    // )
    int id = nextId;
    ++nextId;

    // Si l'utilisateur n'a pas appuyer sur le bouton stop
    // ET Si la file pour ce type de Computation n'est pas pleine
    // → On attends qu'il y ait de la place dans la file.
    if(not stopped and queueInput[(size_t) c.computationType].size() >= MAX_TOLERATED_QUEUE_SIZE) {
        ++inputNotFullNbThreadWaiting[(size_t) c.computationType];
        wait(inputNotFull[(size_t) c.computationType]);
        --inputNotFullNbThreadWaiting[(size_t) c.computationType];
    }

    // Si l'utilisateur a appuyer sur le bouton stop
    // → On termine le thread proprement
    if(stopped) {
        monitorOut();
        throwStopException();
    }

    // On insert la Computation dans la file 
    queueInput[(size_t) c.computationType].push(Request(c, id));

    // On signal qu'il y a au moins 1 élément dans la file.
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

    // On insert l'id annulé dans les deux structures qui les gères
    abortedSet.insert(id);
    nextAbortResult.push(id);

    // On réveille le thread de lecture pour permettre de passé au calcul suivant
    // si le calcul qu'il voulait récupérer viens d'être annulé.
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

    // Tant que l'utilisateur n'a pas appuyer sur le bouton stop
    // et que soit la file de sortie est vide
    //        soit que l'élément sur le dessus de la pile n'est pas celui que l'on attends
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

    // Si l'utilisateur a appuyer sur le bouton stop
    // → On termine le thread proprement
    if(stopped) {
        monitorOut();
        throwStopException();
    }

    // On récupére l'élément au sommet de la pile, qui est celui que l'on attendait
    ++nextIdOutput;
    Result ret = queueOutput.top();
    queueOutput.pop();

    monitorOut();
    return ret;
}

Request ComputationManager::getWork(ComputationType computationType) {
    monitorIn();
    if(not stopped and queueInput[(size_t) computationType].empty()) {
        ++inputNotEmptyNbThreadWaiting[(size_t) computationType];
        wait(inputNotEmpty[(size_t) computationType]);
        --inputNotEmptyNbThreadWaiting[(size_t) computationType];
    }
    if(stopped) {
        monitorOut();
        throwStopException();
    }
    Request front = queueInput[(size_t) computationType].front();
    queueInput[(size_t) computationType].pop();

    signal(inputNotFull[(size_t) computationType]);
    monitorOut();
    return front;
}

bool ComputationManager::continueWork(int id) {
    monitorIn();
    auto localFind = abortedSet.find(id);
    if(localFind != abortedSet.end()) {
        abortedSet.erase(localFind);
        monitorOut();
        return false;
    }
    monitorOut();
    return true and not stopped;
}

void ComputationManager::provideResult(Result result) {
    monitorIn();
    std::cout << result.getId() << std::endl;
    queueOutput.push(result);
    signal(outputNotEmpty);
    monitorOut();
}

void ComputationManager::stop() {
    monitorIn();
    stopped = true;
    signal(outputNotEmpty);
    for (size_t i = 0; i < COMPUTATION_TYPE_SIZE; ++i) {
        for(int j = 0; j <= inputNotEmptyNbThreadWaiting[i]; ++j)
            signal(inputNotEmpty[i]);
        for(int j = 0; j <= inputNotFullNbThreadWaiting[i]; ++j)
            signal(inputNotFull[i]);
    }
    monitorOut();
    // TODO
}
