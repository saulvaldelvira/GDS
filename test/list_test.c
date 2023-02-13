#include "../src/Vector/vector.h"
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
	Vector *a1 = vector_empty(sizeof(int), compare_int);
	Vector *a2 = vector_empty(sizeof(int), compare_int);
	LinkedList *l1 = lnkd_list_init(sizeof(int), compare_int);
	LinkedList *l2 = lnkd_list_init(sizeof(int), compare_int);

	for (int i = 0; i < 10; i++){
		vector_append(a1, &i);
		lnkd_list_append(l1, &i);
	}
	
	for (int i = 10; i < 20; i++){
		vector_append(a2, &i);
		lnkd_list_append(l2, &i);
	}

	Vector *arr_joint = vector_join(a1, a2);
	assert(arr_joint != NULL);
	LinkedList *lnkd_joint = lnkd_list_join(l1, l2);
	assert(lnkd_joint != NULL);
	for (int i = 0; i < 20; i++){
		assert(vector_exists(arr_joint, &i));
		assert(lnkd_list_exists(lnkd_joint, &i));
	}

	vector_free(a1);
	vector_free(a2);
	vector_free(arr_joint);
	lnkd_list_free(l1);
	lnkd_list_free(l2);
	lnkd_list_free(lnkd_joint);
}

int main(){
	int n = 2400;
	int tmp;
	double arr_time, lnkd_time;

	printf("[List Test]\n");
	
	printf("->Vector...\n");
	TIMESTAMP_START
	
	Vector *arr = vector_empty(sizeof(int), compare_int);

	assert(vector_isempty(arr));
	for(int i=0; i < n; i++){
		assert(vector_append(arr, &i));
		assert(vector_size(arr) == (size_t) i+1);
	}

	/// Configure test
	vector_configure(arr, compare_allways_true);
	assert(vector_exists(arr, cast_int(-8574654)));
	vector_configure(arr, compare_int);
	/////////

	assert(!vector_isempty(arr));
	assert(n-1 == * (int*) vector_get_back(arr, &tmp));
	assert(0 == * (int*) vector_get_front(arr, &tmp));
	int menosveinte = -20, treinta = 30;
	assert(vector_indexof(arr, &menosveinte).status == ELEMENT_NOT_FOUND_ERROR);

	index_t ret = vector_indexof(arr, &treinta);

	assert(ret.status);
	assert(ret.value == (size_t) treinta);

	int* get_arr = vector_get_array(arr, vector_size(arr));
	int* get_into = malloc(n * sizeof(int));
	assert(get_into != NULL);
	vector_get_into_array(arr, get_into, vector_size(arr));
	for (int i = 0; i < n; i++){
		assert(i == get_arr[i]);
		assert(i == get_into[i]);
	}


	// Push Pop 
	size_t size = vector_size(arr); 
	assert(vector_push_front(arr, cast_int(123)));
	assert(vector_append(arr, cast_int(456)));
	assert(123 == * (int*) vector_pop_front(arr, &tmp));
	assert(456 == * (int*) vector_pop_back(arr, &tmp));
	assert(size == vector_size(arr));

	/////////////////

	free(get_arr);
	free(get_into);

	for(int i=n-1; i >= 0; i--){
		assert(vector_exists(arr, &i));
		assert(vector_indexof(arr, &i).value == (size_t) i);
		assert(i == * (int*) vector_get_at(arr, i, &tmp));
		assert(i == * (int*) vector_get(arr, &i, &tmp));
		assert(vector_remove(arr, &i));
	}
	assert(vector_size(arr) == 0);

	


	// Set test 2
	int one = 1;
	int two = 2;
	int three = 3;

	assert(vector_append(arr, &one));
	assert(vector_set_at(arr, 0, &two));
	
	assert(two == * (int*) vector_get_at(arr, 0, &tmp));
	
	assert(vector_set(arr, &two, &three) != ELEMENT_NOT_FOUND_ERROR);
	assert(three == * (int*) vector_get_at(arr, 0, &tmp));
	/////////////////////////////////////////////////////
	arr = vector_reset(arr);
	int nums[] = {1, 2, 3, 4, 5};
	assert(vector_append_array(arr, nums, 5));
	assert(5UL == vector_size(arr));

	assert(vector_insert_at(arr, 1, cast_int(120)));
	assert(vector_exists(arr, cast_int(120)));
	assert(1UL == vector_indexof(arr, cast_int(120)).value);
	vector_remove_at(arr, 1);

	assert(vector_insert(arr, &nums[3], cast_int(-89)));
	assert(vector_exists(arr, cast_int(-89)));
	assert(3UL == vector_indexof(arr, cast_int(-89)).value);
	vector_remove_at(arr, 3);

	assert(vector_remove_array(arr, nums, 5));
	assert(vector_isempty(arr));

	vector_push_front(arr, cast_int(0));
	assert(vector_push_front_array(arr, nums, 5));
	assert(6UL == vector_size(arr));
	assert(0 == * (int*) vector_get_at(arr, 5, &tmp));
	vector_free(arr);

	TIMESTAMP_STOP
	arr_time = timestamp;

	printf("\tDone in %.3f seconds\n->Linked List...\n", arr_time);

	TIMESTAMP_START

	LinkedList *lnked = lnkd_list_init(sizeof(int), compare_int);
	
	assert(lnkd_list_isempty(lnked));
	
	for(int i=0; i < n; i++){
		assert(lnkd_list_append(lnked, &i));
		assert(lnkd_list_size(lnked) == (size_t) i+1);
	}
	assert(n-1 == * (int*) lnkd_list_get_back(lnked, &tmp));
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
	assert(-150 == * (int*) lnkd_list_get_front(lnked, &tmp));
	assert(-150 == * (int*) lnkd_list_pop_front(lnked, &tmp));
	assert(!lnkd_list_exists(lnked, &menos150));

	assert(lnkd_list_append(lnked, cast_int(4525)));
	assert(4525 == * (int*) lnkd_list_pop_back(lnked, &tmp));

	/// confign tests
	lnkd_list_configure(lnked, compare_allways_true);
	assert(lnkd_list_exists(lnked, cast_int(-684765)));
	lnkd_list_configure(lnked, compare_int);
	///////////////

	// Remove the last element
	assert(!lnkd_list_isempty(lnked));
	int last = n-1;
	assert(lnkd_list_remove(lnked, &last));
	assert(lnkd_list_size(lnked) == (size_t) n-1);
	assert(lnkd_list_append(lnked, &last));
	assert(lnkd_list_size(lnked) == (size_t) n);
	///////////////////////////////////////////////////
	
	for(int i=0; i < n; i++){
		assert(lnkd_list_exists(lnked, &i));
		assert(i == * (int *) lnkd_list_get(lnked, &i, &tmp));
		assert(lnkd_list_remove(lnked, &i));
	}
	assert(lnkd_list_size(lnked) == 0);

	lnked = lnkd_list_reset(lnked);
	assert(lnkd_list_append_array(lnked, nums, 5UL));
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
