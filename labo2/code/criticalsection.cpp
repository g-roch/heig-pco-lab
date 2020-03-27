#include "criticalsection.h"

#include <thread>

void WonderfulCriticalSection::initialize(unsigned int nbThreads)
{
    level.resize(nbThreads);
    last_to_enter.resize(nbThreads-1);
}

void WonderfulCriticalSection::lock(unsigned int index)
{
    for(int i = 0; (unsigned) i < last_to_enter.size(); ++i){
        level[index] = i;
        last_to_enter[i] = index;
        bool exists = true;

        while(last_to_enter[i] == index && exists){
            exists = false;
            for(unsigned j = 0; j < level.size(); ++j){
                if(j == index) continue;
                if(level[j] >= i){
                    exists = true;
                }

            }
            std::this_thread::yield();
        }
    }
}

void WonderfulCriticalSection::unlock(unsigned int index)
{
    level[index] = -1;
}
