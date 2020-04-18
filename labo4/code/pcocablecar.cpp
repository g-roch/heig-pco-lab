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

// Nous avons créé des sémaphores permettant de gérer les attentes des skieurs et de la télécabine
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

PcoCableCar::~PcoCableCar()
{

}

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

void PcoCableCar::waitInsideCableCar(int id)
{
    qDebug() << "Skieur " << id << " attend dans la télécabine d'arriver en haut";

    // Le skieur attend de sortir de la télécabine
    semaphoreSkierGoOut.acquire();
}

void PcoCableCar::goIn(int id)
{
    qDebug() << "Skieur " << id << " entre dans la télécabine ";

    // Le skieur est monté dans la télécabine, on incrémente le nombre de skieurs à l'intérieur
    mutexNbSkierInside.acquire();
    ++nbSkiersInside;
    mutexNbSkierInside.release();

    // Le skieur entre dans la télécabine
    semaphoreSkierWaitGoUp.release();
}

void PcoCableCar::goOut(int id)
{
    qDebug() << "Skieur " << id << " sort de la télécabine ";

    // Le skieur sort de la télécabine, on décrémente le nb de skieurs dans la télécabine
    mutexNbSkierInside.acquire();
    --nbSkiersInside;
    mutexNbSkierInside.release();

    // Le skieur sort de la télécabine et descend la montagne
    semaphoreSkierWaitGoDown.release();
}

// Indique si la télécabine est en fonction
bool PcoCableCar::isInService()
{
    return inService;
}

// Indique que le service de la télécabine est en train de changer d'état
void PcoCableCar::endService()
{
    qDebug() << "Le service de la télécabine se termine";
    serviceIsEnding = true;
}

// Le service de la télécabine est terminé
void PcoCableCar::realEndService()
{
    qDebug() << "Fin de service de la télécabine ";

    // On change l'état du service de la télécabine
    mutexNbSkierWaiting.acquire();
    inService = false;
    int max = nbSkiersWaiting;
    mutexNbSkierWaiting.release();

    // Le skieur ne doit plus attendre la télécabine
    for(int i = 0; i < max; ++i) {
        semaphoreSkierGoIn.release();
    }
}

void PcoCableCar::goUp()
{
    qDebug() << "La télécabine monte";
    PcoThread::usleep((MIN_SECONDS_DELAY + QRandomGenerator::system()->bounded(MAX_SECONDS_DELAY + 1)) * SECOND_IN_MICROSECONDS);
}

void PcoCableCar::goDown()
{
    qDebug() << "La télécabine descend";
    PcoThread::usleep((MIN_SECONDS_DELAY + QRandomGenerator::system()->bounded(MAX_SECONDS_DELAY + 1)) * SECOND_IN_MICROSECONDS);

    // Si la télécabine descend et que le service se termine, les skieurs doivent arrêter d'attendre pour y monter
    if (serviceIsEnding)
        realEndService();
}

void PcoCableCar::loadSkiers()
{
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

void PcoCableCar::unloadSkiers()
{
    qDebug() << "Déchargement des skieurs";

    // Le skieur sort de la télécabine
    for (unsigned i = 0; i < capacity and isInService(); ++i) {
        semaphoreSkierGoOut.release();
    }

    // Le skieur attend de s'élancer pour descendre la montagne
    for (unsigned i = 0; i < capacity and isInService(); ++i) {
        semaphoreSkierWaitGoDown.acquire();
    }

}
