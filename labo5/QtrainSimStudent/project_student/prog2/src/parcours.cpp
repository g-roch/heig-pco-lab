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
