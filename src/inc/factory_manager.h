#ifndef FACTORY_MANAGER_H
#define FACTORY_MANAGER_H

int tokenizar_linea(char *linea, const char *delim, void *tokens, const unsigned int type);
unsigned int parse_file(const char *filename, char **filebuff);

typedef struct {
    unsigned int id; // ID of the process
    unsigned int size; // Size of the belt from the process
    unsigned int number_of_elements; // Number of elements to produce using the belt
    unsigned int pool_pos; // Position in the THREAD_ARR_PROCESSES pool array
} belt;

int MAX_BELTS; // Max number of processes active
belt *BELTS; // Array to store the belt structures
sem_t BELTS_SEM; // Semaphore for the total amount of processes active at the same time
pthread_t *THREAD_ARR_PROCESSES; // Array storing the thread pool used for the different processe

#endif
