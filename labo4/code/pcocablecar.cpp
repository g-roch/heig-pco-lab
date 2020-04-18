//    ___  _________    ___  ___  ___  ___  //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  |/ _ \ //
//  / ___/ /__/ /_/ / / __// // / __// // / //
// /_/   \___/\____/ /____/\___/____/\___/  //
//                                          //
// Auteurs : Gabriel Roch, Cassandre Wojciechowski

#include "pcocablecar.h"
#include <pcosynchro/pcothread.h>

#include <QDebug>
#include <QRandomGenerator>

constexpr unsigned int MIN_SECONDS_DELAY = 1;
constexpr unsigned int MAX_SECONDS_DELAY = 5;
constexpr unsigned int SECOND_IN_MICROSECONDS = 1000000;

/**
 * Constructeur de la télécabine, construction des sémaphores permettant de gérer les attentes des skieurs et de la télécabine
 */
PcoCableCar::PcoCableCar(const unsigned int capacity) :
    capacity(capacity),
    semaphoreSkierGoIn(0),
    mutexNbSkierWaiting(1),
    mutexNbSkierInside(1),
    semaphoreSkierGoOut(0),
    semaphoreSkierWaitGoUp(0),
    semaphoreSkierWaitGoDown(0)
{

}

/**
 * Destructeur
 */
PcoCableCar::~PcoCableCar()
{

}

/**
 * Fonction gérant l'attente du skieur sur la télécabine
 * @param id est le numéro d'identification du skieur
 * @return la fonction se termine soit quand la télécabine est présente, soit à la fin du service
 */
void PcoCableCar::waitForCableCar(int id)
{
    qDebug() << "Skieur " << id << " attend la télécabine";

    // Le skieur attend, on incrémente le nb de skieurs en attente
    mutexNbSkierWaiting.acquire();
    ++nbSkiersWaiting;
    mutexNbSkierWaiting.release();

    // Le skieur attend de monter dans la télécabine
    if(isInService())
        semaphoreSkierGoIn.acquire();

    // Le skieur va monter dans la télécabine, il n'attend donc plus dehors, on décrémente le nb de skieurs en attente
    mutexNbSkierWaiting.acquire();
    --nbSkiersWaiting;
    mutexNbSkierWaiting.release();
}

/**
 * Fonction gérant l'attente du skieur dans la télécabine durant la montée
 * @param id est le numéro d'identification du skieur
 * @return la fonction se termine quand la télécabine arrive au sommet
 */
void PcoCableCar::waitInsideCableCar(int id)
{
    qDebug() << "Skieur " << id << " attend dans la télécabine d'arriver en haut";

    // Le skieur attend de pouvoir sortir de la télécabine
    semaphoreSkierGoOut.acquire();
}

/**
 * Fonction gérant la montée du skieur dans la télécabine
 * @param id est le numéro d'identification du skieur
 */
void PcoCableCar::goIn(int id)
{
    qDebug() << "Skieur " << id << " entre dans la télécabine ";

    // Le skieur est monté dans la télécabine, on incrémente le nombre de skieurs à l'intérieur
    mutexNbSkierInside.acquire();
    ++nbSkiersInside;
    mutexNbSkierInside.release();

    // Le skieur est monté dans la télécabine, pour ce skieur, elle peut donc partir
    semaphoreSkierWaitGoUp.release();
}

/**
 * Fonction gérant la sortie du skieur de la télécabine
 * @param id est le numéro d'identification du skieur
 */
void PcoCableCar::goOut(int id)
{
    qDebug() << "Skieur " << id << " sort de la télécabine ";

    // Le skieur sort de la télécabine, on décrémente le nb de skieurs dans la télécabine
    mutexNbSkierInside.acquire();
    --nbSkiersInside;
    mutexNbSkierInside.release();

    // Le skieur est sorti de la télécabine, pour ce skieur, elle peut donc partir
    semaphoreSkierWaitGoDown.release();
}

/**
 * Indique si la télécabine est en service
 * @return true si la cabine est en service
 */
bool PcoCableCar::isInService()
{
    return inService;
}

/**
 * Demande la fin de service de la télécabine
 */
void PcoCableCar::endService()
{
    qDebug() << "Le service de la télécabine se termine";
    serviceIsEnding = true;
}

/**
 * Effectue la fin réelle du service après la dernière descente
 */
void PcoCableCar::realEndService()
{
    qDebug() << "Fin de service de la télécabine ";

    // On change l'état du service de la télécabine pour que les skieurs en attente qui seront libérés
    // sachent qu'ils doivent partir et pas monter dans la cabine
    mutexNbSkierWaiting.acquire();
    inService = false;
    int max = nbSkiersWaiting;
    mutexNbSkierWaiting.release();

    // Le skieur ne doit plus attendre la télécabine
    for(int i = 0; i < max; ++i) {
        semaphoreSkierGoIn.release();
    }
}

/**
 * Fait monter la cabine
 */
void PcoCableCar::goUp()
{
    qDebug() << "La télécabine monte";
    PcoThread::usleep((MIN_SECONDS_DELAY + QRandomGenerator::system()->bounded(MAX_SECONDS_DELAY + 1)) * SECOND_IN_MICROSECONDS);
}

/**
 * Fait descendre la cabine (si la fin de service a été demandée, termine effectivement le service)
 */
void PcoCableCar::goDown()
{
    qDebug() << "La télécabine descend";
    PcoThread::usleep((MIN_SECONDS_DELAY + QRandomGenerator::system()->bounded(MAX_SECONDS_DELAY + 1)) * SECOND_IN_MICROSECONDS);

    // Si la télécabine descend et que le service se termine, les skieurs doivent arrêter d'attendre pour y monter
    if (serviceIsEnding)
        realEndService();
}

/**
 * Autorise le chargement des skieurs dans la télécabine
 * @return la fonction se termine quand tous les skieurs autorisés sont dans la cabine
 */
void PcoCableCar::loadSkiers()
{
    // TODO gérer la fin de service PENDANT le chargement

    qDebug() << "Chargement de la cabine ";

    // Le skieur est en train de monter dans la télécabine
    for (unsigned i = 0; i < capacity and isInService(); ++i) {
        semaphoreSkierGoIn.release();
    }

    // Le skieur est monté dans la télécabine
    for (unsigned i = 0; i < capacity and isInService(); ++i) {
        semaphoreSkierWaitGoUp.acquire();
    }

}

/**
 * Autorise le déchargement des skieurs de la télécabine
 * @return la fonction se termine quand tous les skieurs sont sortis de la cabine
 */
void PcoCableCar::unloadSkiers()
{
    qDebug() << "Déchargement des skieurs";

    // Le skieur sort de la télécabine
    for (unsigned i = 0; i < capacity and isInService(); ++i) {
        semaphoreSkierGoOut.release();
    }

    // Le skieur est effectivement sorti de la télécabine
    for (unsigned i = 0; i < capacity and isInService(); ++i) {
        semaphoreSkierWaitGoDown.acquire();
    }

}
