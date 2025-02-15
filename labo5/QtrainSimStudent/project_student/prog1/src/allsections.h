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
    ~AllSections();

    std::shared_ptr<SharedSection> get(Section section);

    void addSection(std::vector<int> contacts);

private :
    PcoSemaphore mutex;
    //std::map<Section, SharedSection> sections;
    std::map<Section, std::shared_ptr<SharedSection>> sections;
    //std::map<Section, SharedSection*> sections;

};

#endif // ALLSECTIONS_H
