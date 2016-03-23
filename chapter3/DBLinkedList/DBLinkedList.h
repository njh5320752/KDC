#ifndef __DB_LINKED_LIST_H__
#define __DB_LINKED_LIST_H__

typedef struct _DList DList;


/* Doubly linked lists  */
DList* d_list_append(DList *list, int data);
DList* d_list_last(DList *list);
int d_list_length(DList *list);
DList* d_list_remove_nth_by_data(DList* list, int data);
DList* d_list_insert(DList *list, int data, int n);
DList* d_list_nth_recursion(DList* list, int n);
DList* d_list_nth_for(DList* list, int n);
#define d_list_next(list) ((list) ? (((DList *)(list))->next) : NULL)
DList* d_list_remove_nth(DList* list, int index);
void d_list_print_all_data(DList* list);
void d_list_free(DList* list);
int d_list_nth(DList* list, int data);
int d_list_get_data(DList* list);
DList* d_list_insert_first(DList* list, int data);
#endif
