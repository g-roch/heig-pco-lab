//    ___  _________    ___  ___  ___  ___  //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  |/ _ \ //
//  / ___/ /__/ /_/ / / __// // / __// // / //
// /_/   \___/\____/ /____/\___/____/\___/  //
//                                          //
// Auteurs : Roch Gabriel, Wojciechowski Cassandre
//
#include "parcours.h"

Parcours::Parcours() { }

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

int Parcours::getPtPassage(int i) const {
  return passage.at((i+size())%size());
}

Parcours::aiguilles const& Parcours::getAiguillages(int i) const {
  return aiguillage.at((i+size())%size());
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
Parcours::aiguilles const& ParcoursIterator::getAiguillages(int i) const {
  if(direction == 1)
    return parcours.getAiguillages(position+i);
  else
    return parcours.getAiguillages(position-(i-1));
}
Section ParcoursIterator::getSection(int i) const {
  return Section(
      parcours.getPtPassage(position+(i+1)*direction),
      parcours.getPtPassage(position+i*direction)
      );
}
bool ParcoursIterator::last(unsigned i) const {
  if(direction == 1)
    return position >= parcours.size()-i;
  else
    return position < i;
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
