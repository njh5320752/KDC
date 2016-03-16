#ifndef __DB_LINKED_LIST_H__
#define __DB_LINKED_LIST_H__

typedef struct _DList DList;

struct _DList
{
  int data;
  DList *next;
  DList *prev;
};

/* Doubly linked lists  */
DList* d_list_insert(DList *list, int data);
DList* d_list_last(DList *list);
int d_list_length(DList *list);
DList* d_list_remove(DList* list, int data);

#define d_list_next(list) ((list) ? (((DList *)(list))->next) : NULL)

void printAllData(DList *list);
DList* d_list_first(DList *list);

#endif
