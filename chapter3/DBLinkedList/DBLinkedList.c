#include <stdio.h>
#include <stdlib.h>
#include "DBLinkedList.h"

struct _DList
{
    DList *next;
    DList *prev;
    int data;
};

DList* d_list_append(DList *list, int data) {
    DList *new_list;
    DList *last;
    new_list = (DList*) malloc(sizeof(DList));
	if (!new_list) {
		return new_list;
	}
    new_list->next = NULL;
    new_list->data = data;

    if (list) {
        last = d_list_last(list);
        last->next = new_list;
        new_list->prev = last;
        return list;
    } else {
        new_list->prev = NULL;
        return new_list;
    }
}

DList* d_list_insert_first(DList* list, int data) {
    DList *new_list;
    new_list = (DList*) malloc(sizeof(DList));
    if (!new_list) {
        return new_list;
    }
    new_list->prev = NULL;
    new_list->data = data;

    if (list) {
        list->prev = new_list;
        new_list->next = list;
    } else {
        new_list->next = NULL;
    }
    return new_list;
}

DList* d_list_last(DList *list) {
    if (list) {
        while (list->next) {
            list = list->next;
        }
    }
    return list;
}

int d_list_length(DList *list) {
    int lenght = 0;
    while (list) {
        lenght++;
        list = list->next;
    }
    return lenght;
}

DList* d_list_remove_nth_by_data(DList* list, int data) {
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

DList* d_list_insert(DList *list, int data, int n) {
    DList *new_list;
    DList *tmp_list;

    tmp_list = d_list_nth_for(list, n);
    if (tmp_list == NULL) {
        printf("Can't insert data");
        return list;
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

DList* d_list_nth_recursion(DList* list, int n) {
    if (n <= 0) {
        return list;
    }
    return d_list_nth_recursion(d_list_next(list), n-1);
}

DList* d_list_nth_for(DList* list, int n) {
    int i;
    for (i = 0; i < n && list != NULL; i++) {
        list = list->next;
    }
    return list;
}

int d_list_get_data(DList* list) {
    return list->data;
}

DList* d_list_remove_nth(DList* list, int n) {
    DList *remove;
    remove = d_list_nth_for(list, n);
    if (remove == NULL) {
        printf("Can't remove data");
        return list;
    }
    printf("Remove index:%d data:%d\n", n, remove->data);
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

void d_list_free(DList *list) {
    DList* remove;
    while(list) {
        remove = list;
        list = list->next;
        free(remove);
    }
    return;
}

int d_list_nth(DList *list, int data) {
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

void d_list_sort(DList* list, int(*comp)(int data, int data2)) {
	DList* first_node = d_list_nth_for(list, 0);
	DList* second_node = d_list_nth_for(list, 1);
	if (first_node == NULL || second_node == NULL) {
		printf("Can't sort the list\n");
		return;
	}

	int rule = comp(first_node->data, second_node->data);
	if (rule > 0) {
		//descending
		d_list_descending_sort(list);
	} else {
		//ascending
		d_list_ascending_sort(list);
	}
}

void d_list_descending_sort(DList* list) {
	int i = d_list_length(list);
	int j;
	int tmp;
	DList* node1;
	DList* node2;

	for (i = i -1; i > 0; i--) {
		for (j = 0; j < i; j++) {
			node1 = d_list_nth_for(list, j);
			node2 = d_list_nth_for(list, j + 1);
			if ((node2->data) > (node1->data)) {
				tmp = node2->data;
				node2->data = node1->data;
				node1->data = tmp;
			}
		}
	}

}

void d_list_ascending_sort(DList* list) {
	int i = d_list_length(list);
	int tmp;
	int j;
	DList* node1;
	DList* node2;

	for (i = i -1; i > 0; i--) {
		for (j = 0; j < i; j++) {
			node1 = d_list_nth_for(list, j);
			node2 = d_list_nth_for(list, j + 1);
			if ((node1->data) > (node2->data)) {
				tmp = node2->data;
				node2->data = node1->data;
				node1->data = tmp;
			}
		}
	}
}
