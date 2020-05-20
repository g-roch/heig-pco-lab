#include "parcours.h"

Parcours::Parcours()
{

}

void Parcours::addPtPassage(int point){
    passage.push_back(point);
    aiguillage.resize(aiguillage.size() + 1);
}

void Parcours::addPtPassage(int point, aiguilles aig){
    passage.push_back(point);
    aiguillage.push_back(aig);
}

size_t Parcours::size() const noexcept {
    return passage.size();
}

int Parcours::getPtPassage(size_t i) const {
    return passage.at(i);

}
Parcours::aiguilles const& Parcours::getAiguillages(size_t i) const{
    return aiguillage.at(i);

}

ParcoursIterator Parcours::cbegin() {
    return ParcoursIterator(*this, 0);
}
ParcoursIterator Parcours::cend() {
    return ParcoursIterator(*this, size());
}
ParcoursIterator Parcours::crbegin() {
    return ParcoursIterator(*this, size()-1, -1);
}
ParcoursIterator Parcours::crend() {
    return ParcoursIterator(*this, -1, -1);
}

ParcoursIterator::ParcoursIterator(Parcours const& parcours, size_t position, int direction)
    : parcours(parcours), position(position), direction(direction) {

}
ParcoursIterator& ParcoursIterator::operator = (ParcoursIterator const & other) {
    ((Parcours&) parcours) = other.parcours;
    position = other.position;
    direction = other.direction;
    return *this;
}
bool ParcoursIterator::operator != (ParcoursIterator const & other) const {
    return position != other.position or &parcours != &other.parcours;
}
ParcoursIterator& ParcoursIterator::operator ++() {
    position += direction;
    return *this;
}
Parcours::aiguilles const& ParcoursIterator::getAiguillages() const {
    if(direction == 1)
        return parcours.getAiguillages(position);
    else
        if(position == 0)
            return parcours.getAiguillages(parcours.size()-1);
        else
            return parcours.getAiguillages(position-1);
 }
Section ParcoursIterator::getNextSection(int i) const {
    return Section(
                parcours.getPtPassage(position+(i-1)*direction),
                parcours.getPtPassage(position+i*direction)
                );
}
Section ParcoursIterator::getLastSection(int i) const {
    return getNextSection(-i+1);
}
bool ParcoursIterator::last() const {
    if(direction == 1)
        return position == parcours.size()-1;
    else
        return position == 0;
}
bool ParcoursIterator::lastOrBeforeLast() const {
    if(direction == 1)
        return position >= parcours.size()-2;
    else
        return position < 2;
}
bool ParcoursIterator::first() const {
    if(direction == 1)
        return position == 0;
    else
        return position == parcours.size()-1;
}

int ParcoursIterator::getPtPassage() const {
    return parcours.getPtPassage(position);
}
