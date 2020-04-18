//    ___  _________    ___  ___  ___  ___  //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  |/ _ \ //
//  / ___/ /__/ /_/ / / __// // / __// // / //
// /_/   \___/\____/ /____/\___/____/\___/  //
//                                          //
// Auteurs : Gabriel Roch, Cassandre Wojciechowski

#include "skierbehavior.h"

#include <QRandomGenerator>

constexpr unsigned int MIN_SECONDS_DELAY = 2;
constexpr unsigned int MAX_SECONDS_DELAY = 10;
constexpr unsigned int SECOND_IN_MICROSECONDS = 1000000;

int SkierBehavior::nextId = 1;

/**
 * Comportement du skieur, quand la télécabine est en service, le skieur l'attend,
 * monte dedans, attend d'être en haut et sort. Puis il descend la montagne à skis.
 */
void SkierBehavior::run()
{
    while (cableCar->isInService()) {
        cableCar->waitForCableCar(id);
        // Si la fin de service arrive pendant que le skieur attend de monter dans la télécabine, il rentre chez lui
        if(cableCar->isInService()) {
            cableCar->goIn(id);
            cableCar->waitInsideCableCar(id);
            cableCar->goOut(id);

            goDownTheMountain();
        }
    }
}

/**
 * Le skieur descend de la montagne
 */
void SkierBehavior::goDownTheMountain()
{
    qDebug() << "Skieur" << id << "est en train de skier et descend de la montagne";
    PcoThread::usleep((MIN_SECONDS_DELAY + QRandomGenerator::system()->bounded(MAX_SECONDS_DELAY + 1)) * SECOND_IN_MICROSECONDS);
    qDebug() << "Skieur" << id << "est arrivé en bas de la montagne";
}
