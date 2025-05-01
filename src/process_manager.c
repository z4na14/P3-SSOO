#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stddef.h>
#include <pthread.h>
#include <semaphore.h>

#include "queue.h"
#include "process_manager.h"


static int BELT_ID; // ID of the belt
static int BELT_SIZE; // Size of the belt
static int BELT_NUM_ELEMS; // Number of elements to produce with the belt

/*
//Thread function
void *PrintHello(void *threadid) {
	int tid = *(int*)threadid;
   	printf("Hello World! It's me, thread #%d!\n", tid);
	printf("Thread #%d ends\n", tid);
   	pthread_exit(0);
}
*/


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




/*
	fprintf(stdout, "[OK][process_manager] Belt with id %d has been created with a maximum of %d elements.\n", BELT_ID, BELT_SIZE);






	fprintf(stdout, "[OK][process_manager] Process_manager with id %d has produced %d elements.\n", BELT_ID, BELT_NUM_ELEMS);
*/

	// Assign exit status to return var
	*result = EXIT_SUCCESS;
   	pthread_exit(result);
}