#include <stdio.h>
#include <time.h>
#include "DBLinkedList.h"
void test();
int main(void) {
    DList *list = NULL;
    DList *tmp;
    list = d_list_append(list, 1);
    list = d_list_append(list, 2);
    list = d_list_append(list, 3);
    list = d_list_append(list, 4);
    list = d_list_append(list, 5);
    printf("Length of list:%d\n", d_list_length(list));
    list = d_list_remove_nth_by_data(list, 3);
    printf("Length of list:%d\n", d_list_length(list));
    d_list_print_all_data(list);
    tmp = d_list_nth_recursion(list, 2);
    printf("recursion tmp index:2 data:%d\n", tmp->data);
    tmp = d_list_nth_for(list, 2);
    printf("for tmp index:2 data:%d\n", tmp->data);
    d_list_print_all_data(list);
    d_list_remove_nth(list, 2);
    d_list_print_all_data(list);
    d_list_insert(list, 8, 2);
    printf("index:%d\n", d_list_nth_data(list, 2));
    d_list_print_all_data(list);
    d_list_free(list);
    printf("Length of list:%d\n", d_list_length(list));
	test();
    return 0;
}

void test() {
	int max = 100000;
	float gap;
	time_t start, end;
	DList *list = NULL;
	int i;
	for (i = 0; i < max; i++) {
		list = d_list_append(list, i);
	}
 	printf("for 문 측정 시작\n");
	start = clock();
	d_list_nth_for(list, max-1);
	end = clock();
	printf("for 문 측정 끝\n");
	gap = (float)(end-start)/(CLOCKS_PER_SEC);
	printf("for 문 측정 시간: %f 초\n", gap); 

	printf("recursion 측정 시작\n");
	start = clock();
	d_list_nth_recursion(list, max-1);
	end = clock();
	printf("recursion 측정 끝\n");
	gap = (float)(end-start)/(CLOCKS_PER_SEC);
	printf("recursion 측정 시간: %f 초\n", gap); 
	return;
}
