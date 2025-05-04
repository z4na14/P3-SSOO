#ifndef QUEUE_H
#define QUEUE_H

#include "belt_struct.h"

typedef struct {
  int num_edition; // Order inside the belt
  int id_belt; // ID of the belt where the object is created
  int last; // 1 if inserted the last one, 0 otherwise
} element;

int queue_init(const belt *current_belt) ;
int queue_destroy(const belt *current_belt);
int queue_put(const belt *current_belt, element *elem);
element *queue_get(const belt *current_belt);
int queue_empty();
int queue_full(const belt *current_belt);

#endif
