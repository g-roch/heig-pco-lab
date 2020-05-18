//    ___  _________    ___  ___  ___  ___  //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  |/ _ \ //
//  / ___/ /__/ /_/ / / __// // / __// // / //
// /_/   \___/\____/ /____/\___/____/\___/  //
//                                          //
// Auteurs : Roch Gabriel, Wojciechowski Cassandre
//
#include "section.h"

/*
 *  On crée la section en plaçant le plus petit nombre en premier
 */
Section::Section(int a, int b) : std::pair<int, int>(a < b ? a : b, a < b ? b : a)
{

}
