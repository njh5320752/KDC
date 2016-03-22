#include <stdio.h>
#include <stdlib.h>
#include "Stack.h"
#include "DBLinkedList.h"

struct _Stack
{
	 DList* list;
};

void push(Stack* stack, int data) {
	stack->list = d_list_insert_first((stack->list), data);
	return;
}

int pop(Stack* stack) {
	DList* tmp = stack->list;
	stack->list = tmp->next;
	int data = d_list_removed_data(tmp);
	if ((data == -1)) {
		printf("There is no data\n");
		return 0;
	}
	return data;
}

int empty(Stack* stack) {
	int length = d_list_length(stack->list);
	return length;
}

void stack_free(Stack* stack) {
     d_list_free(stack->list);
}

Stack* stack_new() {
	Stack* stack = (Stack*) malloc(sizeof(Stack));
	stack->list = NULL; 
	return stack;
}
