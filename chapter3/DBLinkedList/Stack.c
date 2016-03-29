#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Stack.h"
#include "DBLinkedList.h"

struct _Stack
{
	 DList* list;
};

bool push(Stack* stack, int in) {
    if (!stack) {
        return false;
    }
    stack->list = d_list_append((stack->list), in);
    if (!(stack->list)) {
        return false;
    }
	return true;
}

bool pop(Stack* stack, int* out) {
    if (!stack) {
        return false;
    }
    DList* lastNode = d_list_last(stack->list);
    if (!lastNode) {
        printf("No data in list\n");
        return false;
    }

    *out = d_list_get_data(lastNode);
    stack->list = d_list_remove(lastNode);
	return true;
}

bool top(Stack* stack, int* out) {
    if (!stack) {
        false;
    }
	DList* list = d_list_nth_for(stack->list, 0);
	if (!list) {
		printf("No data in list\n");
		return false;
	}
	*out = d_list_get_data(list);
	return true;
}

int size(Stack* stack) {
    if (!stack) {
        return -1;
    }
	return d_list_length(stack->list);
}

bool empty(Stack* stack) {
    return ((stack->list) != NULL) ? true : false;
}

void stack_free(Stack* stack) {
     d_list_free(stack->list);
     free(stack);
     return;
}

Stack* stack_new() {
	Stack* stack = (Stack*) malloc(sizeof(Stack));
	stack->list = NULL; 
	return stack;
}
