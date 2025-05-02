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


void *process_manager(void *belt_ptr) {
	// Cast belt struct to avoid miss writing it
	belt *current_belt = belt_ptr;
	// Malloc return value so it's read in parent thread (factory_manager)
	int *result = malloc(sizeof(int));


	// Wait for factory_manager to start the thread
	sem_wait(&current_belt -> semaphore_b);

	if (current_belt -> id <= 0 || current_belt -> size <= 0 || current_belt -> number_of_elements <= 0) {
		fprintf(stderr, "[ERROR][process_manager] Arguments not valid");
		*result = EXIT_FAILURE;
		pthread_exit(result);
	}

	fprintf(stdout, "[OK][process_manager] Process_manager with id %d waiting to produce %d elements.\n",
	        current_belt->id, current_belt->number_of_elements);

	// Send to factory_manager to continue starting the following threads
	sem_post(&FACTORY_SEM);
	// Wait for the signal of the factory manager to begin working on the belt
	sem_wait(&current_belt -> semaphore_b);

	// Create belt after signal from factory_manager
	queue_init(current_belt);
	fprintf(stdout, "[OK][process_manager] Belt with id %d has been created with a maximum of %d elements.\n",
	        current_belt->id, current_belt->size);

	// Create and initialize producer and consumer threads
	pthread_t produced_thread, consumer_thread;
	pthread_create(&produced_thread, NULL, producer, belt_ptr);
	pthread_create(&consumer_thread, NULL, consumer, belt_ptr);

	// Synchronization mechanism to wait for the producer and consumer functions to finish

	void *thread_return;
	if (pthread_join(produced_thread, &thread_return) != 0 || *(int*)thread_return != 0 ||
		pthread_join(consumer_thread, &thread_return) != 0 || *(int*)thread_return != 0) {
		fprintf(stderr, "[ERROR][process_manager] There was an error executing process_manager with id %d.\n",
		        current_belt->id);
		// Assign exit status to return var
		*result = EXIT_FAILURE;
	}
	else {
		fprintf(stdout, "[OK][process_manager] Process_manager with id %d has produced %d elements.\n",
		        current_belt->id, current_belt->number_of_elements);
		*result = EXIT_SUCCESS;
	}

	// Destroy the queue before exiting
	queue_destroy(current_belt);

   	pthread_exit(result);
}

void *producer(void *belt_ptr) {
	// Cast belt struct to avoid miss writing it
	belt *current_belt = belt_ptr;
	// Malloc return value so it's read in parent thread (factory_manager)
	int *result = malloc(sizeof(int));

	for (int i = 0; i < current_belt -> number_of_elements; i++) {
		element *elem = malloc(sizeof(element));
		if (!elem) {
			fprintf(stderr, "[ERROR][process_manager] There was an error executing process_manager with id %d.\n",
			        current_belt->id);
			*result = EXIT_FAILURE;
			pthread_exit(result);
		}

		// Fill element with data
		elem -> num_edition = i;
		elem -> id_belt = current_belt -> id;
		if (i == current_belt -> number_of_elements - 1) {
			elem -> last = 1;
		}
		else {
			elem -> last = 0;
		}

		queue_put(current_belt, elem); // Put element inside the queue
	}

	*result = EXIT_SUCCESS;
	pthread_exit(result);
}

void *consumer(void *belt_ptr) {
	// Cast belt struct to avoid miss writing it
	belt *current_belt = belt_ptr;
	// Malloc return value so it's read in parent thread (factory_manager)
	int *result = malloc(sizeof(int));

	for (int i = 0; i < current_belt -> number_of_elements; i++) {
		element *elem = queue_get(current_belt);
		// Verify this is the expected element
		if (!elem || elem -> id_belt != current_belt -> id) {
			fprintf(stderr, "[ERROR][process_manager] There was an error executing process_manager with id %d.\n",
			        current_belt->id);
			*result = EXIT_FAILURE;
			pthread_exit(result);
		}

		free(elem); // Free memory after consuming
		elem = NULL;
	}

	*result = EXIT_SUCCESS;
	pthread_exit(result);
}