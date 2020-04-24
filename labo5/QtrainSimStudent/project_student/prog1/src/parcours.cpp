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
