#ifndef PARCOURS_H
#define PARCOURS_H

#include <vector>
#include <map>
#include <set>

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

private:
    std::vector<int> passage;
    std::vector<aiguilles> aiguillage;

};

#endif // PARCOURS_H
