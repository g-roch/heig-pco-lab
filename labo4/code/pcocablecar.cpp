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

    qDebug() << "Sckieur " << id << " attends la télécabines";
    // en liste d'attente
    mutexNbSkierWaiting.acquire();
    ++nbSkiersWaiting;
    mutexNbSkierWaiting.release();

    // attends une place dans la télécabine
    if(isInService())
        semaphoreSkierGoIn.acquire();

    mutexNbSkierWaiting.acquire();
    --nbSkiersWaiting;
    mutexNbSkierWaiting.release();
}

void PcoCableCar::waitInsideCableCar(int id)
{

    qDebug() << "Sckieur " << id << " attends la d'arriver en haut";
    semaphoreSkierGoOut.acquire();
}

void PcoCableCar::goIn(int id)
{
    qDebug() << "Sckieur " << id << " Entre dans la cabine ";

    // monté dans la télécabine
    mutexNbSkierInside.acquire();
    ++nbSkiersInside;
    mutexNbSkierInside.release();

    semaphoreSkierWaitGoUp.release();

}

void PcoCableCar::goOut(int id)
{
    qDebug() << "Sckieur " << id << " sort de la cabine ";
    // Sort de la télécabine
    mutexNbSkierInside.acquire();
    --nbSkiersInside;
    mutexNbSkierInside.release();

    semaphoreSkierWaitGoDown.release();
}

bool PcoCableCar::isInService()
{
    return inService;
}

void PcoCableCar::endService()
{

    qDebug() << "Fin de service :) ";
    serviceIsEnding = true;
}

void PcoCableCar::realEndService()
{
    qDebug() << "Fin de service réel :) :) ";

    mutexNbSkierWaiting.acquire();
    inService = false;
    int max = nbSkiersWaiting;
    mutexNbSkierWaiting.release();

    for(int i = 0; i < max; ++i) {
        semaphoreSkierGoIn.release();
    }
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
        realEndService();
}

void PcoCableCar::loadSkiers()
{

    qDebug() << "Chargement de la cabine ";
    for (unsigned i = 0; i < capacity and isInService(); ++i) {
        semaphoreSkierGoIn.release();
    }

    for (unsigned i = 0; i < capacity and isInService(); ++i) {
        semaphoreSkierWaitGoUp.acquire();
    }

}

void PcoCableCar::unloadSkiers()
{
    qDebug() << "Largage des sckieurs";
    for (unsigned i = 0; i < capacity and isInService(); ++i) {
        semaphoreSkierGoOut.release();
    }

    for (unsigned i = 0; i < capacity and isInService(); ++i) {
        semaphoreSkierWaitGoDown.acquire();
    }

}
