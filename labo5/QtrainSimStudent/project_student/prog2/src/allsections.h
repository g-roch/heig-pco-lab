//    ___  _________    ___  ___  ___  ___  //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  |/ _ \ //
//  / ___/ /__/ /_/ / / __// // / __// // / //
// /_/   \___/\____/ /____/\___/____/\___/  //
//                                          //
// Auteurs : Roch Gabriel, Wojciechowski Cassandre
//
#ifndef ALLSECTIONS_H
#define ALLSECTIONS_H

#include "pcosynchro/pcosemaphore.h"
#include <map>
#include "section.h"
#include "sharedsection.h"

/*
 * La classe "AllSections" permet de répertorier toutes les sections
 * composant les rails dans une map
 */
class AllSections
{
public:
    AllSections(bool * arretUrgence);
    ~AllSections();

    std::shared_ptr<SharedSection> get(Section section);

    void addSection(std::vector<int> contacts);

private :
    PcoSemaphore mutex;
    std::map<Section, std::shared_ptr<SharedSection>> sections;
    bool * arretUrgence;
};

#endif // ALLSECTIONS_H
