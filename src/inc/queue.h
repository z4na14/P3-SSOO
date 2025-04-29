#ifndef QUEUE_H
#define QUEUE_H

struct element {
  int num_edition; // Order inside the belt
  int id_belt; // ID of the belt where the object is created
  int last; // 1 if inserted the last one, 0 otherwise
};

int queue_init (int size);
int queue_destroy (void);
int queue_put (struct element* elem);
struct element *queue_get(void);
int queue_empty (void);
int queue_full(void);

#endif
