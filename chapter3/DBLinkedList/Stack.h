#ifndef __STACK_H__
#define __STACK_H__

typedef struct _Stack Stack;
void push(Stack* stack, int data);
int pop(Stack* stack);
int empty(Stack* stack);
void stack_free(Stack* stack);
Stack* stack_new();
#endif
