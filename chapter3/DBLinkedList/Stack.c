#include <stdio.h>
#include <stdlib.h>
#include "Stack.h"
#include "DBLinkedList.h"

DList* push(DList* list, int data) {
	list = d_list_append(list, data);	
	return list;
}

DList* pop(DList* list) {
	DList* remove = empty(list);
	if (remove == NULL) {
		return list;
	}
	remove->prev->next = NULL;
	free(remove);
	return list;
}

DList* empty(DList* list) {
	list = d_list_last(list);
	return list;
}
