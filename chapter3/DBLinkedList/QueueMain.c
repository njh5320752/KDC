#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "Queue.h"

void free_data(void *data) {
    int *remove = (int*)data;
    free(remove);
}

Queue* insert_data(int in);
int main(void) {
    Queue* queue;
	void* data;
	int length;
	int* value;
    queue = insert_data(10);
    length = size(queue);
    printf("size:%d\n", length);

	while (empty(queue)) {
		data = pop(queue);
        value = (int*)data;
		printf("value:%d\n", *value);
	}

    queue_free(queue, free_data);
	return 0; 
}

Queue* insert_data(int in) {
    int *data;
    int i;
    int random_number;
    Queue* queue;
    queue = NULL;
    srand((unsigned int)time(NULL));
    queue = queue_new();
    for (i = 0; i < in; i++) {   
        data = (int*)malloc(sizeof(int));
        random_number = rand() % in + 1;
        *data = random_number;
        printf("random number:%d\n", random_number);
        push(queue, data);
    }   
    return queue;
}

