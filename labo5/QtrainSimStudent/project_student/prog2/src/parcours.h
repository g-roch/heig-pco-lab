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
class ParcoursIterator;

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

    // Obtention d'informations sur le parcours : la taille, le point de passage et l'aiguillage
    size_t size() const noexcept;
    int getPtPassage(int) const;
    aiguilles const& getAiguillages(int) const;

    // Itérateurs permettant de parcourir le parcours
    ParcoursIterator cbegin();
    ParcoursIterator cend();
    ParcoursIterator crbegin();
    ParcoursIterator crend();
private:
    std::vector<int> passage;
    std::vector<aiguilles> aiguillage;
};

/*
 * La classe "ParcoursIterator" permet de définir des opérateurs et de redéfinir des méthodes de "Parcours"
 * Cette classe a été implémentée pour permettre d'utiliser une seule boucle for() au moment du demi-tour de
 * la locomotive après 2 tours de circuit
 */
class ParcoursIterator {
public:
    using aiguille = std::pair<int, int>;
    using aiguilles = std::set<aiguille>;
private:
    Parcours const& parcours;
    size_t position;
    int direction;
public:
    ParcoursIterator(Parcours const& parcours, size_t position, int direction = 1);

    ParcoursIterator& operator = (ParcoursIterator const & other);
    bool operator != (ParcoursIterator const & other) const;
    ParcoursIterator& operator ++();

    /*!
     * \brief getAiguillages Donne l'aiguillage selon la direction suivie et la position de la locomotive
     * (utile dans le cas du demi-tour fait au bout de 2 tours)
     */
    Parcours::aiguilles const& getAiguillages(int i = 0) const;
    /*!
     * \brief getNextSection Donne la prochaine section parcourue en fonction de la position et de la direction
     * \param i position
     */
    Section getSection(int i=0) const;

    /*!
     * \brief last Permet de savoir si on arrive a la fin du parcours dans un sens ou dans l'autre
     */
    bool last(unsigned i = 1) const;

    /*!
     * \brief first Permet de savoir si on est au début du parcours dans un sens ou dans l'autre
     */
    bool first() const;

    /*!
     * \brief getPtPassage Donne le point de passage en fonction de la position de la loco
     */
    int getPtPassage() const;
};

#endif // PARCOURS_H
