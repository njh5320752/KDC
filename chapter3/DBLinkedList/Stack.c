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
    if (!stack) {
        return false;
    }
    stack->list = d_list_append((stack->list), *out);
    if (!(stack->list)) {
        return false;
    }
	return true;
}

void sort(Stack* stack, int(*comp)(int data, data2)) {
    stack->list = d_list_sort(stack->list, comp);
}

bool pop(Stack* stack, int* out) {
    int n;
    if (!stack) {
        return false;
    }
    DList* list = d_list_last(stack->list);
    if (!list) {
        printf("No data in list\n");
        return false;
    }
    *out = d_list_get_data(list);
    n = d_list_length(stack->list);
	stack->list = d_list_remove_nth(stack->list, n-1);
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
    stack->comp = NULL;
	return stack;
}
