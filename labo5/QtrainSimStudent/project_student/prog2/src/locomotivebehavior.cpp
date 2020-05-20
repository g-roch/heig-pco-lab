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
                                       SharedSectionInterface::Priority priority,
                                       bool * arretUrgence
                                    ) :
                                       loco(loco),
                                       allSections(sharedSection),
                                       parcours(parcours),
                                       priority(priority),
                                       arretUrgence(arretUrgence)
{ }


void LocomotiveBehavior::run()
{
  //Initialisation de la locomotive
  loco.allumerPhares();
  loco.demarrer();
  loco.afficherMessage("Ready!");

  const unsigned int NB_TOURS = 2;

  Section section (0, 0);
  ParcoursIterator begin = parcours.cbegin();
  ParcoursIterator end = parcours.cend();

  // Init. de la direction qu'on modifiera pour faire demi-tour
  int direction = 1;

  // on est sur la section de départ
  allSections->get(parcours.cbegin().getSection(-1))->getAccess(loco, priority);

  while(1){
      // Selon la direction, on va commencer d'un côté ou de l'autre du parcours créé
      if(direction == 1) {
        begin = parcours.cbegin();
        end = parcours.cend();
      } else {
          begin = parcours.crbegin();
          end = parcours.crend();
      }


    for(unsigned int j = 0; j < NB_TOURS; j++){
        bool last_tour = j == NB_TOURS-1;
        bool first_tour = j == 0;

      // On se lance dans le parcours
      for(ParcoursIterator it = begin; it != end; ++it) {


          // On demande l'accès à la section suivante
            allSections->get(it.getSection())->getAccess(loco, priority);

          // On dirige l'aiguillage de la section suivante
          for(Parcours::aiguille const & a : it.getAiguillages(+1)) {
            diriger_aiguillage(a.first, a.second, 0);
          }

          // On indique nos intention pour une section d'avance
          if(not it.last(1) or not last_tour) {
              section = it.getSection(+1);
          } else { // Si on vas faire demi-tour, la prochaine section, c'est la section sur laquel on est !
              section = it.getSection(-1);
          }
          allSections->get(section)->request(loco, priority);

          // On attends le contact que si c'est pas le premier d'un premier tour
          // ceci afin d'éviter que la loco fasse un tour à vide sans contrôle dans
          // le cas ou l'inertie n'est pas actif.
          if(not first_tour or not it.first())
              attendre_contact(it.getPtPassage());

          // On informe la section que l'on vient de la quitter
            allSections->get(it.getSection(-1))->leave(loco);
       }
    }

    // On arrête la locomotive, on la tourne et on la redémarre
    this->loco.arreter();
    this->loco.inverserSens();
    this->loco.demarrer();

    // On change la direction de la boucle pour faire demi-tour
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
