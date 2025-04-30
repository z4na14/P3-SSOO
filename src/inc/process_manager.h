#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include "belt_struct.h"

void *PrintHello(void *threadid);
void *process_manager(void *current_belt);

extern int MAX_BELTS; // Max number of processes active

#endif
