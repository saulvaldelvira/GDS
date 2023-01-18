#include "../src/DynamicArray/dynamic_array.h"
#include "../src/LinkedList/linked_list.h"

#define QUIET_DISABLE
#include "test.h"
#undef QUIET_DISABLE

#include <stdint.h>

#include <memory.h>
struct test {
	int a;
	int b;
	char ch[];
};

void joins_test(){
	DynamicArray *a1 = din_arr_empty(sizeof(int), compare_int);
	DynamicArray *a2 = din_arr_empty(sizeof(int), compare_int);
	LinkedList *l1 = lnkd_list_init(sizeof(int), compare_int);
	LinkedList *l2 = lnkd_list_init(sizeof(int), compare_int);

	for (int i = 0; i < 10; i++){
		din_arr_push_back(a1, &i);
		lnkd_list_push_back(l1, &i);
	}
	
	for (int i = 10; i < 20; i++){
		din_arr_push_back(a2, &i);
		lnkd_list_push_back(l2, &i);
	}

	DynamicArray *arr_joint = din_arr_join(a1, a2);
	assert(arr_joint != NULL);
	LinkedList *lnkd_joint = lnkd_list_join(l1, l2);
	assert(lnkd_joint != NULL);
	for (int i = 0; i < 20; i++){
		assert(din_arr_exists(arr_joint, &i));
		assert(lnkd_list_exists(lnkd_joint, &i));
	}

	din_arr_free(a1);
	din_arr_free(a2);
	din_arr_free(arr_joint);
	lnkd_list_free(l1);
	lnkd_list_free(l2);
	lnkd_list_free(lnkd_joint);
}

int main(){
	int n = 2400;
	double arr_time, lnkd_time;

	printf("[List Test]\n");
	
	printf("->DynamicArray...\n");
	TIMESTAMP_START
	
	DynamicArray *arr = din_arr_empty(sizeof(int), compare_int);

	assert(din_arr_isempty(arr));
	for(int i=0; i < n; i++){
		assert(din_arr_push_back(arr, &i));
		assert(din_arr_size(arr) == (size_t) i+1);
	}
	assert(!din_arr_isempty(arr));
	int menosveinte = -20, treinta = 30;
	assert(din_arr_indexof(arr, &menosveinte).status == ELEMENT_NOT_FOUND_ERROR);

	index_t ret = din_arr_indexof(arr, &treinta);

	assert(ret.status);
	assert(ret.value == (size_t) treinta);

	int* get_arr = din_arr_get_array(arr, din_arr_size(arr));
	int* get_into = malloc(n * sizeof(int));
	assert(get_into != NULL);
	din_arr_get_into_array(arr, get_into, din_arr_size(arr));
	for (int i = 0; i < n; i++){
		assert(i == get_arr[i]);
		assert(i == get_into[i]);
	}

	free(get_arr);
	free(get_into);

	int tmp;
	for(int i=n-1; i >= 0; i--){
		assert(din_arr_exists(arr, &i));
		assert(din_arr_indexof(arr, &i).value == (size_t) i);
		assert(i == * (int*) din_arr_get_at(arr, i, &tmp));
		assert(i == * (int*) din_arr_get(arr, &i, &tmp));
		assert(din_arr_remove(arr, &i));
	}
	assert(din_arr_size(arr) == 0);

	// Set test 2
	int one = 1;
	int two = 2;
	int three = 3;

	assert(din_arr_push_back(arr, &one));
	assert(din_arr_set_at(arr, 0, &two));
	
	assert(two == * (int*) din_arr_get_at(arr, 0, &tmp));
	
	assert(din_arr_set(arr, &two, &three) != ELEMENT_NOT_FOUND_ERROR);
	assert(three == * (int*) din_arr_get_at(arr, 0, &tmp));
	/////////////////////////////////////////////////////
	arr = din_arr_reset(arr);
	int nums[] = {1, 2, 3, 4, 5};
	assert(din_arr_push_back_array(arr, nums, 5));
	assert(5UL == din_arr_size(arr));
	assert(din_arr_remove_array(arr, nums, 5));
	assert(din_arr_isempty(arr));

	din_arr_push_front(arr, cast_int(0));
	assert(din_arr_push_front_array(arr, nums, 5));
	assert(6UL == din_arr_size(arr));
	assert(0 == * (int*) din_arr_get_at(arr, 5, &tmp));
	din_arr_free(arr);

	TIMESTAMP_STOP
	arr_time = timestamp;

	printf("\tDone in %.3f seconds\n->Linked List...\n", arr_time);

	TIMESTAMP_START

	LinkedList *lnked = lnkd_list_init(sizeof(int), compare_int);
	
	assert(lnkd_list_isempty(lnked));
	
	for(int i=0; i < n; i++){
		assert(lnkd_list_push_back(lnked, &i));
		assert(lnkd_list_size(lnked) == (size_t) i+1);
	}

	get_arr = lnkd_list_get_array(lnked, lnkd_list_size(lnked));
	get_into = malloc(n * sizeof(int));
	assert(get_into != NULL);
	lnkd_list_get_into_array(lnked, get_into, lnkd_list_size(lnked));
	for (int i = 0; i < n; i++){
		assert(i == get_arr[i]);
		assert(i == get_into[i]);
	}


	free(get_arr);
	free(get_into);

	/*PUSH FRONT*/
	int menos150 = -150;
	assert(lnkd_list_push_front(lnked, &menos150));
	assert(lnkd_list_size(lnked) == 1UL + n);
	assert(lnkd_list_remove(lnked, &menos150));

	// Remove the last element
	assert(!lnkd_list_isempty(lnked));
	int last = n-1;
	assert(lnkd_list_remove(lnked, &last));
	assert(lnkd_list_size(lnked) == (size_t) n-1);
	assert(lnkd_list_push_back(lnked, &last));
	///////////////////////////////////////////////////
	
	for(int i=0; i < n; i++){
		assert(lnkd_list_exists(lnked, &i));
		assert(i == * (int *) lnkd_list_get(lnked, &i, &tmp));
		assert(lnkd_list_remove(lnked, &i));
	}
	assert(lnkd_list_size(lnked) == 0);

	lnked = lnkd_list_reset(lnked);
	assert(lnkd_list_push_back_array(lnked, nums, 5UL));
	assert(5UL == lnkd_list_size(lnked));
	assert(lnkd_list_remove_array(lnked, nums, 5UL));
	assert(lnkd_list_isempty(lnked));
	assert(lnkd_list_push_front_array(lnked, nums, 5UL));
	assert(5UL == lnkd_list_size(lnked));

	lnkd_list_free(lnked);

	joins_test();

	TIMESTAMP_STOP
	lnkd_time = timestamp;

	printf("\tDone in %.3f seconds\n", lnkd_time);

	double total = lnkd_time + arr_time;

	timestamp += total;


	END_MSG(List)
	return 0;
}
