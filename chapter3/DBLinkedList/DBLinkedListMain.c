#include <stdio.h>
#include "DBLinkedList.h"

int main(void) {
    DList *list = NULL;
    list = d_list_append(list, 1);
    list = d_list_append(list, 2);
    list = d_list_append(list, 3);
    list = d_list_append(list, 4);
    list = d_list_append(list, 5);
    printf("Length of list:%d\n", d_list_length(list));
    list = d_list_remove(list, 3);
    printf("Length of list:%d\n", d_list_length(list));
    printAllData(list);
    return 0;
}
