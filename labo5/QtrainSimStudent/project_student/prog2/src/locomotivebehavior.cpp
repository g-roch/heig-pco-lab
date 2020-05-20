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
  ParcoursIterator begin = parcours.cbegin();
  ParcoursIterator end = parcours.cend();

  int direction = 1;
  while(1){
    for(unsigned int j = 0; j < NB_TOURS; j++){
        if(direction == 1) {
          begin = parcours.cbegin();
          end = parcours.cend();
        }
        else {
            begin = parcours.crbegin();
            end = parcours.crend();
        }
      for(ParcoursIterator it = begin; it != end; ++it) {

          for(Parcours::aiguille const & a : it.getAiguillages()) {
            diriger_aiguillage(a.first, a.second, 0);
          }
          if(not it.last()) {
            s = it.getNextSection();
            allSections->get(s)->getAccess(loco, priority);
          }
          if(not it.lastOrBeforeLast()) {
              s = it.getNextSection(2);
              allSections->get(s)->request(loco, priority);
          }
          attendre_contact(it.getPtPassage());

          if(not it.first()) {
            s = it.getLastSection();
            allSections->get(s)->leave(loco);
          }
      }

    }

    this->loco.arreter();
    this->loco.inverserSens();
    this->loco.demarrer();
    direction *= -1;
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
