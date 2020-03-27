//    ___  _________    ___  ___  ___  ___  //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  |/ _ \ //
//  / ___/ /__/ /_/ / / __// // / __// // / //
// /_/   \___/\____/ /____/\___/____/\___/  //
//                                          //
// Auteurs : Prénom Nom, Prénom Nom

#include "pcocablecar.h"
#include <pcosynchro/pcothread.h>

#include <QDebug>
#include <QRandomGenerator>
constexpr unsigned int MIN_SECONDS_DELAY = 1;
constexpr unsigned int MAX_SECONDS_DELAY = 5;
constexpr unsigned int SECOND_IN_MICROSECONDS = 1000000;

// A vous de remplir les méthodes ci-dessous

PcoCableCar::PcoCableCar(const unsigned int capacity) :
    capacity(capacity),
    semaphoreSkierGoIn(0),
    mutexNbSkierWaiting(1),
    mutexNbSkierInside(1),
    semaphoreSkierGoOut(0)
{

}

PcoCableCar::~PcoCableCar()
{

}

void PcoCableCar::waitForCableCar(int id)
{
    // en liste d'attente
    mutexNbSkierWaiting.acquire();
    ++nbSkiersWaiting;
    mutexNbSkierWaiting.release();

    // Problème si endService pendant le acquire()
    // attends une place dans la télécabine
    semaphoreSkierGoIn.acquire();
}

void PcoCableCar::waitInsideCableCar(int id)
{
    semaphoreSkierGoOut.acquire();
}

void PcoCableCar::goIn(int id)
{

    // monté dans la télécabine
    mutexNbSkierWaiting.acquire();
    --nbSkiersWaiting;
    mutexNbSkierWaiting.release();

    mutexNbSkierInside.acquire();
    ++nbSkiersInside;
    mutexNbSkierInside.release();

}

void PcoCableCar::goOut(int id)
{
    // Sort de la télécabine
    mutexNbSkierInside.acquire();
    --nbSkiersInside;
    mutexNbSkierInside.release();
}

bool PcoCableCar::isInService()
{
    return inService;
}

void PcoCableCar::endService()
{
    serviceIsEnding = true;

}

void PcoCableCar::goUp()
{
    qDebug() << "Le télécabine monte";
    PcoThread::usleep((MIN_SECONDS_DELAY + QRandomGenerator::system()->bounded(MAX_SECONDS_DELAY + 1)) * SECOND_IN_MICROSECONDS);
}

void PcoCableCar::goDown()
{
    qDebug() << "Le télécabine descend";
    PcoThread::usleep((MIN_SECONDS_DELAY + QRandomGenerator::system()->bounded(MAX_SECONDS_DELAY + 1)) * SECOND_IN_MICROSECONDS);
    if (serviceIsEnding)
        inService = false;
}

void PcoCableCar::loadSkiers()
{
    for (unsigned i = 0; i < capacity and isInService(); ++i) {
        semaphoreSkierGoIn.release();
    }

    bool end = false;
    // attente active
    while (!end) {
        // Attention fin de service alors que tous ne sont pas dedans
        mutexNbSkierInside.acquire();
        end = nbSkiersInside == capacity;
        mutexNbSkierInside.release();
    }

}

void PcoCableCar::unloadSkiers()
{
    for (unsigned i = 0; i < capacity and isInService(); ++i) {
        semaphoreSkierGoOut.release();
    }

    bool end = false;
    // attente active
    while (!end) {
        mutexNbSkierInside.acquire();
        end = nbSkiersInside == 0;
        mutexNbSkierInside.release();
    }

}
