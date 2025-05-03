#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#include "queue.h"
#include "belt_struct.h"


static element **BUFFER; // Buffer for the producer - consumer solution
static sem_t EMPTY_SLOTS_SEM; // Synchronization mechanism for the buffer
static sem_t FULL_SLOTS_SEM; // Synchronization mechanism for the buffer
static pthread_mutex_t QUEUE_MUTEX;

static int head_pos = 0; // Position of first element inside the buffer
static int tail_pos = 0; // Position of last element inside the buffer
static int count = 0; // Total number of elements inside the buffer


// Initialize the queue with a fixed size
int queue_init(belt *current_belt) {
    if (current_belt -> size <= 0) {
        fprintf(stderr, "[ERROR][queue] There was an error while using queue with id: %d.\n", current_belt -> id);
        exit(EXIT_FAILURE);
    }

    BUFFER = malloc(current_belt -> size * sizeof(element **));
    if (!BUFFER) {
        fprintf(stderr, "[ERROR][queue] There was an error while using queue with id: %d.\n", current_belt -> id);
        exit(EXIT_FAILURE);
    }
    memset(BUFFER, 0, current_belt -> size * sizeof(element **));

    sem_init(&EMPTY_SLOTS_SEM, 0, current_belt -> size); // Initially all slots are empty
    sem_init(&FULL_SLOTS_SEM, 0, 0); // Initially no slots are full
    pthread_mutex_init(&QUEUE_MUTEX, NULL); // Initialize mutual exclusion

    return 0;
}

// Destroy the queue and free all resources
int queue_destroy(belt *current_belt) {
    // Free any remaining elements in the buffer
    for (int i = 0; i < current_belt -> size; i++) {
        if (BUFFER[i] != NULL) {
            free(BUFFER[i]);
            BUFFER[i] = NULL;
        }
    }

    free(BUFFER);
    BUFFER = NULL;
    sem_destroy(&EMPTY_SLOTS_SEM);
    sem_destroy(&FULL_SLOTS_SEM);
    pthread_mutex_destroy(&QUEUE_MUTEX);

    return 0;
}

// Add an element to the queue (waits if full)
int queue_put(belt *current_belt, element *elem) {
    sem_wait(&EMPTY_SLOTS_SEM); // Check if there are enough empty slots
    pthread_mutex_lock(&QUEUE_MUTEX); // and lock the critical section while modifying it

    // Calculate insertion point BEFORE modifying count
    int insert_pos = (head_pos + 1) % current_belt -> size;
    BUFFER[head_pos] = elem;
    head_pos = insert_pos;  // Update head after insertion
    count++;

    pthread_mutex_unlock(&QUEUE_MUTEX); // Unlock the critical section
    sem_post(&FULL_SLOTS_SEM); // Send signal to full_slots semaphore

    fprintf(stdout, "[OK][queue] Introduced element with id %d in belt %d.\n",
        elem -> num_edition, current_belt -> id);

    return 0;
}

// Get an element from the queue (waits if empty)
element *queue_get(belt *current_belt) {
    sem_wait(&FULL_SLOTS_SEM); // Check if there are any elements in the buffer
    pthread_mutex_lock(&QUEUE_MUTEX); // Lock mutex and access critical section

    element *elem = BUFFER[tail_pos];
    if (elem == NULL) {  // Additional safety check
        pthread_mutex_unlock(&QUEUE_MUTEX);
        sem_post(&FULL_SLOTS_SEM);  // Restore semaphore count
        return NULL;
    }

    BUFFER[tail_pos] = NULL;
    tail_pos = (tail_pos + 1) % current_belt -> size;
    count--;

    pthread_mutex_unlock(&QUEUE_MUTEX);
    sem_post(&EMPTY_SLOTS_SEM);

    fprintf(stdout, "[OK][queue] Obtained element with id %d in belt %d.\n",
        elem -> num_edition, current_belt -> id);

    return elem;
}

// Check if the queue is empty
int queue_empty() {
    if (count == 0) return 1;
    return 0;
}

// Check if the queue is full
int queue_full(belt *current_belt) {
    if (count == current_belt -> size) return 1;
    return 0;
}
