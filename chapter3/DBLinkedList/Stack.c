#include <stdio.h>
#include <stdlib.h>
#include "Stack.h"
#include "DBLinkedList.h"

struct _Stack
{
  DList* list;
};

void push(Stack* stack, int data) {
	d_list_insert_first((stack->list), data);
	return;
}

int pop(Stack* stack) {
	d_list_removed_data(stack->list);
	return data;
}

int empty(Stack* stack) {
	int length = d_list_length(list);
	return length;
}

int stack_free(Stack* stack) {
     d_list_free(stack->list);
}

Stack* stack_new() {
	Stack* stack = (Stack*) malloc(sizeof(Stack));
	stack->list = NULL; 
	return stack;
}
