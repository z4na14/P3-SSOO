#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

void *PrintHello(void *threadid);
int process_manager(int id, int belt_size, int items_to_produce);

#endif
