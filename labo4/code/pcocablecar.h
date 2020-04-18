//    ___  _________    ___  ___  ___  ___  //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  |/ _ \ //
//  / ___/ /__/ /_/ / / __// // / __// // / //
// /_/   \___/\____/ /____/\___/____/\___/  //
//                                          //
// Auteurs : Prénom Nom, Prénom Nom

#ifndef PCOCABLECAR_H
#define PCOCABLECAR_H

#include "cablecar.h"
#include <pcosynchro/pcosemaphore.h>

/*!
 * \brief La classe PcoCableCar implémente la classe abstraite CableCar, la classe PcoCableCar
 * doit fournir les méthodes pour modéliser le comportement du télécabine ainsi que les méthodes qui
 * permettent aux skieurs d'intéragir avec le télécabine.
 */
class PcoCableCar : public CableCar
{
public:

    /*!
     * \brief constructeur qui permet de définir la capaicité du télécabine
     * \param capacity La capacité (nombre de places) dans le télécabine
     * Par défaut la capacité est de 10.
     */
    PcoCableCar(const unsigned int capacity = 10);

    /*!
     * \brief destructeur de la classe PcoCableCar
     */
    ~PcoCableCar();

    /********************************************
     * Méthodes de l'interface pour les skieurs *
     *******************************************/

    /*!
     * \brief Permet à un skieur d'attendre le télécabine (en bas à la station)
     * \param id L'id du skieur qui attend
     */
    void waitForCableCar(int id) override;

    /*!
     * \brief Permet à un skieur d'attendre dans le télécabine jusqu'à ce qu'il arrive en haut
     * \param id L'id du skieur qui attend
     */
    void waitInsideCableCar(int id) override;

    /*!
     * \brief Permet à un skieur de monter dans le télécabine
     * \param id L'id du skieur qui monte
     */
    void goIn(int id) override;

    /*!
     * \brief Permet à un skieur de descendre du télécabine
     * \param id L'id du skieur qui descend
     */
    void goOut(int id) override;

    /****************************************************
     * Méthodes pour l'interface du télécabine lui-même *
     ***************************************************/

    /*!
     * \brief Représente le fait que le télécabine monte
     */
    void goUp() override;

    /*!
     * \brief Représente le fait que le télécabine descend
     */
    void goDown() override;

    /*!
     * \brief Représente le fait que le télécabine charge des skieurs
     */
    void loadSkiers() override;

    /*!
     * \brief Représente le fait que le télécabine décharge des skieurs
     */
    void unloadSkiers() override;

    /**********************
     * Méthodes générales *
     *********************/

    /*!
     * \brief Permet de savoir si le télécabine est en service
     * \return retourn vrai si le télécabine est en service
     */
    bool isInService() override;

    /*!
     * \brief Permet de mettre fin au service du télécabine
     */
    void endService() override;

protected:

    /*!
     * \brief Permet de mettre fin au service du télécabine
     */
    void realEndService();

    /*!
     * \brief La capacité du télécabine en nombre de skieurs (nombre de places)
     */
    const unsigned int capacity;

    /*!
     * \brief Le nombre de skieurs en attente sur le télécabine (en bas)
     */
    unsigned int nbSkiersWaiting = 0;

    /*!
     * \brief nbSkrsInside Le nombre de skieurs dans le télécabine
     */
    unsigned int nbSkiersInside = 0;

    /*!
     * \brief L'état du service du télécabine
     */
    bool inService = true;

    /*!
     * \brief Le service de la télécabine se termine la prochaine fois que l'on arrive en-bas
     * \author Cassandre Wojciechowki
     * \author Gabriel Roch
     */
    bool serviceIsEnding = false;

    /*!
     * \brief Les deux mutex permettent de gérer la concurrence d'accès pour les variables NbSkierWaiting et NbSkierInside
     */
    PcoSemaphore mutexNbSkierWaiting;
    PcoSemaphore mutexNbSkierInside;

    /*!
     * \brief Sémaphore permettant de s'assurer qu'uniquement le bon nombre de skieurs monte dans la télécabine
     */
    PcoSemaphore semaphoreSkierGoIn;

    /*!
     * \brief Sémaphore permettant de s'assurer que tous les skieurs sortent de la télécabine
     */
    PcoSemaphore semaphoreSkierGoOut;

    /*!
     * \brief Sémaphore permettant de s'assurer que tous les skieurs sont dans la télécabine avant le départ
     */
    PcoSemaphore semaphoreSkierWaitGoUp;

    /*!
     * \brief Sémaphore permettant de s'assurer que tous les skieurs sont en dehors de la télécabine
     */
    PcoSemaphore semaphoreSkierWaitGoDown;

};

#endif // PCOCABLECAR_H
