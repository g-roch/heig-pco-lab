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
LocomotiveBehavior::LocomotiveBehavior(Locomotive& loco, std::shared_ptr<AllSections> sharedSection, Parcours& parcours /*, autres paramètres éventuels */) :
    loco(loco), allSections(sharedSection), parcours(parcours)
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

    for(size_t i = 0 ; i < parcours.size(); ++i) {
        Parcours::aiguilles const & aiguilles = parcours.getAiguillages(i);
        for(Parcours::aiguille const & a : aiguilles) {
           diriger_aiguillage(a.first, a.second, 0);
        }
        int nextContact ;
        if(i == parcours.size() -1)
            nextContact = parcours.getPtPassage(0);
        else
            nextContact = parcours.getPtPassage(i+1);
        Section s (parcours.getPtPassage(i), nextContact);
        allSections->get(s)->getAccess(loco, SharedSectionInterface::Priority::LowPriority);
        attendre_contact(parcours.getPtPassage(i));
        int lastContact ;
        if(i == 0)
            lastContact = parcours.getPtPassage(parcours.size() -1);
        else
            lastContact = parcours.getPtPassage(i-1);
        s = Section(parcours.getPtPassage(i), lastContact);
        allSections->get(s)->leave(loco);
    }

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
