#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include "belt_struct.h"

void *PrintHello(void *threadid);
int process_manager(int id, int belt_size, int items_to_produce);

extern int MAX_BELTS; // Max number of processes active
extern sem_t BELTS_SEM; // Semaphore for the total amount of processes active at the same time
extern pthread_t *THREAD_ARR_PROCESSES; // Array storing the thread pool used for the different processe

#endif
