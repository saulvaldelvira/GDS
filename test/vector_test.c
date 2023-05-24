#include "test.h"
#include "../src/Vector.h"

void joins_test(){
	Vector *a1 = vector_init(sizeof(int), compare_int);
	Vector *a2 = vector_init(sizeof(int), compare_int);
	Vector *diff_size = vector_init(sizeof(char), compare_char);

	for (int i = 0; i < 10; i++)
		vector_append(a1, &i);

	for (int i = 10; i < 20; i++)
		vector_append(a2, &i);

	Ignore_Error(assert(!vector_join(a1, diff_size)), -1);

	Vector *vec_joint = vector_join(a1, a2);
	assert(vec_joint != NULL);
	for (int i = 0; i < 20; i++)
		assert(vector_exists(vec_joint, &i));

	vector_free_all(4, a1, a2, diff_size, vec_joint);
}

void destructor_test(){
	Vector *vector = vector_init(sizeof(int*), compare_lesser);
	vector_set_destructor(vector, destroy_ptr);
	for (int i = 0; i < 1024; i++){
		int *ptr = malloc(sizeof(int));
		assert(vector_append(vector, &ptr) == SUCCESS);
	}
	vector_free(vector);
}

void reserve_shrink_test(){
	Vector *vector = vector_init(sizeof(int), compare_int);
	assert(vector_reserve(vector, 1024));
	assert(vector_capacity(vector) == 1024);
	vector_reset(vector);
	for (int i = 0; i < 100; i++)
		vector_append(vector, &i);
	assert(vector_capacity(vector) > 100);
	assert(vector_shrink(vector));
	assert(vector_capacity(vector) == 100);
	vector_free(vector);
}

int main(){
        int n = 2400;
	int tmp;
        print_test_start(Vector);
	TIMESTAMP_START
        Vector *vec = vector_init(sizeof(int), compare_int);

	assert(vector_isempty(vec));
	for(int i=0; i < n; i++){
		assert(vector_append(vec, &i));
		assert(vector_size(vec) == (size_t)i + 1UL);
	}

	/// Configure test
	vector_set_comparator(vec, compare_equal);
	assert(vector_exists(vec, &(int){-8574654}));
	vector_set_comparator(vec, compare_int);
	/////////

	assert(!vector_isempty(vec));
	assert(n-1 == * (int*) vector_get_back(vec, &tmp));
	assert(0 == * (int*) vector_get_front(vec, &tmp));
	assert(vector_indexof(vec, &(int){-20}).status == ELEMENT_NOT_FOUND_ERROR);

	index_t ret = vector_indexof(vec, &(int){30});

	assert(ret.status);
	assert(ret.value == (size_t) 30);

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
	assert(vector_push_front(vec, &(int){123}));
	assert(vector_append(vec, &(int){456}));
	assert(123 == * (int*) vector_get_front(vec, &tmp));
	assert(vector_remove_front(vec));
	assert(456 == * (int*) vector_get_back(vec, &tmp));
	assert(vector_remove_back(vec));
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
	assert(vector_append(vec, &(int){1}));
	assert(vector_set_at(vec, 0, &(int){2}));

	assert(2 == * (int*) vector_get_at(vec, 0, &tmp));

	assert(vector_set(vec, &(int){2}, &(int){3}) != ELEMENT_NOT_FOUND_ERROR);
	assert(3 == * (int*) vector_get_at(vec, 0, &tmp));
	/////////////////////////////////////////////////////
	vec = vector_reset(vec);
	int nums[] = {1, 2, 3, 4, 5};
	assert(vector_append_array(vec, nums, 5));
	assert(5UL == vector_size(vec));

	assert(vector_insert_at(vec, 1, &(int){120}));
	assert(vector_exists(vec, &(int){120}));
	assert(1UL == vector_indexof(vec, &(int){120}).value);
	vector_remove_at(vec, 1);

	assert(vector_insert(vec, &nums[3], &(int){-89}));
	assert(vector_exists(vec, &(int){-89}));
	assert(3UL == vector_indexof(vec, &(int){-89}).value);
	vector_remove_at(vec, 3);

	assert(vector_remove_array(vec, nums, 5));
	assert(vector_isempty(vec));

	vector_push_front(vec, &(int){0});
	assert(vector_push_front_array(vec, nums, 5));
	assert(6UL == vector_size(vec));
	assert(0 == * (int*) vector_get_at(vec, 5, &tmp));
	vector_free(vec);

	destructor_test();
	joins_test();
	reserve_shrink_test();

	TIMESTAMP_STOP
	print_test_end(Vector);
}