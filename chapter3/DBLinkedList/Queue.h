#ifndef __QUEUE_H__
#define __QUEUE_H__

typedef struct _Queue Queue;
bool empty(Queue* queue);
int size(Queue* queue);
bool push(Queue* queue, int* out);
bool pop(Queue* queue, int* out);
Queue* queue_new();
void queue_free(Queue* queue);
#endif

