#include <stdio.h>
#include <time.h>
#include "DBLinkedList.h"

int set_sort_rule(int data1, int data2) {
    if (data1 > data2) {
        return 1;
    } else {
        return 0;
    }
}

void test();
int main(void) {
    DList *list = NULL;
    DList *tmp = NULL;
    DList *list2 = NULL;
    DList *list3 = NULL;
    list = d_list_append(list, 2);
    list = d_list_append(list, 3);
    list = d_list_append(list, 6);
    list = d_list_append(list, 1);
    list = d_list_append(list, 4);
    printf("Length of list:%d\n", d_list_length(list));
    list = d_list_remove_nth_with_data(list, 3);
    printf("Length of list:%d\n", d_list_length(list));
    printf("After sort\n");
    d_list_sort(list, set_sort_rule);
    d_list_print_all_data(list);
    tmp = d_list_nth_recursion(list, 2);
    printf("recursion tmp index:2 data:%d\n", d_list_get_data(tmp));
    tmp = d_list_nth_for(list, 2);
    printf("for tmp index:2 data:%d\n", d_list_get_data(tmp));
    d_list_print_all_data(list);
    d_list_remove_nth(list, 2);
    d_list_print_all_data(list);
    d_list_insert(list, 8, 2);
    printf("index:%d\n", d_list_nth_with_data(list, 2));
    d_list_print_all_data(list);
    d_list_free(list);
    list2 = d_list_append(list2, 3);
    list2 = d_list_append(list2, 2);
    list2 = d_list_append(list2, 1);
    list2 = d_list_append(list2, 5);
    list2 = d_list_append(list2, 7);
    d_list_print_all_data(list2); 
    printf("After sort\n");
    d_list_sort(list2, set_sort_rule);
    d_list_print_all_data(list2);
    printf("After insert sort\n");
    list3 = d_list_append(list3, 3);
    list3 = d_list_append(list3, 2);
    list3 = d_list_append(list3, 1);
    list3 = d_list_append(list3, 5);
    list3 = d_list_append(list3, 7);
    d_list_insert_sort(list3, set_sort_rule);
    d_list_print_all_data(list3);
	test();
    return 0;
}

void test() {
	int max = 100;
	float gap;
	time_t start, end;
	DList *list = NULL;
	int i;
	for (i = 0; i < max; i++) {
		list = d_list_append(list, i);
	}
 	printf("Start to find index in for loop\n");
	start = clock();
	d_list_nth_for(list, max-1);
	end = clock();
	printf("End to find index in for loop\n");
	gap = (float)(end-start)/(CLOCKS_PER_SEC);
	printf("Total time in for loop: %f sec\n", gap); 

	printf("Start to find index in recursion function\n");
	start = clock();
	d_list_nth_recursion(list, max-1);
	end = clock();
	printf("End to find index in recurtion function\n");
	gap = (float)(end-start)/(CLOCKS_PER_SEC);
	printf("Total time in recursion function: %f sec\n", gap); 
    d_list_free(list);
	return;
}
