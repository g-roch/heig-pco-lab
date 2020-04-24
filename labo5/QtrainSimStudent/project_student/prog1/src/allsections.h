#ifndef ALLSECTIONS_H
#define ALLSECTIONS_H

#include "pcosynchro/pcosemaphore.h"
#include <map>
#include "section.h"
#include "sharedsection.h"

class AllSections
{
public:
    AllSections();

    std::shared_ptr<SharedSection> get(Section section);
private :
    PcoSemaphore mutex;
    //std::map<Section, SharedSection> sections;
     std::map<Section, std::shared_ptr<SharedSection>> sections;

};

#endif // ALLSECTIONS_H
