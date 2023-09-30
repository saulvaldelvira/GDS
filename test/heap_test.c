#define QUIET_DISABLE
#include "test.h"
#undef QUIET_DISABLE

#include "../src/Heap.h"

void change_priority(void){
	print_test_step("Change Priority");
	Heap *min = heap_init(sizeof(int), compare_int);
	int elements[] = {12 ,14, 15, 20, 16, 17, 19, 24, 30};
	heap_add_array(min, elements, 9);

	assert(heap_change_priority(min, &(int){-78}, &(int){90}) != SUCCESS);
	assert(heap_change_priority(min, NULL, NULL) != SUCCESS);
	assert(heap_change_priority(min, &elements[0], NULL) != SUCCESS);

	// Change priority of 20 to 5
	assert(heap_change_priority(min, &elements[3], &(int){5}));
	int res[9];
	heap_get_into_array(min, res, 9);
	int expected[] = {5, 12 ,15, 14, 16, 17, 19, 24, 30};
	assert_array_int(res, expected, 9);

	// Change priority of 12 to 21
	assert(heap_change_priority(min, &res[1], &(int){21}));
	heap_get_into_array(min, res, 9);
	int expected2[] = {5, 14 ,15, 21, 16, 17, 19, 24, 30};
	assert_array_int(res, expected2, 9);
	heap_free(min);
	print_test_ok();
}

void filter_up(void){
	Heap *min = heap_init(sizeof(int), compare_int);
	heap_add(min, &(int){10});
	heap_add(min, &(int){9});
	heap_add(min, &(int){8});

	/// Config test
	heap_set_comparator(min, compare_equal);
	assert(heap_exists(min, &(int){-15454}));
	heap_set_comparator(min, compare_int);
	////////////

	int exp1[] = {8, 10, 9};
	int *res = heap_get_array(min, 3);
	assert_array_int(res, exp1, 3);
	free(res);

	heap_add(min, &(int){7});
	int exp2[] = {7, 8, 9, 10};
	res = heap_get_array(min, 4);
	assert_array_int(res, exp2, 4);
	free(res);

	heap_add(min, &(int){6});
	int exp3[] = {6, 7, 9, 10, 8};
	res = heap_get_array(min, 5);
	assert_array_int(res, exp3, 5);
	free(res);

	heap_add(min, &(int){5});
	int exp4[] = {5, 7, 6, 10, 8, 9};
	res = heap_get_array(min, 6);
	assert_array_int(res, exp4, 6);
	free(res);

	heap_add(min, &(int){4});
	int exp5[] = {4, 7, 5, 10, 8, 9, 6};
	res = heap_get_array(min, 7);
	assert_array_int(res, exp5, 7);
	free(res);

	heap_add(min, &(int){3});
	int exp6[] = {3, 4, 5, 7, 8, 9, 6, 10};
	res = heap_get_array(min, 8);
	assert_array_int(res, exp6, 8);
	free(res);

	heap_add(min, &(int){2});
	int exp7[] = {2, 3, 5, 4, 8, 9, 6, 10, 7};
	res = heap_get_array(min, 9);
	assert_array_int(res, exp7, 9);
	free(res);

	heap_add(min, &(int){1});
	int exp8[] = {1, 2, 5, 4, 3, 9, 6, 10, 7, 8};
	res = heap_get_array(min, 10);
	assert_array_int(res, exp8, 10);
	free(res);

	heap_free(min);

	// CHAR TEST
	min = heap_init(sizeof(char), compare_char);

	char input[] = {'f', 'g', 'a', 'z', 'd'};
	heap_add_array(min, input, 5);
	char expchar[] = {'a', 'd', 'f', 'z', 'g'};
	char* result = heap_get_array(min, 0);
	assert_array_char(result, expchar, 5);
	free(result);
	char dest;
	assert('a' == * (char*) heap_pop_min(min, &dest));
	char expchar2[] = {'d', 'g', 'f', 'z'};
	result = heap_get_array(min, 0);
	assert_array_char(result, expchar2, 4);
	free(result);

	heap_free(min);
}

void pop_min(void){
	Heap *min = heap_init(sizeof(int), compare_int);
	heap_add(min, &(int){200});
	heap_add(min, &(int){105});
	heap_add(min, &(int){1});

	int exp[] = {1, 105, 200};
	int index = 0;
	int tmp;
	while (!heap_isempty(min)){
		int value = * (int*) heap_pop_min(min, &tmp);
		assert(exp[index++] == value);
	}

	heap_free(min);

	min = heap_init(sizeof(char), compare_char);
	char input[] = {'Z', 'X', 'R', 'P', 'O', 'G', 'E', 'D', 'B', 'A'};
	heap_add_array(min, input, 10);
	char exp2[] = {'A','B','D','E','G','O','P','R','X','Z'};
	index = 0;
	char c;
	while (!heap_isempty(min)){
		char value = * (char*) heap_pop_min(min, &c);
		assert(exp2[index++] == value);
	}

	heap_free(min);
}

void destructor_test(void){
	Heap *heap = heap_init(sizeof(int*), compare_lesser);
	heap_set_destructor(heap, destroy_ptr);
	for (int i = 0; i < 1024; i++){
		int *ptr = malloc(sizeof(int));
		assert(heap_add(heap, &ptr) == SUCCESS);
	}
	heap_free(heap);
}

int main(void){
	print_test_start(Heap);
	TIMESTAMP_START
	int tmp;

	Heap *min = heap_init(sizeof(int), compare_int);
	assert(heap_isempty(min));
	assert(heap_pop_min(min, &tmp) == NULL);

	int nums[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
	heap_add_array(min, nums, 10);

	int *res = heap_get_array(min, 10);
	int exp1[] = {1, 2, 4, 3, 6, 5, 8, 10, 7, 9};
	assert_array_int(res, exp1, 10);
	free(res);

heap_clear(min);

	int nums2[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
	heap_add_array(min, nums2, 11);

	int exp2[] = {0, 1, 4, 2, 6, 5, 8, 3, 7, 9, 10};
	res = heap_get_array(min, 11);
	assert_array_int(res, exp2, 11);
	free(res);

	heap_free(min);

	filter_up();

	pop_min();

	change_priority();
	destructor_test();

	TIMESTAMP_STOP
	print_test_end(Heap);
	return 0;
}
