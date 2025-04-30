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

	// Wait for the signal of the factory manager to begin working on the belt
	sem_wait(&current_belt -> semaphore_b);

	printf("UWU %d\n", current_belt -> id);

	// Assign exit status to return var
	*result = EXIT_SUCCESS;
   	pthread_exit(result);
}