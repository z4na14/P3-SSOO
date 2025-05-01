#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "queue.h"

extern int MAX_BELTS; // Max number of processes active
extern sem_t FACTORY_SEM; // Factory semaphore to synch with child threads

extern int BELT_ID; // ID of the belt currently being processed
extern int BELT_SIZE; // Size of the belt currently being processed
extern int BELT_NUM_ELEMS; // Number of elements to produce with the belt currently being processed

static element **BUFFER; // Buffer for the producer - consumer solution
static sem_t EMPTY_SLOTS_SEM; // Synchronization mechanism for the buffer
static sem_t FULL_SLOTS_SEM; // Synchronization mechanism for the buffer

static int tail_pos = 0; // Position of last element inside the buffer
static int count = 0; // Total number of elements inside the buffer


// Initialize the queue with a fixed size
int queue_init(const int size) {
    if (size <= 0) {
        fprintf(stderr, "[ERROR][queue] There was an error while using queue with id: %d.\n", BELT_ID);
        exit(EXIT_FAILURE);
    }

    BUFFER = malloc(size * sizeof(element *));
    if (!BUFFER) {
        fprintf(stderr, "[ERROR][queue] There was an error while using queue with id: %d.\n", BELT_ID);
        exit(EXIT_FAILURE);
    }

    sem_init(&EMPTY_SLOTS_SEM, 0, size); // Initially all slots are empty
    sem_init(&FULL_SLOTS_SEM, 0, 0); // Initially no slots are full

    return 0;
}

// Destroy the queue and free all resources
int queue_destroy(void) {
    free(BUFFER);
    sem_destroy(&EMPTY_SLOTS_SEM);
    sem_destroy(&FULL_SLOTS_SEM);

    return 0;
}

// Add an element to the queue (waits if full)
int queue_put(element *elem) {
    sem_wait(&EMPTY_SLOTS_SEM); // Check if there are enough empty slots
    BUFFER[elem -> num_edition % BELT_SIZE] = elem; // Add element to buffer
    sem_post(&FULL_SLOTS_SEM); // Send signal to full_slots semaphore
    count++; // Increase count of the number of elements

    fprintf(stdout, "[OK][queue] Introduced element with id %d in belt %d.\n", elem -> num_edition, BELT_ID);

    return 0;
}

// Get an element from the queue (waits if empty)
element *queue_get(void) {
    sem_wait(&FULL_SLOTS_SEM); // Check if there are any elements in the buffer
    element *elem = BUFFER[tail_pos]; // Retrieve element from buffer
    tail_pos = (tail_pos + 1) % BELT_SIZE;
    count--; // Decrease count of the number of elements

    fprintf(stdout, "[OK][queue] Obtained element with id %d in belt %d.\n", elem -> num_edition, BELT_ID);

    return elem;
}

// Check if the queue is empty
int queue_empty(void) {

    return 0;
}

// Check if the queue is full
int queue_full(void) {

    return 0;
}
