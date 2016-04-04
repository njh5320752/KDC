#ifndef __STACK_H__
#define __STACK_H__

typedef struct _Stack Stack;
bool push(Stack *stack, void *in);
void* pop(Stack *stack, void(*free_data)(void *data));
void* top(Stack *stack);
int size(Stack *stack);
bool empty(Stack *stack);
void stack_free(Stack *stack, void(*free_data)(void *data));
Stack* stack_new();
#endif
