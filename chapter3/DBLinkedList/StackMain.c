#include <stdio.h>
#include "Stack.h"

int main(void) {
	Stack* stack;
	stack = stack_new();
	int data;
	push(stack, 3);
	push(stack, 4);
	push(stack, 5);
	push(stack, 6);
	push(stack, 7);
	
	while(data = pop(stack)) {
		printf("data:%d\n", data);
	}
	return 0;
}
