#include "counterincrementer.h"
#include <thread>
static volatile long unsigned int counter;

void runTask(CriticalSection *criticalSection, unsigned long nbIterations, int id)
{
    long unsigned int i = 0;

    while (i < nbIterations)
    {
        criticalSection->lock(id);
        std::atomic_thread_fence(std::memory_order_acq_rel);
        counter++;
        std::atomic_thread_fence(std::memory_order_acq_rel);
        criticalSection->unlock(id);

        i++;
    }
}

void initCounter()
{
    counter = 0;
}

long unsigned int getCounter()
{
    return counter;
}
