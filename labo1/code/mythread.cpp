#include "mythread.h"
#include <unistd.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

static volatile long unsigned int counter;
static volatile int busy = 0;

void runTask(unsigned long nbIterations)
{
    long unsigned int i = 0;

    int id = 0;
    // ID unique par thread (gettid() ne foncionne pas ??)
    while(!id) id = rand();

    while (i < nbIterations)
    {
	while (busy != id) {
		// Attend la libération de la ressource
		while (busy);

		// Demande le lock
		busy = id;
	}
        counter++;
	busy = 0;
        i++;
    }
}

void initCounter()
{
	srand (time(NULL));
    counter = 0;
    busy = 0;
}

long unsigned int getCounter()
{
    return counter;
}
