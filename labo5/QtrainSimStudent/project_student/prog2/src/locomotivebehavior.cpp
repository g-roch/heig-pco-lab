//    ___  _________    ___  ___  ___  ___  //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  |/ _ \ //
//  / ___/ /__/ /_/ / / __// // / __// // / //
// /_/   \___/\____/ /____/\___/____/\___/  //
//                                          //
// Auteurs : Roch Gabriel, Wojciechowski Cassandre
//
#include "locomotivebehavior.h"
#include "ctrain_handler.h"

/*!
 * \brief locomotiveBehavior Constructeur de la classe
 * \param loco la locomotive dont on représente le comportement
 */
LocomotiveBehavior::LocomotiveBehavior(Locomotive& loco,
                                       std::shared_ptr<AllSections> sharedSection,
                                       Parcours& parcours,
                                       SharedSectionInterface::Priority priority)
                                       :
                                       loco(loco),
                                       allSections(sharedSection),
                                       parcours(parcours),
                                       priority(priority) { }

PcoSemaphore LocomotiveBehavior::mutexSections(1);

void LocomotiveBehavior::run()
{
  //Initialisation de la locomotive
  loco.allumerPhares();
  loco.demarrer();
  loco.afficherMessage("Ready!");

  const unsigned int NB_TOURS = 2;

  Section s (0, 0);
  ParcoursIterator begin = parcours.cbegin();
  ParcoursIterator end = parcours.cend();

  // Init. de la direction qu'on modifiera pour faire demi-tour
  int direction = 1;

  while(1){
    for(unsigned int j = 0; j < NB_TOURS; j++){
        // Selon la direction, on va commencer d'un côté ou de l'autre du parcours créé
        if(direction == 1) {
          begin = parcours.cbegin();
          end = parcours.cend();
        } else {
            begin = parcours.crbegin();
            end = parcours.crend();
        }

      // On se lance dans le parcours
      for(ParcoursIterator it = begin; it != end; ++it) {
          // On dirige l'aiguillage
          for(Parcours::aiguille const & a : it.getAiguillages()) {
            diriger_aiguillage(a.first, a.second, 0);
          }

          // Tant qu'on n'est pas à la fin, on demande la section suivante à parcourir et on essaie d'y accéder
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

    // On arrête la locomotive, on la tourne et on la redémarre
    this->loco.arreter();
    this->loco.inverserSens();
    this->loco.demarrer();

    // On change la direction pour faire demi-tour
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
