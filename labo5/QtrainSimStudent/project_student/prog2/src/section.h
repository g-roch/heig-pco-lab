//    ___  _________    ___  ___  ___  ___  //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  |/ _ \ //
//  / ___/ /__/ /_/ / / __// // / __// // / //
// /_/   \___/\____/ /____/\___/____/\___/  //
//                                          //
// Auteurs : Roch Gabriel, Wojciechowski Cassandre
//
#ifndef SECTION_H
#define SECTION_H

#include <map>

/*
 * La classe "Section" permet de créer les sections des rails en les
 * identifiant avec les deux nombres qui l'encadrent
 */
class Section : public std::pair<int, int>
{
public:
    /*!
     * \brief Section constructeur de la classe "Section"
     * \param a Premier point de passage de la section
     * \param b Second point de passage de la section
     */
    Section(int a, int b);
};

#endif // SECTION_H
