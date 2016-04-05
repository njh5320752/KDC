#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Queue.h"
#include "DBLinkedList.h"

struct _Queue
{
	DList* list;
};

bool empty(Queue* queue) {
	return ((queue->list) != NULL) ? true : false;
}

int size(Queue* queue) {
	if (!queue) {
		return -1;
	}
	return d_list_length(queue->list);
}

bool push(Queue* queue, void *in) {
	if (!queue) {
		return false;
	}
	queue->list = d_list_append((queue->list), in);
	if (!(queue->list)) {
		return false;
	}
	return true;
}

void* pop(Queue* queue) {
	DList* firstNode;
    DList* data;
	if (!queue) {
		printf("Can't pop\n");
		return false;
	}
	firstNode = d_list_first(queue->list);
    if (!firstNode) {
        printf("Can't pop\n");
        return NULL;
    }
	data = d_list_get_data(firstNode);
	queue->list = d_list_delete(queue->list);
	return data;
}
	
Queue* queue_new() {
	Queue* queue = (Queue*) malloc(sizeof(Queue));
	queue->list = NULL;
	return queue;
}

void queue_free(Queue* queue, void(*free_data)(void *data)) {
	d_list_free(queue->list, free_data);
	free(queue);
	return;
}
