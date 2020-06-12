//    ___  _________    ___  ___  ___  ___  //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  |/ _ \ //
//  / ___/ /__/ /_/ / / __// // / __// // / //
// /_/   \___/\____/ /____/\___/____/\___/  //
//                                          //
// Auteurs : Cassandre Wojciechowski et Gabriel Roch

// Ci-dessous se trouvent aussi les déclarations de divers classes et interfaces
// utilisées avec le buffer ComputationManager.

#ifndef COMPUTATIONMANAGER_H
#define COMPUTATIONMANAGER_H

#include "pcosynchro/pcohoaremonitor.h"

#include <memory>
#include <set>
#include <iostream>

/**
 * @brief The ComputationType enum represents the abstract computation types that are available
 */
enum class ComputationType {A, B, C};

/**
 * @brief Number elems of ComputationType
 */
#define COMPUTATION_TYPE_SIZE 3

/**
 * @brief The Computation class Represents a computation with a given type and data.
 */
class Computation
{
public:
    /**
     * @brief Computation Constructs a computation of a given type
     * @param computationType
     */
    Computation(ComputationType computationType): computationType(computationType) {data = std::make_shared<std::vector<double>>();}

    /**
     * @brief computationType The given type
     */
    ComputationType computationType;
    /**
     * @brief data The data for the computation
     */
    std::shared_ptr<std::vector<double>> data;
};

/**
 * @brief The Request class is a request for a computation with and id and data
 */
class Request
{
public:
    Request(): data(nullptr), id(0) {}
    Request(std::shared_ptr<std::vector<double>> data, int id): data(data), id(id) {}
    Request(Computation c, int id): data(c.data), id(id) {}

    int getId() const {return id;}

    /**
     * @brief data The data for the computation
     */
    std::shared_ptr<const std::vector<double>> data;

private:
    int id;
};

/**
 * @brief The Result class holds a result and an id
 */
class Result
{
public:
    Result(int id, double result): id(id), result(result) {}

    int getId() const {return id;}
    double getResult() const {return result;}

private:
    int id;
    double result;
};

/**
 * @brief The ClientInterface class contains the methods of the buffer that are exposed to the client
 */
class ClientInterface
{
public:
    /**
     * @brief requestComputation Request a computation c
     * @param c The computation to be done
     * @return The assigned id (should follow the order of the requests)
     */
    virtual int requestComputation(Computation c) = 0;

    /**
     * @brief abortComputation Allows the client to abort a computation
     * This should remove the Request/Result corresponding to the id in
     * the ComputationManager (buffer) if there is one and stop a compute
     * engine working on it if there was one.
     * @param id the id of the computation to be aborted
     */
    virtual void abortComputation(int id) = 0;

    /**
     * @brief getNextResult Method that provides the next result.
     * The order of the results must follow the order of the requests.
     * (order of ids of the results should follow the order of the request ids
     * which should follow the order of the requests).
     * @return The next Result of the requested computations
     */
    virtual Result getNextResult() = 0;
};

/**
 * @brief The ComputeEngineInterface class contains the methods of the buffer that are exposed to the compute engines
 */
class ComputeEngineInterface
{
public:
    /**
     * @brief getWork is used to ask for work of a given type which will come as a Request
     * @param computationType the type of work that is wanted
     * @return a request to be fulfilled
     */
    virtual Request getWork(ComputationType computationType) = 0;

    /**
     * @brief continueWork Allows a compute engine to ask if it must continue working on a request
     * @param id the id of the request the compute engine is currently working on
     * @return true if the worker should continue working on the request with id id
     */
    virtual bool continueWork(int id) = 0;

    /**
     * @brief provideResult Allows a compute engine to prove a result to the buffer
     * @param result the result that has been computed
     */
    virtual void provideResult(Result result) = 0;
};

/**
 * @brief The ComputationManager class is the implementation of the shared buffer between client and compute engines.
 * It is to be implemented as a Hoare monitor.
 */
class ComputationManager : public ClientInterface, public ComputeEngineInterface, protected PcoHoareMonitor
{
public:
    /**
     * @brief The StopException class is an exception that is thrown when a thread tries to wait
     * when the buffer is stopped or is awoken (signaled) after the buffer is stopped.
     */
    class StopException : public std::exception {};

    /**
     * @brief ComputationManager Allows to create a buffer with a maximum queue size
     * @param maxQueueSize the maximum queue size allowed to store pending requests
     */
    ComputationManager(int maxQueueSize = 10);

    // Client Interface
    // Documentation above
    int requestComputation(Computation c) override;
    void abortComputation(int id) override;
    Result getNextResult() override;

    // Compute Engine Interface
    // Documentation above
    Request getWork(ComputationType computationType) override;
    bool continueWork(int id) override;
    void provideResult(Result result) override;

    // Control Interface
    /**
     * @brief stop Is used when the buffer is stopped, will release and interrupt waiting threads
     */
    void stop();

protected:

    /**
     * @brief taille maximale autorisée pour la file d'attente d'entrée
     */
    const size_t MAX_TOLERATED_QUEUE_SIZE;

    /**
     * @brief Indicateur de l'appui sur le bouton stop de la part de l'utilisateur
     */
    bool stopped = false;

private:

    /**
     * @brief Classe pour trier par ordre croissant les résultats
     */
    class cmpGreaterResultId {
      public:
      bool operator()(Result const & lhs, Result const & rhs) const{
              return lhs.getId() > rhs.getId();
      }
    };

    /**
     * @brief throwStopException Throws a StopException (will be handled by the caller)
     */
    inline void throwStopException() {
        std::cout << "throwStopException()" << std::endl;
        throw StopException();
    }

    /**
     * @brief ID qu'aura le prochain calcul à entrer dans la file d'attente
     */
    int nextId = 0;
    /**
     * @brief ID du prochain résultat à retourner
     */
    int nextIdOutput = 0;

    /**
     * @brief Condition d'attente pour avoir au moins 1 élément dans la file d'entrée 
     *        (un par ComputationType)
     */
    std::vector<Condition> inputNotEmpty;
    /**
     * @brief Condition d'attente pour avoir de la place dans la file d'entrée 
     *        (une par ComputationType)
     */
    std::vector<Condition> inputNotFull;

    /**
     * @brief File d'attente d'entrée pour les calculs (une `queue` par ComputationType)
     */
    std::vector<std::queue<Request>> queueInput;
    /**
     * @brief Nombre de threads en attente de contenu dans la file d'entrée (un compteur
     *        par ComputationType)
     */
    std::vector<int> inputNotEmptyNbThreadWaiting;
    /**
     * @brief Nombre de threads en attente de place dans la file d'entrée (un compteur
     *        par ComputationType)
     */
    std::vector<int> inputNotFullNbThreadWaiting;

    /**
     * @brief Condition d'attente pour avoir au moins 1 élément dans la file de sortie
     */
    Condition outputNotEmpty;
    /**
     * @brief File d'attente de sortie pour les calculs.
     *        Le calcul avec le plus petit identifiant se trouve toujours au sommet.
     */
    std::priority_queue<
        Result,
        std::vector<Result>,
        cmpGreaterResultId
    > queueOutput;

    /**
     * @brief Set d'identifiant des calculs annulés qui n'ont pas encore été signalés
     *        au thread de calcul
     */
    std::set<int> abortedSet;
    /**
     * @brief File d'identifiants de calculs annulés qui n'ont pas encore été supprimés
     *        de la file de sortie.
     *        L'id le plus petit se trouve toujours sur le dessus.
     */
    std::priority_queue<
        int,
        std::vector<int>,
        std::greater<int>
    > nextAbortResult;

};

#endif // COMPUTATIONMANAGER_H
