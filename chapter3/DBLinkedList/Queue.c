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

bool push(Queue* queue, int* out) {
	if (!queue) {
		return false;
	}
	queue->list = d_list_append((queue->list), *out);
	if (!(queue->list)) {
		return false;
	}
	return true;
}

bool pop(Queue* queue, int* out) {
	if (!queue && !(queue->list)) {
		printf("Can't pop\n");
		return false;
	}

	*out = d_list_get_data(queue->list);
	queue->list = d_list_remove_nth(queue->list, 0);
	return true;
}
	
Queue* queue_new() {
	Queue* queue = (Queue*) malloc(sizeof(Queue));
	queue->list = NULL;
	return queue;
}

void queue_free(Queue* queue) {
	d_list_free(queue->list);
	free(queue);
	return;
}
