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
	LinkedList *l1 = list_init(sizeof(int), compare_int);
	LinkedList *l2 = list_init(sizeof(int), compare_int);

	for (int i = 0; i < 10; i++){
		vector_append(a1, &i);
		list_append(l1, &i);
	}

	for (int i = 10; i < 20; i++){
		vector_append(a2, &i);
		list_append(l2, &i);
	}

	Vector *vec_joint = vector_join(a1, a2);
	assert(vec_joint != NULL);
	LinkedList *lnkd_joint = list_join(l1, l2);
	assert(lnkd_joint != NULL);
	for (int i = 0; i < 20; i++){
		assert(vector_exists(vec_joint, &i));
		assert(list_exists(lnkd_joint, &i));
	}

	vector_free(a1);
	vector_free(a2);
	vector_free(vec_joint);
	list_free(l1);
	list_free(l2);
	list_free(lnkd_joint);
}

int main(){
	int n = 2400;
	int tmp;

	print_test_start(List);

	print_test_step(Vector)
	TIMESTAMP_START

	Vector *vec = vector_empty(sizeof(int), compare_int);

	assert(vector_isempty(vec));
	for(int i=0; i < n; i++){
		assert(vector_append(vec, &i));
		assert(vector_size(vec) == (size_t) i+1);
	}

	/// Configure test
	vector_configure(vec, compare_always_equal);
	assert(vector_exists(vec, cast_int(-8574654)));
	vector_configure(vec, compare_int);
	/////////

	assert(!vector_isempty(vec));
	assert(n-1 == * (int*) vector_get_back(vec, &tmp));
	assert(0 == * (int*) vector_get_front(vec, &tmp));
	int menosveinte = -20, treinta = 30;

	fprintf(stderr, "\n");
	assert(vector_indexof(vec, &menosveinte).status == ELEMENT_NOT_FOUND_ERROR);
	fprintf(stderr, Clear_Line);
	fprintf(stderr, Move_Line(13));
	fflush(stderr);


	index_t ret = vector_indexof(vec, &treinta);

	assert(ret.status);
	assert(ret.value == (size_t) treinta);

	int* get_arr = vector_get_array(vec, vector_size(vec));
	int* get_into = malloc(n * sizeof(int));
	assert(get_into != NULL);
	vector_get_into_array(vec, get_into, vector_size(vec));
	for (int i = 0; i < n; i++){
		assert(i == get_arr[i]);
		assert(i == get_into[i]);
	}

	// Push Pop
	size_t size = vector_size(vec);
	assert(vector_push_front(vec, cast_int(123)));
	assert(vector_append(vec, cast_int(456)));
	assert(123 == * (int*) vector_pop_front(vec, &tmp));
	assert(456 == * (int*) vector_pop_back(vec, &tmp));
	assert(size == vector_size(vec));

	/////////////////

	free(get_arr);
	free(get_into);

	for(int i=n-1; i >= 0; i--){
		assert(vector_exists(vec, &i));
		assert(vector_indexof(vec, &i).value == (size_t) i);
		assert(i == * (int*) vector_get_at(vec, i, &tmp));
		assert(i == * (int*) vector_get(vec, &i, &tmp));
		assert(vector_remove(vec, &i));
	}
	assert(vector_size(vec) == 0);

	// Set test 2
	int one = 1;
	int two = 2;
	int three = 3;

	assert(vector_append(vec, &one));
	assert(vector_set_at(vec, 0, &two));

	assert(two == * (int*) vector_get_at(vec, 0, &tmp));

	assert(vector_set(vec, &two, &three) != ELEMENT_NOT_FOUND_ERROR);
	assert(three == * (int*) vector_get_at(vec, 0, &tmp));
	/////////////////////////////////////////////////////
	vec = vector_reset(vec);
	int nums[] = {1, 2, 3, 4, 5};
	assert(vector_append_array(vec, nums, 5));
	assert(5UL == vector_size(vec));

	assert(vector_insert_at(vec, 1, cast_int(120)));
	assert(vector_exists(vec, cast_int(120)));
	assert(1UL == vector_indexof(vec, cast_int(120)).value);
	vector_remove_at(vec, 1);

	assert(vector_insert(vec, &nums[3], cast_int(-89)));
	assert(vector_exists(vec, cast_int(-89)));
	assert(3UL == vector_indexof(vec, cast_int(-89)).value);
	vector_remove_at(vec, 3);

	assert(vector_remove_array(vec, nums, 5));
	assert(vector_isempty(vec));

	vector_push_front(vec, cast_int(0));
	assert(vector_push_front_array(vec, nums, 5));
	assert(6UL == vector_size(vec));
	assert(0 == * (int*) vector_get_at(vec, 5, &tmp));
	vector_free(vec);

	print_test_ok();
	print_test_step(Linked List);

	LinkedList *lnked = list_init(sizeof(int), compare_int);

	assert(list_isempty(lnked));

	for(int i=0; i < n; i++){
		assert(list_append(lnked, &i));
		assert(list_size(lnked) == (size_t) i+1);
	}
	assert(n-1 == * (int*) list_get_back(lnked, &tmp));
	get_arr = list_get_array(lnked, list_size(lnked));
	get_into = malloc(n * sizeof(int));
	assert(get_into != NULL);
	list_get_into_array(lnked, get_into, list_size(lnked));
	for (int i = 0; i < n; i++){
		assert(i == get_arr[i]);
		assert(i == get_into[i]);
	}


	free(get_arr);
	free(get_into);

	/*PUSH FRONT*/
	int menos150 = -150;
	assert(list_push_front(lnked, &menos150));
	assert(list_size(lnked) == 1UL + n);
	assert(-150 == * (int*) list_get_front(lnked, &tmp));
	assert(-150 == * (int*) list_pop_front(lnked, &tmp));
	assert(!list_exists(lnked, &menos150));

	assert(list_append(lnked, cast_int(4525)));
	assert(4525 == * (int*) list_pop_back(lnked, &tmp));

	/// confign tests
	list_configure(lnked, compare_always_equal);
	assert(list_exists(lnked, cast_int(-684765)));
	list_configure(lnked, compare_int);
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

	lnked = list_reset(lnked);
	assert(list_append_array(lnked, nums, 5UL));
	assert(5UL == list_size(lnked));
	assert(list_remove_array(lnked, nums, 5UL));
	assert(list_isempty(lnked));
	assert(list_push_front_array(lnked, nums, 5UL));
	assert(5UL == list_size(lnked));

	list_free(lnked);

	joins_test();

	TIMESTAMP_STOP

	print_test_ok();
	print_test_end(List);
	return 0;
}
