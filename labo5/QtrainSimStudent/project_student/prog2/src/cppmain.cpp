//    ___  _________    ___  ___  ___  ___  //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  |/ _ \ //
//  / ___/ /__/ /_/ / / __// // / __// // / //
// /_/   \___/\____/ /____/\___/____/\___/  //
//                                          //
// Auteurs : Roch Gabriel, Wojciechowski Cassandre
//
#include "ctrain_handler.h"

#include "locomotive.h"
#include "locomotivebehavior.h"
#include "sharedsectioninterface.h"
#include "sharedsection.h"
#include "allsections.h"
#include "parcours.h"

// Locomotives :
// Vous pouvez changer les vitesses initiales, ou utiliser la fonction loco.fixerVitesse(vitesse);
// Laissez les numéros des locos à 0 et 1 pour ce laboratoire

// Locomotive A
static Locomotive locoA(7 /* Numéro (pour commande trains sur maquette réelle) */, 10 /* Vitesse */);
// Locomotive B
static Locomotive locoB(42 /* Numéro (pour commande trains sur maquette réelle) */, 12 /* Vitesse */);

// Status arret urgence
static bool arretUrgence = false;

//Arret d'urgence
void emergency_stop()
{
  // On arrête les deux locomotives
  locoA.arreter();
  locoB.arreter();

  // On indique aux threads de s'arreter s'ils en ont la possibilité
  arretUrgence = true;

  afficher_message("\nSTOP!");
}


//Fonction principale
int cmain()
{
  /************
   * Maquette *
   ************/

  //Choix de la maquette (A ou B)
  selection_maquette(MAQUETTE_A /*MAQUETTE_B*/);

  /**********************************
   * Initialisation des aiguillages *
   **********************************/

  // Initialisation des aiguillages
  // Positiion de base donnée comme exemple, vous pouvez la changer comme bon vous semble
  // Vous devrez utiliser cette fonction pour la section partagée pour aiguiller les locos
  // sur le bon parcours (par exemple à la sortie de la section partagée) vous pouvez l'
  // appeler depuis vos thread des locos par ex.
  diriger_aiguillage(1,  TOUT_DROIT, 0);
  diriger_aiguillage(2,  DEVIE     , 0);
  diriger_aiguillage(3,  DEVIE     , 0);
  diriger_aiguillage(4,  TOUT_DROIT, 0);
  diriger_aiguillage(5,  TOUT_DROIT, 0);
  diriger_aiguillage(6,  TOUT_DROIT, 0);
  diriger_aiguillage(7,  TOUT_DROIT, 0);
  diriger_aiguillage(8,  DEVIE     , 0);
  diriger_aiguillage(9,  DEVIE     , 0);
  diriger_aiguillage(10, TOUT_DROIT, 0);
  diriger_aiguillage(11, TOUT_DROIT, 0);
  diriger_aiguillage(12, TOUT_DROIT, 0);
  diriger_aiguillage(13, TOUT_DROIT, 0);
  diriger_aiguillage(14, DEVIE     , 0);
  diriger_aiguillage(15, DEVIE     , 0);
  diriger_aiguillage(16, TOUT_DROIT, 0);
  diriger_aiguillage(17, TOUT_DROIT, 0);
  diriger_aiguillage(18, TOUT_DROIT, 0);
  diriger_aiguillage(19, TOUT_DROIT, 0);
  diriger_aiguillage(20, DEVIE     , 0);
  diriger_aiguillage(21, DEVIE     , 0);
  diriger_aiguillage(22, TOUT_DROIT, 0);
  diriger_aiguillage(23, TOUT_DROIT, 0);
  diriger_aiguillage(24, TOUT_DROIT, 0);
  // diriger_aiguillage(/*NUMERO*/, /*TOUT_DROIT | DEVIE*/, /*0*/);

  /********************************
   * Position de départ des locos *
   ********************************/

  // Loco 0
  // Exemple de position de départ
  locoA.fixerPosition(25, 32);

  // Loco 1
  // Exemple de position de départ
  locoB.fixerPosition(22, 28);

  /***********
   * Message *
   **********/

  // Affiche un message dans la console de l'application graphique
  afficher_message("Hit play to start the simulation...");

  /*********************
   * Threads des locos *
   ********************/

  // Création des parcours
  Parcours parcoursB;
  parcoursB.addPtPassage(22);
  parcoursB.addPtPassage(21, {{16, TOUT_DROIT}});
  parcoursB.addPtPassage(20);
  parcoursB.addPtPassage(19, {{13, TOUT_DROIT}});
  parcoursB.addPtPassage(13);
  parcoursB.addPtPassage(12, {{10, TOUT_DROIT}});
  parcoursB.addPtPassage(11);
  parcoursB.addPtPassage(10, {{7, TOUT_DROIT}});
  parcoursB.addPtPassage(4);
  parcoursB.addPtPassage(3, {{4, TOUT_DROIT}});
  parcoursB.addPtPassage(2);
  parcoursB.addPtPassage(1, {{1, TOUT_DROIT}});
  parcoursB.addPtPassage(31);
  parcoursB.addPtPassage(30, {{22, TOUT_DROIT}});
  parcoursB.addPtPassage(29);
  parcoursB.addPtPassage(28, {{19, TOUT_DROIT}});

  // Création des parcours
  Parcours parcoursA;
  parcoursA.addPtPassage(25);
  parcoursA.addPtPassage(24, {{15, DEVIE}, {17, TOUT_DROIT}});
  parcoursA.addPtPassage(23, {{14, DEVIE}});
  parcoursA.addPtPassage(16);
  parcoursA.addPtPassage(15, {{9, DEVIE}});
  parcoursA.addPtPassage(10, {{8, TOUT_DROIT}, {7, DEVIE}});
  parcoursA.addPtPassage(4);
  parcoursA.addPtPassage(6, {{4, DEVIE}, {3, TOUT_DROIT}});
  parcoursA.addPtPassage(5, {{2, DEVIE}});
  parcoursA.addPtPassage(34);
  parcoursA.addPtPassage(33, {{21, DEVIE}});
  parcoursA.addPtPassage(32, {{20, DEVIE}, {23, TOUT_DROIT}});


  // Création des sections partagées
  std::shared_ptr<AllSections> sharedSection = std::make_shared<AllSections>(&arretUrgence);
  sharedSection->addSection({11, 15, 10, 4, 3, 6});

  // Création du thread pour la loco 0
  std::unique_ptr<Launchable> locoBehaveA = std::make_unique<LocomotiveBehavior>(locoA, sharedSection, parcoursA, SharedSectionInterface::Priority::LowPriority, &arretUrgence);
  // Création du thread pour la loco 1
  std::unique_ptr<Launchable> locoBehaveB = std::make_unique<LocomotiveBehavior>(locoB, sharedSection, parcoursB, SharedSectionInterface::Priority::HighPriority, &arretUrgence);

  // Lanchement des threads
  afficher_message(qPrintable(QString("Lancement thread loco A (numéro %1)").arg(locoB.numero())));
  locoBehaveA->startThread();
  afficher_message(qPrintable(QString("Lancement thread loco B (numéro %1)").arg(locoA.numero())));
  locoBehaveB->startThread();

  // Attente sur la fin des threads
  locoBehaveA->join();
  locoBehaveB->join();

  //Fin de la simulation
  mettre_maquette_hors_service();

  return EXIT_SUCCESS;
}
