#ifndef BELT_STRUCT_H
#define BELT_STRUCT_H

typedef struct {
    int id; // ID of the process
    int size; // Size of the belt from the process
    int number_of_elements; // Number of elements to produce using the belt
    sem_t semaphore_b; // Semaphore assigned to the corresponding belt
    pthread_t thread_b; // Thread assigned to the corresponding belt
} belt;

#endif
