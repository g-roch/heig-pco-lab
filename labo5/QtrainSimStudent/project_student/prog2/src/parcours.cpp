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

int Parcours::getPtPassage(size_t i) const {
    return passage.at(i);
}

Parcours::aiguilles const& Parcours::getAiguillages(size_t i) const{
    return aiguillage.at(i);
}

ParcoursIterator Parcours::begin() {
    return ParcoursIterator(*this, 0);
}

ParcoursIterator Parcours::end() {
    return ParcoursIterator(*this, size());
}

ParcoursIterator Parcours::rbegin() {
    return ParcoursIterator(*this, size()-1, -1);
}

ParcoursIterator Parcours::rend() {
    return ParcoursIterator(*this, -1, -1);
}
