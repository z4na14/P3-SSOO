#ifndef FACTORY_MANAGER_H
#define FACTORY_MANAGER_H

int tokenizar_linea(char *linea, const char *delim, void *tokens, const unsigned int type);
unsigned int parse_file(const char *filename, char **filebuff);

typedef struct {
    unsigned int id;
    unsigned int size;
    unsigned int number_of_elements;
} belt;

belt *BELTS;

#endif
