#include <stdio.h>
#include <stdbool.h>
#include "Stack.h"

int set_sort_rule(int data, int data2) {
	if (data > data2) {
		return 1; // desecending
	} else {
		return 0; // ascending
	}
}

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
	sort(stack, set_sort_rule);

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
	sort(stack, set_sort_rule);
	printf("size:%d\n", length);
	stack_free(stack); 
	return 0;
}
