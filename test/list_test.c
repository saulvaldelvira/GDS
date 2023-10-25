#include "../src/LinkedList.h"

#define QUIET_DISABLE
#include "test.h"
#undef QUIET_DISABLE

#include <stdint.h>

void joins_test(void){
	LinkedList *l1 = list_init(sizeof(int), compare_int);
	LinkedList *l2 = list_init(sizeof(int), compare_int);

	for (int i = 0; i < 10; i++)
		list_append(l1, &i);

	for (int i = 10; i < 20; i++)
		list_append(l2, &i);

	LinkedList *lnkd_joint = list_join(l1, l2);
	assert(lnkd_joint != NULL);
	for (int i = 0; i < 20; i++)
		assert(list_exists(lnkd_joint, &i));

	list_free_all(3, l1, l2, lnkd_joint);
}

void destructor_test(void){
	LinkedList *list = list_init(sizeof(int*), compare_pointer);
	list_set_destructor(list, destroy_ptr);
	int *ptr;
	for (int i = 0; i < 1024; i++){
		ptr = malloc(sizeof(int));
		assert(list_append(list, &ptr) == SUCCESS);
	}

	int *ptr2 = malloc(sizeof(int));
	list_set(list, &ptr, &ptr2);
	list_free(list);
}

int main(void){
	int n = 2400;
	int tmp;
	print_test_start(LinkedList);
	TIMESTAMP_START

	LinkedList *lnked = list_init(sizeof(int), compare_int);

	assert(list_isempty(lnked));
	assert(list_get_front(lnked, &tmp) == NULL);
	assert(list_get_back(lnked, &tmp) == NULL);

	for(int i=0; i < n; i++){
		assert(list_append(lnked, &i));
		assert(list_size(lnked) == (size_t) i+1);
	}
	assert(n-1 == * (int*) list_get_back(lnked, &tmp));
	int* get_arr = list_get_array(lnked, list_size(lnked));
	int* get_into = malloc(n * sizeof(int));
	assert(get_into != NULL);
	list_get_into_array(lnked, get_into, list_size(lnked));
	for (int i = 0; i < n; i++){
		assert(i == get_arr[i]);
		assert(i == get_into[i]);
	}


	free(get_arr);
	free(get_into);

	/*PUSH FRONT*/
	assert(list_push_front(lnked, &(int){-150}));
	assert(list_size(lnked) == 1UL + n);
	assert(-150 == * (int*) list_get_front(lnked, &tmp));
	assert(list_remove_front(lnked));
	assert(!list_exists(lnked, &(int){-150}));

	assert(list_append(lnked, &(int){4525}));
	assert(list_remove_back(lnked));

	/// confign tests
	list_set_comparator(lnked, compare_equal);
	assert(list_exists(lnked, &(int){-684765}));
	list_set_comparator(lnked, compare_int);
	///////////////

	// Remove the last element
	assert(!list_isempty(lnked));
	int last = n-1;
	assert(list_remove(lnked, &last));
	assert(list_size(lnked) == (size_t) n-1);
	assert(list_append(lnked, &last));
	assert(list_size(lnked) == (size_t) n);
	///////////////////////////////////////////////////

	for(int i=0; i < n; i++){
		assert(list_exists(lnked, &i));
		assert(i == * (int *) list_get(lnked, &i, &tmp));
		assert(list_remove(lnked, &i));
	}
	assert(list_size(lnked) == 0);

list_clear(lnked);

	int nums[] = {1, 2, 3, 4, 5};
	assert(list_append_array(lnked, nums, 5UL));
	assert(5UL == list_size(lnked));
	assert(list_remove_array(lnked, nums, 5UL));
	assert(list_isempty(lnked));
	assert(list_push_front_array(lnked, nums, 5UL));
	assert(5UL == list_size(lnked));

	list_free(lnked);

	joins_test();
	destructor_test();

	TIMESTAMP_STOP
	print_test_end(LinkedList);
	return 0;
}
