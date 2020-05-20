//    ___  _________    ___  ___  ___  ___  //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  |/ _ \ //
//  / ___/ /__/ /_/ / / __// // / __// // / //
// /_/   \___/\____/ /____/\___/____/\___/  //
//                                          //
// Auteurs : Roch Gabriel, Wojciechowski Cassandre
//
#ifndef LOCOMOTIVEBEHAVIOR_H
#define LOCOMOTIVEBEHAVIOR_H

#include "locomotive.h"
#include "launchable.h"
#include "sharedsectioninterface.h"
#include "sharedsection.h"
#include "allsections.h"
#include "parcours.h"

#define NB_TOURS 2

/**
 * @brief La classe LocomotiveBehavior représente le comportement d'une locomotive
 */
class LocomotiveBehavior : public Launchable
{
  public:
    /*!
     * \brief Constructeur de la classe
     * \param loco la locomotive dont on représente le comportement
     * \param allSections Pointeur vers l'object de gestion de toutes les sections partagée
     * \param parcours Parcours que la loco doit faire
     * \param priority Priorité assignée à la loco
     * \param arretUrgence Pointeur vers une indication si l'arret d'urgence est déclenchée
     */
    LocomotiveBehavior(
        Locomotive& loco,
        std::shared_ptr<AllSections> allSections,
        Parcours& parcours,
        SharedSectionInterface::Priority priority,
        bool * arretUrgence);

  protected:
    /*!
     * \brief Fonction lancée par le thread, représente le comportement de la locomotive
     */
    void run() override;

    /*!
     * \brief Message affiché lors du démarrage du thread
     */
    void printStartMessage() override;

    /*!
     * \brief Message affiché lorsque le thread a terminé
     */
    void printCompletionMessage() override;

    /*!
     * \brief La locomotive dont on représente le comportement
     */
    Locomotive& loco;

    /*!
     * \brief Pointeur sur la gestion des sections partagées
     */
    std::shared_ptr<AllSections> allSections;

    /*!
     * \brief Parcours fait par la locomotive
     */
    Parcours& parcours;

    /*!
     * \brief Priorité de la locomotive pour l'accès à la section partagée
     */
    SharedSectionInterface::Priority priority;

    /*!
     * \brief permet de savoir si le boutton d'arret d'urgence est appuyé
     */
    bool * arretUrgence;
};
#endif
