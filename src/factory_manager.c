#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stddef.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include "process_manager.h"
#include "factory_manager.h"


int main (const int argc, const char *argv[]){
	//int* status;

	if (argc != 2) {
		errno = EINVAL;
		perror("Invalid arguments");
		exit(EXIT_FAILURE);
	}

	// Get information from the input file and store it into file_buffer
	char* file_buffer = 0;
	int buffer_size = parse_file(argv[1], &file_buffer);

	if (tokenizar_linea(file_buffer, "\n", NULL, 0) != 1) {
		errno = EINVAL;
		perror("Invalid number of arguments inside input file");
		exit(EXIT_FAILURE);
	}

	// Divide into tokens the input line from the buffer
	int tokens[buffer_size / 2];
	memset(tokens, 0, sizeof(tokens));

	int num_info = tokenizar_linea(file_buffer, "\t\n ", tokens, 1);

	// If we don't have an even number from the input, it means that the input is incorrect
	if (((num_info - 1) % 3) != 0) {
		errno = EINVAL;
		perror("Wrong number of arguments inside input file");
		exit(EXIT_FAILURE);
	}


	// Populate global variables with the contents of the input file
	MAX_BELTS = tokens[0];
	// Initialize semaphore with the maximum number of process managers
	sem_init(&BELTS_SEM, 0, MAX_BELTS);
	// Create array with the maximum number of threads we are going to be using
	THREAD_ARR_PROCESSES = malloc(sizeof(pthread_t *) * MAX_BELTS);

	// Allocate memory for storing the belts
	BELTS = malloc(sizeof(belt) * MAX_BELTS);
	// Move all the information into the global array
	int num_of_belts = 0;
	for (int i = 1; i < num_info; i+=3) {
		BELTS[num_of_belts] = (belt) {tokens[i], tokens[i+1], tokens[i+2]};
		num_of_belts++;
	}
	// Free the buffer with the contents of the input
	free(file_buffer);


	// For each belt that must be processes
	for (int i = 0; i < num_of_belts; i++) {
		// perform wait on parent thread
		sem_wait(&BELTS_SEM);

		// get the current semaphore value and use as index in the thread pool
		int curr_semaphore;
		sem_getvalue(&BELTS_SEM, &curr_semaphore);
		curr_semaphore = MAX_BELTS - curr_semaphore - 1;

		// and create a new one
		// pthread_create(&THREAD_ARR_PROCESSES[curr_semaphore], NULL, (void*)process_manager, (void*)curr_semaphore);
		pthread_create(&THREAD_ARR_PROCESSES[curr_semaphore], NULL, PrintHello, &curr_semaphore);
		//pthread_join(THREAD_ARR_PROCESSES[curr_semaphore], NULL);
	}


	free(THREAD_ARR_PROCESSES);
	free(BELTS);

	return 0;
}


/**
 * This function splits a char* line into different tokens based on a given character.
 * @param linea line to tokenize
 * @param delim line delimiter
 * @param tokens where to store the tokens
 * @param type 1: integer, 0: string
 * @return Number of tokens
 */
int tokenizar_linea(char *linea, const char *delim, void *tokens, const unsigned int type) {
	if (linea == NULL) {
		errno = EINVAL;
		perror("Invalid arguments inside input");
		exit(EXIT_FAILURE);
	}

	int i = 0;
	char *token = strtok(linea, delim);
	char *err = 0;

	// If no place to store tokens is passed, return just the number of findings
	if (tokens == NULL) {
		while (token != NULL) {
			i++;
			token = strtok(NULL, delim);
		}
		return i;
	}

	int *tokens_ptr_int = NULL;
	char **tokens_ptr_str = NULL;

	if (type) {tokens_ptr_int = (int *) tokens;}
	else {tokens_ptr_str = (char **) &tokens;}

	while (token != NULL) {
		if (type) {
			tokens_ptr_int[i++] = (int) strtol(token, &err, 10);

			if (*err != '\0') {
				perror("Invalid number inside input file");
				exit(EXIT_FAILURE);
			}
		}
		else {tokens_ptr_str[i++] = token;}

		token = strtok(NULL, delim);
	}

	// Null-terminate the string array
	if (!type) { tokens_ptr_str[i] = NULL;}

	return i;
}

/**
 * Function used to parse the input file and return the contents inside a buffer. \0 delimits
 * are also added when processing it.
 * @param filename Filename string where the commands are stored.
 * @param filebuff Buffer where to store dynamically the contents of the file
 * @returns Size of the buffer.
 */
unsigned int parse_file(const char *filename, char **filebuff) {
	// Open file
	int filefd;
	if ((filefd = open(filename, O_RDWR, 0664)) < 0) {
		perror("Error opening scripting file");
		exit(EXIT_FAILURE);
	}

	// Define buffer with size from file
	struct stat fd_st;
	if (fstat(filefd, &fd_st) < 0) {
		perror("Error doing stat to file");
		exit(EXIT_FAILURE);
	}

	// If the file is empty, exit the program
	if (fd_st.st_size == 0) {
		errno = EINVAL;
		perror("Script is empty");
		exit(EXIT_FAILURE);
	}

	// Allocate buffer dynamically
	unsigned int buffer_size = fd_st.st_size + 1;
	// Allocate memory for the buffer
	*filebuff = malloc(buffer_size);
	if (!*filebuff) {
		perror("Malloc failed");
		exit(EXIT_FAILURE);
	}

	// Read contents from file
	if (read(filefd, *filebuff, buffer_size - 1) < 0) {
		perror("Error reading file");
		exit(EXIT_FAILURE);
	}

	// Close the file with the commands :D
	close(filefd);

	return buffer_size;
}


