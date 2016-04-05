#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Stack.h"
#include "DBLinkedList.h"

struct _Stack
{
	 DList* list;
};

bool push(Stack* stack, void *in) {
    if (!stack) {
        return false;
    }
    stack->list = d_list_append((stack->list), in);
    if (!(stack->list)) {
        return false;
    }
	return true;
}

void* pop(Stack* stack) {
    void* data;
    if (!stack) {
        return NULL;
    }
    DList* lastNode = d_list_last(stack->list);
    if (!lastNode) {
        printf("No data in list\n");
        return NULL;
    }

    data = d_list_get_data(lastNode);
    stack->list = d_list_delete(lastNode);
	return data;
}

void* top(Stack* stack) {
    void* data;
    if (!stack) {
        return NULL;
    }
	DList* list = d_list_last(stack->list);
	if (!list) {
		printf("No data in list\n");
		return NULL;
	}
	data = d_list_get_data(list);
	return data;
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

void stack_free(Stack* stack, void(*free_data)(void *data)) {
     d_list_free(stack->list, free_data);
     free(stack);
     return;
}

Stack* stack_new() {
	Stack* stack = (Stack*) malloc(sizeof(Stack));
	stack->list = NULL; 
	return stack;
}
