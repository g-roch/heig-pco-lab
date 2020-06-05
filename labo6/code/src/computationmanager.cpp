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
{

    // TODO
}

int ComputationManager::requestComputation(Computation c) {
    monitorIn();

    if(not stopped and queueInput[(size_t) c.computationType].size() >= MAX_TOLERATED_QUEUE_SIZE) {
        ++inputNotFullNbThreadWaiting[(size_t) c.computationType];
        wait(inputNotFull[(size_t) c.computationType]);
        --inputNotFullNbThreadWaiting[(size_t) c.computationType];
    }
    if(stopped) {
        monitorOut();
        throwStopException();
    }
    int id = nextId;
    ++nextId;

    queueInput[(size_t) c.computationType].push(Request(c, id));

    signal(inputNotEmpty[(size_t) c.computationType]);
    monitorOut();
    return id;
}

void ComputationManager::abortComputation(int id) {
    monitorIn();
    abortedSet.insert(id);
    nextAbortResult.push(id);
    signal(outputNotEmpty);
    monitorOut();
}

Result ComputationManager::getNextResult() {
    monitorIn();

    while(not stopped and
          (queueOutput.empty()
            or queueOutput.top().getResult().getId() != nextIdOutput
          ))
    {
        if(not nextAbortResult.empty() and nextAbortResult.top() == nextIdOutput) {
            nextAbortResult.pop();
            ++nextIdOutput;
        }
        wait(outputNotEmpty);
        if(stopped) {
            break;
        }
    }
    if(stopped) {
        monitorOut();
        throwStopException();
    }

    ++nextIdOutput;
    Result ret = queueOutput.top().getResult();
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
