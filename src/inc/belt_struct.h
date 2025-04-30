#ifndef BELT_STRUCT_H
#define BELT_STRUCT_H

typedef struct {
    unsigned int id; // ID of the process
    unsigned int size; // Size of the belt from the process
    unsigned int number_of_elements; // Number of elements to produce using the belt
    unsigned int pool_pos; // Position in the THREAD_ARR_PROCESSES pool array
} belt;

#endif
