#include "allsections.h"

AllSections::AllSections() : mutex(1), sections()
{

}

std::shared_ptr<SharedSection> AllSections::get(Section section)
{
    mutex.acquire();
    std::shared_ptr<SharedSection> ret ;

    if(sections.count(section) == 0) {
        sections.insert({section, std::make_shared<SharedSection>(&sections[section])});
    }
    ret = sections.at(section);
    //= std::make_shared<SharedSection>(&sections[section]);
    //std::shared_ptr<SharedSection> ret = &sections[section];
    mutex.release();

    return ret;
}
