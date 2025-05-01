#ifndef FACTORY_MANAGER_H
#define FACTORY_MANAGER_H

#include "belt_struct.h"

int tokenizar_linea(char *linea, const char *delim, void *tokens, const unsigned int type);
unsigned int parse_file(const char *filename, char **filebuff);

int MAX_BELTS; // Max number of processes active
belt *BELTS; // Array to store the belt structures

sem_t FACTORY_SEM;

#endif
