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
LocomotiveBehavior::LocomotiveBehavior(Locomotive& loco, std::shared_ptr<AllSections> sharedSection, Parcours& parcours, SharedSectionInterface::Priority priority /*, autres paramètres éventuels */) :
  loco(loco), allSections(sharedSection), parcours(parcours), priority(priority)
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
  Section s (0, 0);
  while(1){
    for(unsigned int j = 0; j < NB_TOURS; j++){

      for(size_t i = 1 ; i < parcours.size(); ++i) {
        for(Parcours::aiguille const & a : parcours.getAiguillages(i)) {
          diriger_aiguillage(a.first, a.second, 0);
        }
        if(i != parcours.size() -1) {
          int nextContact = parcours.getPtPassage(i+1);
          s = Section(parcours.getPtPassage(i), nextContact);
          allSections->get(s)->getAccess(loco, priority);
        }
        if(i < parcours.size() - 2) {
            int a = parcours.getPtPassage(i+1);
            int b = parcours.getPtPassage(i+2);
            s = Section(a, b);
            allSections->get(s)->request(loco, priority);
        }
        attendre_contact(parcours.getPtPassage(i));
        printf("D\n");
        int lastContact ;
        if(i != 0) {
          lastContact = parcours.getPtPassage(i-1);
          s = Section(parcours.getPtPassage(i), lastContact);
          allSections->get(s)->leave(loco);
        }
      }

    }

    this->loco.arreter();
    this->loco.inverserSens();
    this->loco.demarrer();

    for(unsigned int j = 0; j < NB_TOURS; j++){
      printf("g\n");
      for(int i = parcours.size() - 2 ; i >= 0 ; --i) {
        if(i != 0) {
          int nextContact = parcours.getPtPassage(i-1);
          s = Section(parcours.getPtPassage(i), nextContact);
          allSections->get(s)->getAccess(loco, priority);
        }
        if(i > 1) {
            int a = parcours.getPtPassage(i-1);
            int b = parcours.getPtPassage(i-2);
          s = Section(a, b);
          allSections->get(s)->request(loco, priority);
        }
        printf("3eme b\n");
        attendre_contact(parcours.getPtPassage(i));
        printf("c\n");
        // Définir aiguilles
        Parcours::aiguilles const & aiguilles = parcours.getAiguillages(i);
        for(Parcours::aiguille const & a : aiguilles) {
          diriger_aiguillage(a.first, a.second, 0);
        }
        int lastContact ;
        if(i != parcours.size() - 1) {
          lastContact = parcours.getPtPassage(i+1);
          printf("d\n");
          s = Section(parcours.getPtPassage(i), lastContact);
          allSections->get(s)->leave(loco);
        }

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
