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

    ParcoursIterator begin();
    ParcoursIterator end();
    ParcoursIterator rbegin();
    ParcoursIterator rend();
private:
    std::vector<int> passage;
    std::vector<aiguilles> aiguillage;

};

class ParcoursIterator {
public:
using aiguille = std::pair<int, int>;
using aiguilles = std::set<aiguille>;
//private:
    Parcours& parcours;
    size_t position;
    int direction;
public:
    ParcoursIterator(Parcours& parcours, size_t position, int direction = 1)
        : parcours(parcours), position(position), direction(direction) {

    }
    ParcoursIterator& operator = (ParcoursIterator const & other) {
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
    aiguilles const& getAiguillages() const {
        if(direction == 1)
            return parcours.getAiguillages(position);
        else
            if(position == 0)
                return parcours.getAiguillages(parcours.size()-1);
            else
                return parcours.getAiguillages(position-1);
        }
    Section getNextSection(int i = 1) const {
        return Section(
                    parcours.getPtPassage(position+(i-1)*direction),
                    parcours.getPtPassage(position+i*direction)
                    );
    }
    Section getLastSection(int i = 1) const {
        return getNextSection(-i+1);
    }
    bool last() const {
        if(direction == 1)
            return position == parcours.size()-1;
        else
            return position == 0;
    }
    bool lastOrBeforeLast() const {
        if(direction == 1)
            return position >= parcours.size()-2;
        else
            return position < 2;
    }
    bool first() const {
        if(direction == 1)
            return position == 0;
        else
            return position == parcours.size()-1;
    }
    int getPtPassage() const {
        return parcours.getPtPassage(position);
    }
};

#endif // PARCOURS_H
