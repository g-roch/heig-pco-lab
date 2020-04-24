#ifndef PARCOURS_H
#define PARCOURS_H

#include <vector>
#include <map>
#include <set>

class Parcours
{
using aiguille = std::pair<int, int>;
using aiguilles = std::set<aiguille>;

public:
    Parcours();
    void addPtPassage(int point);
    void addPtPassage(int point, aiguilles aiguillage);

private:
    std::vector<int> passage;
    std::vector<aiguilles> aiguillage;

};

#endif // PARCOURS_H
