#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>

#include "queue.h"
#include "process_manager.h"
#include "belt_struct.h"

int BELT_ID; // ID of the belt
int BELT_SIZE; // Size of the belt
int BELT_NUM_ELEMS; // Number of elements to produce with the belt


void *process_manager(void *belt_ptr) {
	// Cast belt struct to avoid miss writing it
	belt *current_belt = belt_ptr;
	// Malloc return value so it's read in parent thread (factory_manager)
	int *result = malloc(sizeof(int));


	// Wait for factory_manager to start the thread
	sem_wait(&current_belt -> semaphore_b);

	// Processing belt information
	BELT_ID = current_belt -> id;
	BELT_SIZE = current_belt -> size;
	BELT_NUM_ELEMS = current_belt -> number_of_elements;
	fprintf(stdout, "[OK][process_manager] Process_manager with id %d waiting to produce %d elements.\n", BELT_ID, BELT_NUM_ELEMS);

	// Send to factory_manager to continue starting the following threads
	sem_post(&FACTORY_SEM);
	// Wait for the signal of the factory manager to begin working on the belt
	sem_wait(&current_belt -> semaphore_b);

	// Create belt after signal from factory_manager
	queue_init(BELT_SIZE);
	fprintf(stdout, "[OK][process_manager] Belt with id %d has been created with a maximum of %d elements.\n", BELT_ID, BELT_SIZE);

	// Create and initialize producer and consumer threads
	pthread_t produced_thread, consumer_thread;
	pthread_create(&produced_thread, NULL, producer, NULL);
	pthread_create(&consumer_thread, NULL, consumer, NULL);

	// Synchronization mechanism to wait for the producer and consumer functions to finish
	pthread_join(produced_thread, NULL);
	pthread_join(consumer_thread, NULL);
	fprintf(stdout, "[OK][process_manager] Process_manager with id %d has produced %d elements.\n", BELT_ID, BELT_NUM_ELEMS);

	// Assign exit status to return var
	*result = EXIT_SUCCESS;
   	pthread_exit(result);
}

void *producer() {
	for (int i = 0; i < BELT_NUM_ELEMS; i++) {
		element *elem = malloc(sizeof(element));
		if (!elem) {
			fprintf(stderr, "[ERROR][process_manager] There was an error executing process_manager with id %d.\n", BELT_ID);
			pthread_exit(NULL);
		}

		// Fill element with data
		elem -> num_edition = i;
		elem -> id_belt = BELT_ID;
		if (i == BELT_NUM_ELEMS - 1) {
			elem -> last = 1;
		}
		else {
			elem -> last = 0;
		}

		queue_put(elem); // Put element inside the queue
	}

	pthread_exit(NULL);
}

void *consumer() {
	for (int i = 0; i < BELT_NUM_ELEMS; i++) {
		element *elem = queue_get();
		if (!elem) {
			fprintf(stderr, "[ERROR][process_manager] There was an error executing process_manager with id %d.\n", BELT_ID);
			continue;
		}

		free(elem); // Free memory after consuming
	}

	pthread_exit(NULL);
}