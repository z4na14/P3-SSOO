#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>

#include "process_manager.h"
#include "factory_manager.h"
#include "belt_struct.h"

int MAX_BELTS; // Max number of processes active
belt *BELTS; // Array to store the belt structures

sem_t FACTORY_SEM;


int main (const int argc, const char *argv[]){
	//int* status;

	if (argc != 2) {
		fprintf(stderr, "[ERROR][factory_manager] Invalid file\n");
		exit(EXIT_FAILURE);
	}

	// Get information from the input file and store it into file_buffer
	char* file_buffer = 0;
	int buffer_size = (int) parse_file(argv[1], &file_buffer);

	if (tokenizar_linea(file_buffer, "\n", NULL, 0) != 1) {
		fprintf(stderr, "[ERROR][factory_manager] Invalid file\n");
		exit(EXIT_FAILURE);
	}

	// Divide into tokens the input line from the buffer
	int tokens[buffer_size / 2];
	memset(tokens, 0, sizeof(tokens));

	int num_info = tokenizar_linea(file_buffer, "\t\n ", tokens, 1);


	// Populate global variables with the contents of the input file
	MAX_BELTS = tokens[0];
	// Allocate memory for storing the belts
	BELTS = malloc(sizeof(belt) * MAX_BELTS);
	// Move all the information into the global array
	int i, num_of_belts;
	for (i = 1, num_of_belts = 0; i < num_info; i+=3, num_of_belts++) {
		// Inputted information
		BELTS[num_of_belts] = (belt) {tokens[i], tokens[i+1], tokens[i+2]};
		// Semaphore initialization
		sem_init(&BELTS[num_of_belts].semaphore_b, 0, 0);
	}
	// Begin global factory semaphore to synchronize the start of the process threads
	sem_init(&FACTORY_SEM, 0, 0);

	// If we don't have an even number from the input, it means that the input is incorrect
	if (((num_info - 1) % 3) != 0 || num_of_belts >= tokens[0]) {
		fprintf(stderr, "[ERROR][factory_manager] Invalid file\n");
		exit(EXIT_FAILURE);
	}

	// Free the buffer with the contents of the input
	free(file_buffer);


	// For each belt that must be processes
	for (i = 0; i < num_of_belts; i++) {
		pthread_create(&BELTS[i].thread_b, NULL, process_manager, &BELTS[i]);
		fprintf(stdout, "[OK][factory_manager] Process_manager with id %d has been created.\n", BELTS[i].id);
	}

	for (i = 0; i < num_of_belts; i++) {
		// Send signal to start each thread in order
		sem_post(&BELTS[i].semaphore_b);
		// After starting the thread, wait for it to answer
		sem_wait(&FACTORY_SEM);
	}

	for (i = 0; i < num_of_belts; i++) {
		// Send signal to begin processing each belt in order
		sem_post(&BELTS[i].semaphore_b);

		// Wait for the process to finish, get its return value, and go back to the beginning of
		// the loop to launch the following thread
		void *thread_return;
		if (pthread_join(BELTS[i].thread_b, &thread_return) != 0 || *(int*)thread_return != 0) {
			fprintf(stderr, "[ERROR][factory_manager] Process_manager with id %d has finished with errors.\n", BELTS[i].id);
		}
		else {
			fprintf(stdout, "[OK][factory_manager] Process_manager with id %d has finished.\n", BELTS[i].id);
		}

		free(thread_return);
	}


	free(BELTS);

	fprintf(stdout, "[OK][factory_manager] Finishing.\n");

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
		fprintf(stderr, "[ERROR][factory_manager] Invalid file\n");
		exit(EXIT_FAILURE);
	}

	int index = 0;
	char *token = strtok(linea, delim);
	char *err = 0;

	// If no place to store tokens is passed, return just the number of findings
	if (tokens == NULL) {
		while (token != NULL) {
			index++;
			token = strtok(NULL, delim);
		}
		return index;
	}

	// Divide into two possible input strings, depending on the type passed as argument
	int *tokens_ptr_int = NULL;
	char **tokens_ptr_str = NULL;

	if (type) {tokens_ptr_int = (int *) tokens;}
	else {tokens_ptr_str = (char **) &tokens;}

	// While there are tokens found
	while (token != NULL) {
		if (type) {
			// If type = 1: Integer, convert into integer
			tokens_ptr_int[index++] = (int) strtol(token, &err, 10);
			// and check for errors during the conversion
			if (*err != '\0') {
				fprintf(stderr, "[ERROR][factory_manager] Invalid file\n");
				exit(EXIT_FAILURE);
			}
		}
		// Otherwise, add to the array directly
		else {tokens_ptr_str[index++] = token;}

		// and get the following token to add
		token = strtok(NULL, delim);
	}

	// Null-terminate the string array
	if (!type) {tokens_ptr_str[index] = NULL;}

	return index;
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
		fprintf(stderr, "[ERROR][factory_manager] Invalid file\n");
		exit(EXIT_FAILURE);
	}

	// Define buffer with size from file
	struct stat fd_st;
	if (fstat(filefd, &fd_st) < 0) {
		fprintf(stderr, "[ERROR][factory_manager] Invalid file\n");
		exit(EXIT_FAILURE);
	}

	// If the file is empty, exit the program
	if (fd_st.st_size == 0) {
		fprintf(stderr, "[ERROR][factory_manager] Invalid file\n");
		exit(EXIT_FAILURE);
	}

	// Allocate buffer dynamically
	unsigned int buffer_size = fd_st.st_size + 1;
	// Allocate memory for the buffer
	*filebuff = malloc(buffer_size);
	if (!*filebuff) {
		fprintf(stderr, "[ERROR][factory_manager] Process_manager with id 0 has finished with errors.\n");
		exit(EXIT_FAILURE);
	}

	// Read contents from file
	if (read(filefd, *filebuff, buffer_size - 1) < 0) {
		fprintf(stderr, "[ERROR][factory_manager] Invalid file\n");
		exit(EXIT_FAILURE);
	}

	// Close the file with the commands :D
	close(filefd);

	return buffer_size;
}
