#include "section.h"

Section::Section(int a, int b) : std::pair<int, int>(a < b ? a : b, a < b ? b : a)
{

}
