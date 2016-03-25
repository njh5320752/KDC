#ifndef __STACK_H__
#define __STACK_H__

typedef struct _Stack Stack;
bool push(Stack* stack, int* out);
bool pop(Stack* stack, int* out);
bool top(Stack* stack, int* out);
int size(Stack* stack);
bool empty(Stack* stack);
void stack_free(Stack* stack);
void sort(Stack* stack, int(*comp)(int data, data2));
Stack* stack_new();
#endif
