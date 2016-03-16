#include <stdio.h>
#include <stdlib.h>
#include "DBLinkedList.h"

DList* d_list_append(DList *list, int data)
{
  DList *new_list;
  DList *last;
  
  new_list = (DList*) malloc(sizeof(DList));
  new_list->data = data;
  
  if (list) {
    last = d_list_last(list);
    last->next = new_list;
    new_list->prev = last;
    return list;
  } else
      return new_list;
}

DList* d_list_last(DList *list)
{
  if (list) {
    while (list->next)
      list = list->next;
  }
  return list;
}

int d_list_length(DList *list)
{
    int lenght;

    lenght = 0;
    while (list) {
	lenght++;
	list = list->next;
    }

    return lenght;
}

DList* d_list_remove(DList* list, int data) {
    DList *tmp;
    printf("Remove data:%d\n", data);
    tmp = list;
    while (tmp) {
	if (tmp->data != data)
	    tmp = tmp->next;
	else {
	    if (tmp->prev)
		tmp->prev->next = tmp->next;
	    if (tmp->next)
	       	tmp->next->prev = tmp->prev;
	    if(list == tmp)
		list = list->next;
	    
	    free(tmp);
	    break;
	}
    }
    return list;
}

void printAllData(DList *list) {
    DList* first = d_list_first(list);
    int index = 0;
    while(first) {
	index++;
	printf("index:%d, data:%d\n", index, first->data);
	first = first->next;
    }
}

DList* d_list_first(DList *list) {
    if(list) {
	while (list->prev)
	    list = list->prev;
    }
    return list;
}
