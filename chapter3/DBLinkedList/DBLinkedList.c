#include <stdio.h>
#include <stdlib.h>
#include "DBLinkedList.h"

struct _DList
{
    DList *next;
    DList *prev;
    void *data;
};

DList* d_list_append(DList *list, void *data) {
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

DList* d_list_insert_first(DList *list, void *data) {
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

DList* d_list_first(DList *list) {
	if (list) {
		while (list->prev) {
			list = list->prev;
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

DList* d_list_remove_nth_with_data(DList *list, void *data, void(*free_data)(void *data)) {
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
            free_data(tmp->data);
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

DList* d_list_insert(DList *list, void *data, int n) {
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

DList* d_list_nth_recursion(DList *list, int n) {
    if (n <= 0) {
        return list;
    }
    return d_list_nth_recursion(d_list_next(list), n-1);
}

DList* d_list_nth_for(DList *list, int n) {
    int i;
    for (i = 0; i < n && list != NULL; i++) {
        list = list->next;
    }
    return list;
}

void* d_list_get_data(DList *list) {
    if (!list) {
        printf("Can't get data");
        return 0;
    }
    return list->data;
}

DList* d_list_remove_nth(DList *list, int ni, void(*free_data)(void *data)) {
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
    free_data(remove->data);
    free(remove);
    return list;
}

DList* d_list_remove(DList *list, void(*free_data)(void *data)) {
    DList* remove;

    if (!list) {
        printf("No data\n");
        return list;
    }
    remove = list;
    if (list->prev) {
        list->prev->next = NULL;
    }

    if (list->next) {
        list->next->prev = NULL;
    }
	list = list->next;
    free_data(remove->data);
    free(remove);
	return list;
}

void d_list_free(DList *listi, void(*free_data)(void *data)) {
    DList* remove;
    while(list) {
        remove = list;
        list = list->next;
        free_data(remove->data)
        free(remove);
    }
    return;
}

int d_list_nth_with_data(DList *list, void *data) {
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

void d_list_sort(DList* list, int(*comp)(void *data1, void *data2)) {
    int i, j, tmp;
    bool is_switched = false;
    DList* node;
    if (!(list) || !(list->next)) {
        printf("Can't sort the list\n");
        return;
    }
    i = d_list_length(list);
    for (i = i -1; i > 0; i--) {
        node = list;
        for (j = 0; j < i; j++) {
            if (comp(node->data, node->next->data)) {
                switch_node(node, node->next)                
                is_switched = true;
            }
            node = node->next;
        }
        if (!is_switched) {
            break;
        } 
        is_switched = false;
    }
}

void d_list_insert_sort(DList* list, int(*comp)(void *data1, void *data2)) {
    int i, n, remember;
    DList* remember_node;
    DList* tmp;
    remember_node = list->next;
    n = d_list_length(list);

    for (i = 1; i < n; i++) {
        remember = remember_node->data;
        tmp = remember_node;
        printf("test\n");
        while (((tmp = tmp->prev) != NULL) && comp(tmp->data, remember)) {
                tmp->next->data = tmp->data;
                printf("data:%d\n", tmp->data);
        }
        if (!tmp) {
            tmp = list;
            tmp->data = remember;
        } else {
            tmp->next->data = remember;
        }
        remember_node = remember_node->next;
    }
}

DList* switch_node(DList* node1, DList* node2) {
    DList *tmp1 = node1->next;
    DList *tmp2 = node2->prev;

    node1->next = node2->next;
    if (node2->next) {
        node2->next->prev = node1;
    }

    node2->prev = node1->prev;

    if (node1->prev) {
        node1->prev->next = node2;
    }

    node1->prev = tmp2;
    node2->next = tmp1;
    tmp1->prev = node2;
    tmp2->next = node1;
}
