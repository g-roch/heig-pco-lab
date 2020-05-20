//    ___  _________    ___  ___  ___  ___  //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  |/ _ \ //
//  / ___/ /__/ /_/ / / __// // / __// // / //
// /_/   \___/\____/ /____/\___/____/\___/  //
//                                          //
// Auteurs : Roch Gabriel, Wojciechowski Cassandre
//
#ifndef PARCOURS_H
#define PARCOURS_H

#include <vector>
#include <map>
#include <set>

#include"section.h"

/*
 * La classe "Parcours" permet de configurer le parcours que les locomotives
 * vont effectuer
 */
class ParcoursConstIterator;

class Parcours
{
  public:
    using aiguille = std::pair<int, int>;
    using aiguilles = std::set<aiguille>;

    Parcours();

    /*!
     * \brief addPtPassage Ajout d'un point de passage au parcours
     * \param point Point de passage à ajouter
     */
    void addPtPassage(int point);

    /*!
     * \brief addPtPassage Ajout d'un point de passage au parcours
     * \param point Point de passage à ajouter
     * \param aiguillage Aiguillage à ajouter au parcours (avec son orientation)
     */
    void addPtPassage(int point, aiguilles aiguillage);

    /*!
     * \brief Taille du parcours
     * \return la taille
     */
    size_t size() const noexcept;

    /*!
     * \brief Obtient un point de passage en fonction de sa position
     * \param i position du point de passage voulu
     * \return Numéro du point de passage
     */
    int getPtPassage(int i) const;

    /*!
     * \brief Obtien une liste d'aiguillages en fonction de sa position
     * \param i position de l'aiguillage voulu
     * \return liste des aiguillages
     */
    aiguilles const& getAiguillages(int i) const;

    // Itérateurs permettant de parcourir le parcours
    ParcoursConstIterator cbegin() const;
    ParcoursConstIterator cend() const;
    ParcoursConstIterator crbegin() const;
    ParcoursConstIterator crend() const;
  private:
    // Liste des points de passage et des aiguillages
    std::vector<int> passage;
    std::vector<aiguilles> aiguillage;
};

/*
 * La classe "ParcoursIterator" permet de définir des opérateurs et de redéfinir des méthodes de "Parcours"
 * Cette classe a été implémentée pour permettre d'utiliser une seule boucle for() au moment du demi-tour de
 * la locomotive après 2 tours de circuit
 */
class ParcoursConstIterator {
  public:
    using aiguille = std::pair<int, int>;
    using aiguilles = std::set<aiguille>;
  private:
    Parcours const& parcours;
    size_t position;
    int direction;
  public:
    ParcoursConstIterator(Parcours const& parcours, size_t position, int direction = 1);

    ParcoursConstIterator& operator = (ParcoursConstIterator const & other);
    bool operator != (ParcoursConstIterator const & other) const;
    ParcoursConstIterator& operator ++();

    /*!
     * \brief Donne l'aiguillage selon la direction suivie et la position de la locomotive
     * (utile dans le cas du demi-tour fait au bout de 2 tours)
     */
    Parcours::aiguilles const& getAiguillages(int i = 0) const;

    /*!
     * \brief Obtient une section relativement à la position actuelle et à la direction
     * \param i décalage à appliquer
     */
    Section getSection(int i=0) const;

    /*!
     * \brief Permet de savoir si on arrive a la fin du parcours dans un sens ou dans l'autre
     * \param i nombre d'élément encore necessaire
     */
    bool last(unsigned i = 1) const;

    /*!
     * \brief Permet de savoir si on est au début du parcours dans un sens ou dans l'autre
     * \param i nombre d'élément encore necessaire
     */
    bool first() const;

    /*!
     * \brief Donne le point de passage en fonction de la position de la loco
     */
    int getPtPassage() const;
};

#endif // PARCOURS_H
