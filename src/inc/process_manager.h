#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include "belt_struct.h"

void *PrintHello(void *threadid);
void *process_manager(void *current_belt);

extern int MAX_BELTS; // Max number of processes active
extern sem_t FACTORY_SEM; // Factory semaphore to synch with child threads

#endif
