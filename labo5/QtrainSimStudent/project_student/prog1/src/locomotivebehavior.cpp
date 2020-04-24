//    ___  _________    ___  ___  ___  ___  //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  |/ _ \ //
//  / ___/ /__/ /_/ / / __// // / __// // / //
// /_/   \___/\____/ /____/\___/____/\___/  //
//                                          //
// Auteurs : Nom Prénom, Nom Prénom
//
#include "locomotivebehavior.h"
#include "ctrain_handler.h"

/*!
 * \brief locomotiveBehavior Constructeur de la classe
 * \param loco la locomotive dont on représente le comportement
 */
LocomotiveBehavior::LocomotiveBehavior(Locomotive& loco, std::shared_ptr<AllSections> sharedSection /*, autres paramètres éventuels */) :
    loco(loco), allSections(sharedSection)
{
    // Eventuel code supplémentaire du constructeur
}

PcoSemaphore LocomotiveBehavior::mutexSections(1);

void LocomotiveBehavior::run()
{
    //Initialisation de la locomotive
    loco.allumerPhares();
    loco.demarrer();
    loco.afficherMessage("Ready!");

    /* A vous de jouer ! */

    // Vous pouvez appeler les méthodes de la section partagée comme ceci :
    //sharedSection->request(loco);
    //sharedSection->getAccess(loco);
    //sharedSection->leave(loco);

    while(1) {}
}

void LocomotiveBehavior::printStartMessage()
{
    qDebug() << "[START] Thread de la loco" << loco.numero() << "lancé";
    loco.afficherMessage("Je suis lancée !");
}

void LocomotiveBehavior::printCompletionMessage()
{
    qDebug() << "[STOP] Thread de la loco" << loco.numero() << "a terminé correctement";
    loco.afficherMessage("J'ai terminé");
}
