#include "allsections.h"

AllSections::AllSections() : mutex(1), sections()
{

}

AllSections::~AllSections()
{

}

//SharedSection* AllSections::get(Section section)
std::shared_ptr<SharedSection> AllSections::get(Section section)
{
    mutex.acquire();
    std::shared_ptr<SharedSection> ret ;

    if(sections.count(section) == 0) {
        sections.insert({section, std::make_shared<SharedSection>()});
    }
    ret = sections.at(section);
    mutex.release();

    return ret;
}
