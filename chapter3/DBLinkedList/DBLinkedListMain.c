#include <stdio.h>
#include "DBLinkedList.h"

int main(void) {
    DList *list = NULL;
	DList *tmp;
    list = d_list_append(list, 1);
    list = d_list_append(list, 2);
    list = d_list_append(list, 3);
    list = d_list_append(list, 4);
    list = d_list_append(list, 5);
    printf("Length of list:%d\n", d_list_length(list));
    list = d_list_remove(list, 3);
    printf("Length of list:%d\n", d_list_length(list));
    d_list_print_all_data(list);
	tmp = d_list_nth_while(list, 2);
	printf("while tmp datat:%d\n", tmp->data);
	tmp = d_list_nth_for(list, 2);
	printf("for tmp datat:%d\n", tmp->data);
	d_list_print_all_data(list);
	d_list_remove_index(list, 2);
	d_list_print_all_data(list);
	d_list_insert(list, 8, 2);
	printf("index:%d\n", d_list_index(list, 2));
	d_list_print_all_data(list);
	list = d_list_free(list);
 	printf("Length of list:%d\n", d_list_length(list));	
	return 0;
}
