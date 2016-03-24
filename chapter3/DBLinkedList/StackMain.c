#include <stdio.h>
#include <stdbool.h>
#include "Stack.h"
int main(void) {
	Stack* stack;
    Stack* m;
    int out;
    int value;
	int length;
	stack = stack_new();
	length = size(stack);
	printf("size:%d\n", length);
    
	out = 3;
	push(stack, &out);
    out = 4;
	push(stack, &out);
    out = 5;
	push(stack, &out);
    out = 6;
	push(stack, &out);
    out = 7;
	push(stack, &out);
	length = size(stack);
	printf("size:%d\n", length);
	
	while (empty(stack)) {
		pop(stack, &value);
		printf("value:%d\n", value);
	}
	out = 5;
	push(stack, &out);
	stack_free(stack);
    push(m, &out);
	return 0;
}
