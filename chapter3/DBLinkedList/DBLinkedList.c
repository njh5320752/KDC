#include <stdio.h>
#include <stdlib.h>
#include "DBLinkedList.h"

DList* d_list_append(DList *list, int data)
{
  DList *new_list;
  DList *last;
  
  new_list = (DList*) malloc(sizeof(DList));
  new_list->next = NULL;
  new_list->prev = NULL;
  new_list->data = data;
  
  if (list) {
    last = d_list_last(list);
    last->next = new_list;
    new_list->prev = last;
    return list;
  } else {
      return new_list;
  }
}

DList* d_list_prepend(DList* list, int data) {
	DList *new_list;
	new_list = (DList*) malloc(sizeof(DList));
	new_list->next = NULL;
	new_list->prev = NULL;
	new_list->data = data;
	
	if (list) {
		if (list->prev) {
			list->prev->next = new_list;
			new_list->prev = list->prev;
		}
		list->prev = new_list;
		new_list->next = list;
	}

	return new_list;
}

DList* d_list_last(DList *list)
{
  if (list) {
    while (list->next) {
		list = list->next;
	}
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
		if (tmp->data != data) {
			tmp = tmp->next;
		} else {
			if (tmp->prev) {
				tmp->prev->next = tmp->next;
			}
			if (tmp->next) {
				tmp->next->prev = tmp->prev;
			}
			if(list == tmp) {
				list = list->next;
			}
			free(tmp);
			break;
		}
	}
	return list;
}

void d_list_print_all_data(DList *list) {
    int index = 0;
    while (list) {
		printf("index:%d, data:%d\n", index, list->data);
		index++;
		list = list->next;
    }
}

DList* d_list_first(DList *list) {
    if(list) {
	while (list->prev)
	    list = list->prev;
    }
    return list;
}

DList* d_list_insert(DList *list, int data, int position) {
	DList *new_list;
	DList *tmp_list;

	if (position < 0 || position > (d_list_length(list) - 1)) {
		printf("Can't insert the data\n");
		return list;
	} else if (position == 0) {
		return d_list_prepend(list, data);
	}

	tmp_list = d_list_nth_while(list, position);
	if (!tmp_list) {
		return d_list_append(list, data);
	}

	new_list = (DList*) malloc(sizeof(DList));
	new_list->next = NULL;
	new_list->prev = NULL;
	new_list->data = data;

	if (tmp_list->prev) {
		tmp_list->prev->next = new_list;
		new_list->prev = tmp_list->prev;
	}
	new_list->next = tmp_list;
	tmp_list->prev = new_list;

	if (tmp_list == list) {
		return new_list;
	} else {
		return list;
	}
}

DList* d_list_nth_while(DList* list, int n) {
	while ((n-- > 0) && list) {
		list = list->next;
	}
	return list;
}

DList* d_list_nth_for(DList* list, int n) {
	int index;
	for (index = n; index > 0; index--) {
		list = list->next;
	}
	return list;
}

DList* d_list_remove_index(DList* list, int index) {
	DList *remove;
	remove = d_list_nth_for(list, index);
	printf("Remove index:%d data:%d\n", index, remove->data);
	if (remove->prev) {
		remove->prev->next = remove->next;
	}
	if (remove->next) {
		remove->next->prev = remove->prev;
	}
	if(list == remove) {
		list = list->next;
	}
	free(remove);
	return list;
}

DList* d_list_free(DList *list) {
	DList *tmp = list;
	DList* remove;
	while(tmp) {
		remove = tmp;
		tmp = tmp->next;
		if (tmp != NULL) { 
			remove->next->prev = NULL;
			remove->next = NULL;
		}
		free(remove);
	}
	return NULL;
}

int d_list_index(DList *list, int data) {
	int i = 0;
	while (list) {
		if (list->data == data) {
			return i;
		}
		i++;
		list = list->next;
	}
	return -1;
}
