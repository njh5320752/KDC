#include <stdio.h>
#include "Stack.h"
#include "DBLinkedList.h"

int main(void) {
	DList *list = NULL;
	list = pop(list);
	list = push(list, 3);
	list = push(list, 4);
	list = push(list, 5);
	list = pop(list);
	list = push(list, 6);
	list = push(list, 7);
	list = pop(list);
	d_list_print_all_data(list);
	return 0;
}
