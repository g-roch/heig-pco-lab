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
    int getPtPassage(size_t) const;
    aiguilles const& getAiguillages(size_t) const;

    // Itérateurs permettant de parcourir le parcours
    ParcoursIterator begin();
    ParcoursIterator end();
    ParcoursIterator rbegin();
    ParcoursIterator rend();

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

    Parcours& parcours;
    size_t position;
    int direction;

    ParcoursIterator(Parcours& parcours, size_t position, int direction = 1)
        : parcours(parcours), position(position), direction(direction) { }

    void operator = (ParcoursIterator const & other) {
        parcours = other.parcours;
        position = other.position;
        direction = other.direction;
    }

    bool operator != (ParcoursIterator const & other) const {
        return position != other.position or &parcours != &other.parcours;
    }

    void operator ++() {
        position += direction;
    }

    /*!
     * \brief getAiguillages Donne l'aiguillage selon la direction suivie et la position de la locomotive
     * (utile dans le cas du demi-tour fait au bout de 2 tours)
     */
    aiguilles const& getAiguillages() const {
        if(direction == 1)
            return parcours.getAiguillages(position);
        else
            if(position == 0)
                return parcours.getAiguillages(parcours.size() - 1);
            else
                return parcours.getAiguillages(position - 1);
    }

    /*!
     * \brief getNextSection Donne la prochaine section parcourue en fonction de la position et de la direction
     * \param i position
     */
    Section getNextSection(int i = 1) const {
        return Section(
                    parcours.getPtPassage(position + (i-1) * direction),
                    parcours.getPtPassage(position + i * direction)
                    );
    }

    /*!
     * \brief getLastSection Donne la dernière section
     * \param i position
     */
    Section getLastSection(int i = 1) const {
        return getNextSection(-i + 1);
    }

    /*!
     * \brief last Permet de savoir si on arrive a la fin du parcours dans un sens ou dans l'autre
     */
    bool last() const {
        if(direction == 1)
            return position == parcours.size() - 1;
        else
            return position == 0;
    }

    /*!
     * \brief lastOrBeforeLast Permet de savoir si on arrive vers la fin du parcours dans un sens ou dans l'autre
     */
    bool lastOrBeforeLast() const {
        if(direction == 1)
            return position >= parcours.size() - 2;
        else
            return position < 2;
    }

    /*!
     * \brief first Permet de savoir si on est au début du parcours dans un sens ou dans l'autre
     */
    bool first() const {
        if(direction == 1)
            return position == 0;
        else
            return position == parcours.size() - 1;
    }

    /*!
     * \brief getPtPassage Donne le point de passage en fonction de la position de la loco
     */
    int getPtPassage() const {
        return parcours.getPtPassage(position);
    }
};

#endif // PARCOURS_H
