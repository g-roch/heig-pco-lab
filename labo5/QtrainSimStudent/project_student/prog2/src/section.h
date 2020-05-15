#ifndef SECTION_H
#define SECTION_H

#include <map>

class Section : public std::pair<int, int>
{
public:
    Section(int, int);
};

#endif // SECTION_H
