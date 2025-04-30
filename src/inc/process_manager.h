#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

void *PrintHello(void *threadid);
int process_manager(int id, int belt_size, int items_to_produce);

typedef struct {
    unsigned int id; // ID of the process
    unsigned int size; // Size of the belt from the process
    unsigned int number_of_elements; // Number of elements to produce using the belt
    unsigned int pool_pos; // Position in the THREAD_ARR_PROCESSES pool array
} belt;

extern int MAX_BELTS; // Max number of processes active
extern sem_t BELTS_SEM; // Semaphore for the total amount of processes active at the same time
extern pthread_t *THREAD_ARR_PROCESSES; // Array storing the thread pool used for the different processe

#endif
