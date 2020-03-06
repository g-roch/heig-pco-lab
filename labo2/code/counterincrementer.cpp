#include "counterincrementer.h"

static volatile long unsigned int counter;

void runTask(CriticalSection *criticalSection, unsigned long nbIterations, int id)
{
    long unsigned int i = 0;

    while (i < nbIterations)
    {
        criticalSection.lock(id);
	std::atomic_thread_fence();
        counter++;
	std::atomic_thread_fence();
        criticalSection.unlock(id);
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
