#include <stdio.h>
#include <stdbool.h>
#include "Stack.h"
int main(void) {
	Stack* stack;
    int out;
    int value;
    bool hasData;
	stack = stack_new();
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
	
	while (1) {
		hasData = pop(stack, &value);
		if (hasData == false) {
			break;
		}
		printf("value:%d\n", value);
	}
	return 0;
}
