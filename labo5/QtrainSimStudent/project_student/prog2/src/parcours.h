#ifndef PARCOURS_H
#define PARCOURS_H

#include <vector>
#include <map>
#include <set>

#include"section.h"

class ParcoursIterator;


class Parcours
{
public:
using aiguille = std::pair<int, int>;
using aiguilles = std::set<aiguille>;

public:
    Parcours();
    void addPtPassage(int point);
    void addPtPassage(int point, aiguilles aiguillage);

    size_t size() const noexcept;
    int getPtPassage(size_t) const;
    aiguilles const& getAiguillages(size_t) const;

    ParcoursIterator cbegin();
    ParcoursIterator cend();
    ParcoursIterator crbegin();
    ParcoursIterator crend();
private:
    std::vector<int> passage;
    std::vector<aiguilles> aiguillage;

};

class ParcoursIterator {
private:
    Parcours const& parcours;
    size_t position;
    int direction;
public:
    ParcoursIterator(Parcours const& parcours, size_t position, int direction = 1);

    ParcoursIterator& operator = (ParcoursIterator const & other);
    bool operator != (ParcoursIterator const & other) const;
    ParcoursIterator& operator ++();
    Parcours::aiguilles const& getAiguillages() const;
    Section getNextSection(int i = 1) const;
    Section getLastSection(int i = 1) const;
    bool last() const;
    bool lastOrBeforeLast() const;
    bool first() const;
    int getPtPassage() const;
};

#endif // PARCOURS_H
