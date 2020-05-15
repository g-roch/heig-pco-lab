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
    std::string name = "";
    name += std::to_string(section.first);
    name += " ";
    name += std::to_string(section.second);
    sections.insert({section, std::make_shared<SharedSection>(name)});
  }
  ret = sections.at(section);
  mutex.release();

  return ret;
}

  void 
AllSections::addSection(std::vector<int> contacts)
{
  std::string name = "";
  for(int i : contacts) {
    name += std::to_string(i);
    name += ' ';
  }
  std::shared_ptr<SharedSection> section = std::make_shared<SharedSection>(name);
  for(int a : contacts) {
    for(int b : contacts) {
      if(a != b) {
        sections.insert({Section(a, b), section});
      }
    }
  }
}
