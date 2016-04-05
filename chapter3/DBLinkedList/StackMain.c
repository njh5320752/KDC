#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "Stack.h"

void free_data(void *data) {
    int *remove = (int*)data;
    free(remove);
}
Stack* insert_data(int in);

int main(void) {
    Stack* stack;
	int length;
    int *d;
    void* data;
	stack = insert_data(10);
	length = size(stack);
	printf("size:%d\n", length);
    
	while (empty(stack)) {
		data = pop(stack);
        d = (int*)data;
		printf("data:%d\n", *d);
	}
    stack_free(stack, free_data);
	return 0; 
}

Stack* insert_data(int in) {
    int *data;
    int i;
    int random_number;
    Stack* stack;
    stack = NULL;
    srand((unsigned int)time(NULL));
    stack = stack_new();
    for (i = 0; i < in; i++) {   
        data = (int*)malloc(sizeof(int));
        random_number = rand() % in + 1;
        *data = random_number;
        printf("random number:%d\n", random_number);
        push(stack, data);
    }   
    return stack;
}

