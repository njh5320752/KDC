#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Stack.h"
#include "DBLinkedList.h"

struct _Stack
{
	 DList* list;
};

bool push(Stack* stack, int* out) {
	stack->list = d_list_insert_first((stack->list), *out);
    if (!stack->list) {
        return false;
    }
	return true;
}

bool pop(Stack* stack, int* out) {
    DList* list = d_list_nth_for(stack->list, 0);
    if (!list) {
        printf("No data in list\n");
        return false;
    }
    *out = d_list_get_data(list);
	stack->list = d_list_remove_nth(stack->list, 0);
	return true;
}

bool top(Stack* stack, int* out) {
	DList* list = d_list_nth_for(stack->list, 0);
	if (!list) {
		printf("No data in list\n");
		return false;
	}
	*out = d_list_get_data(list);
	return true;
}

int size(Stack* stack) {
	return d_list_length(stack->list);
}

bool empty(Stack* stack) {
    return ((stack->list) != NULL) ? true : false;
}

void stack_free(Stack* stack) {
     d_list_free(stack->list);
}

Stack* stack_new() {
	Stack* stack = (Stack*) malloc(sizeof(Stack));
	stack->list = NULL; 
	return stack;
}
