#ifndef __STACK_H__
#define __STACK_H__

typedef struct _Stack Stack;
bool push(Stack* stack, int* out);
bool pop(Stack* stack, int* out);
bool empty(Stack* stack);
void stack_free(Stack* stack);
Stack* stack_new();
#endif
