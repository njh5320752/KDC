#include <stdio.h>
#include <stdbool.h>
#include "Stack.h"

int main(void) {
	Stack* stack;
    int out;
    int value;
	int length;
	stack = stack_new();
	length = size(stack);
	printf("size:%d\n", length);
    
	out = 1;
	push(stack, &out);
    out = 3;
	push(stack, &out);
    out = 5;
	push(stack, &out);
    out = 7;
	push(stack, &out);
    out = 9;
	push(stack, &out);
	length = size(stack);
	printf("size:%d\n", length);

	while (empty(stack)) {
		pop(stack, &value);
		printf("value:%d\n", value);
	}
	out = 9;
	push(stack, &out);
	out = 7;
	push(stack, &out);
	out = 5;
	push(stack, &out);
	out = 3;
	push(stack, &out);
	out = 1;
	push(stack, &out);

	printf("size:%d\n", length);
	while (empty(stack)) {
		pop(stack, &value);
		printf("value:%d\n", value);
	}
	length = size(stack);
	out = 9;
	push(stack, &out);
	printf("size:%d\n", length);
	stack_free(stack); 
	return 0;
}
