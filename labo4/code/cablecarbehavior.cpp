//    ___  _________    ___  ___  ___  ___  //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  |/ _ \ //
//  / ___/ /__/ /_/ / / __// // / __// // / //
// /_/   \___/\____/ /____/\___/____/\___/  //
//                                          //
// Auteurs : Gabriel Roch, Cassandre Wojciechowski

#include "cablecarbehavior.h"

/**
 * Fonction décidant du comportement de la télécabine
 * Quand la télécabine est en service, les skieurs montent dedans, la télécabine monte,
 * les skieurs sortent, la télécabine redescend.
 */
void CableCarBehavior::run()
{
    while (cableCar->isInService()) {
        cableCar->loadSkiers();
        cableCar->goUp();
        cableCar->unloadSkiers();
        cableCar->goDown();
    }
}
