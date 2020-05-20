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

ParcoursConstIterator Parcours::cbegin() const {
  return ParcoursConstIterator(*this, 0);
}
ParcoursConstIterator Parcours::cend() const {
  return ParcoursConstIterator(*this, size());
}
ParcoursConstIterator Parcours::crbegin() const {
  return ParcoursConstIterator(*this, size()-1, -1);
}
ParcoursConstIterator Parcours::crend() const {
  return ParcoursConstIterator(*this, -1, -1);
}

ParcoursConstIterator::ParcoursConstIterator(Parcours const& parcours, size_t position, int direction)
  : parcours(parcours), position(position), direction(direction) {

  }
ParcoursConstIterator& ParcoursConstIterator::operator = (ParcoursConstIterator const & other) {
  ((Parcours&) parcours) = other.parcours;
  position = other.position;
  direction = other.direction;
  return *this;
}
bool ParcoursConstIterator::operator != (ParcoursConstIterator const & other) const {
  return position != other.position or &parcours != &other.parcours;
}
ParcoursConstIterator& ParcoursConstIterator::operator ++() {
  position += direction;
  return *this;
}
Parcours::aiguilles const& ParcoursConstIterator::getAiguillages(int i) const {
  if(direction == 1)
    return parcours.getAiguillages(position+i);
  else
    return parcours.getAiguillages(position-(i-1));
}
Section ParcoursConstIterator::getSection(int i) const {
  return Section(
      parcours.getPtPassage(position+(i+1)*direction),
      parcours.getPtPassage(position+i*direction)
      );
}
bool ParcoursConstIterator::last(unsigned i) const {
  if(direction == 1)
    return position >= parcours.size()-i;
  else
    return position < i;
}
bool ParcoursConstIterator::first() const {
  if(direction == 1)
    return position == 0;
  else
    return position == parcours.size()-1;
}

int ParcoursConstIterator::getPtPassage() const {
  return parcours.getPtPassage(position);
}
