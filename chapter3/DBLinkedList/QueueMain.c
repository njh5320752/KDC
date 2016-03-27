#include <stdio.h>
#include <stdbool.h>
#include "Queue.h"

int main(void) {
	Queue* queue;
	int out;
	int length;
	int value;
	queue = NULL;
	queue = queue_new();
	length = size(queue);
	printf("size:%d\n", length);

	out = 1;		    
	push(queue, &out);
	out = 2;
	push(queue, &out);
	out = 3;
	push(queue, &out);
	out = 4;
	push(queue, &out);
	out = 5;
	push(queue, &out);
	length = size(queue);
	printf("size:%d\n", length);

	while (empty(queue)) {
		pop(queue, &value);
		printf("value:%d\n", value);
	}   
	length = size(queue);
	printf("size:%d\n", length);
	out = 5;
	push(queue, &out);
	while (empty(queue)) {
		pop(queue, &value);
		printf("value:%d\n", value);
	}
	length = size(queue);
	printf("size:%d\n", length);
	queue_free(queue);
	return 0;
}
