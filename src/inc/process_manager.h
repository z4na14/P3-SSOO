#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

void *process_manager(void *current_belt);
void *producer();
void *consumer();

extern int MAX_BELTS; // Max number of processes active
extern sem_t FACTORY_SEM; // Factory semaphore to synch with child threads

#endif
