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
    const unsigned int NB_TOURS = 1;
    while(1){
        for(unsigned int j = 0; j < NB_TOURS; j++){

            for(size_t i = 1 ; i < parcours.size(); ++i) {
                printf("A - %i \n", (int) i);
                Parcours::aiguilles const & aiguilles = parcours.getAiguillages(i);
printf("A\n");
                for(Parcours::aiguille const & a : aiguilles) {
                   diriger_aiguillage(a.first, a.second, 0);
                }
printf("B\n");
                int nextContact ;
                if(i == parcours.size() -1)
                    nextContact = parcours.getPtPassage(0);
                else
                    nextContact = parcours.getPtPassage(i+1);
printf("C\n");
                Section s (parcours.getPtPassage(i), nextContact);
                allSections->get(s)->getAccess(loco, SharedSectionInterface::Priority::LowPriority);
                attendre_contact(parcours.getPtPassage(i));
printf("D\n");
                int lastContact ;
                if(i == 0)
                    lastContact = parcours.getPtPassage(parcours.size() -1);
                else
                    lastContact = parcours.getPtPassage(i-1);
printf("E\n");
                s = Section(parcours.getPtPassage(i), lastContact);
                allSections->get(s)->leave(loco);
            }

        }

        this->loco.arreter();
        this->loco.inverserSens();
        this->loco.demarrer();

        for(unsigned int j = 0; j < NB_TOURS; j++){
printf("g\n");
            for(int i = parcours.size() - 2 ; i >= 0 ; --i) {
                printf("h - %i\n", (int) i);
                Parcours::aiguilles const & aiguilles = parcours.getAiguillages((i == 0 ? parcours.size() : i) - 1);

                for(Parcours::aiguille const & a : aiguilles) {
                    diriger_aiguillage(a.first, a.second, 0);
                }
printf("a\n");
                int nextContact ;
                if(i == 0)
                    nextContact = parcours.getPtPassage(parcours.size() - 1);
                else
                    nextContact = parcours.getPtPassage(i-1);
printf("b %i\n", (int) i);
                Section s (parcours.getPtPassage(i), nextContact);
printf("b %i, %i\n", s.first, s.second);
// HELP : ici la loco s'arrête -> pb sur la section partagée qui doit être mal quittée au précédent passage
                allSections->get(s)->getAccess(loco, SharedSectionInterface::Priority::LowPriority);
printf("3eme b\n");
                attendre_contact(parcours.getPtPassage(i));
printf("c\n");
                int lastContact ;
                if(i == parcours.size() - 1)
                    lastContact = parcours.getPtPassage(0);
                else
                    lastContact = parcours.getPtPassage(i+1);
printf("d\n");
                s = Section(parcours.getPtPassage(i), lastContact);
                allSections->get(s)->leave(loco);
printf("e\n");
            }

        }

        this->loco.arreter();
        this->loco.inverserSens();
        this->loco.demarrer();
    }

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
