#include "test.h"
#include "../src/Vector.h"
#include <string.h>

void assert_index(Vector *v, int i, int val){
        int tmp;
        assert(vector_at(v, i, &tmp));
        assert(tmp == val);
}

void assert_noindex(Vector *v, int i){
        int tmp;
        assert(!vector_at(v, i, &tmp));
}

void joins_test(void){
	Vector *a1 = vector_init(sizeof(int), compare_int);
	Vector *a2 = vector_init(sizeof(int), compare_int);
	Vector *diff_size = vector_init(sizeof(char), compare_char);

	for (int i = 0; i < 10; i++)
		vector_append(a1, &i);

	for (int i = 10; i < 20; i++)
		vector_append(a2, &i);

	Vector *vec_joint = vector_join(a1, a2);
	assert(vec_joint != NULL);
	for (int i = 0; i < 20; i++)
		assert(vector_exists(vec_joint, &i));

	vector_free(a1, a2, diff_size, vec_joint);
}

void dup_test(void){
	Vector *v = vector_init(sizeof(int), compare_int);

	for (int i = 0; i < 10; i++)
		vector_append(v, &i);

	Vector *dup = vector_dup(v);
	for (int i = 0; i < 10; i++)
		assert(vector_exists(dup, &i));

	vector_free(v, dup);
}

void destructor_test(void){
	Vector *vector = vector_init(sizeof(int*), compare_lesser);
	vector_set_destructor(vector, destroy_ptr);
	for (int i = 0; i < 1024; i++){
		int *ptr = malloc(sizeof(int));
		assert(vector_append(vector, &ptr) == SUCCESS);
	}

	/* Replacing an element is like removing it
	   and inserting another in it's position. */
	int *ptr = malloc(sizeof(int));
	assert(vector_set_at(vector,12,&ptr));

	vector_free(vector);
}

void reserve_shrink_test(void){
	Vector *vector = vector_init(sizeof(int), compare_int);
	assert(vector_reserve(vector, 1024));
	assert(vector_capacity(vector) == 1024);
	vector_reset(vector);
	for (int i = 0; i < 100; i++)
		vector_append(vector, &i);
	assert(vector_capacity(vector) > 100);
	vector_shrink(vector);
	assert(vector_capacity(vector) == 100);
	vector_free(vector);
}

void clear_test(void){
	Vector *vector = vector_init(sizeof(int), compare_int);
	for (int i = 0; i < 100; i++)
		vector_append(vector, &i);
	assert(vector_capacity(vector) > 100);
	assert(vector_size(vector) == 100);
	vector_clear(vector);
	assert(vector_capacity(vector) > 100);
	assert(vector_size(vector) == 0);
	vector_free(vector);

	// Make sure the destructor is called
	vector = vector_init(sizeof(void*), compare_equal);
	vector_set_destructor(vector, destroy_ptr);
	for (int i = 0; i < 100; i++){
		void *ptr = malloc(1024);
		vector_append(vector, &ptr);
	}
	vector_clear(vector); // Should free the memory, else valgrind will complain
	vector_free(vector);
}

static void multiply_by(void *e, void *arg){
	int *n = (int*) e;
	int mult = * (int*) arg;
	*n *= mult;
}

void map_test(void){
	test_step("Map");
	Vector *vector = vector_init(sizeof(int), compare_int);

	int arr[] = {1,2,3,4,5};

	vector_append_array(vector, arr, 5);
	int n = 2;
	vector_map(vector, multiply_by, &n);

	int tmp;
	for (int i = 0; i < 5; i++){
		vector_at(vector, i, &tmp);
		assert(tmp == n * arr[i]);
	}
	vector_free(vector);
	test_ok();
}

static bool filter_even(void *e){
	int n = * (int*) e ;
	return n % 2 == 0;
}

void filter_test(void){
	test_step("Filter");
	Vector *vector = vector_init(sizeof(int), compare_int);

	int arr[] = {1,2,3,4,5,6,7};

	vector_append_array(vector, arr, 7);
	Vector *filtered = vector_filter(vector, filter_even);


	size_t size = vector_size(filtered);
	assert(size == 3);

	int tmp;
	for (size_t i = 0; i < size; i++){
		vector_at(filtered, i, &tmp);
		assert(tmp % 2 == 0);
	}

	vector_free(vector, filtered);
	test_ok();
}

void addition(const void* e, void *acc){
	int *iacc = (int*) acc;
	int n = * (int*) e;
	*iacc += n;
}

void reduce_test(void){
	test_step("Reduce");
	Vector *vector = vector_init(sizeof(int), compare_int);

	int arr[] = {1,2,3,4,5};

	vector_append_array(vector, arr, 5);
	int n = 0;
	vector_reduce(vector, addition, &n);

	assert(n == 15);

	vector_free(vector);
	test_ok();
}

void sort_test(void){
	const int n = 1024;
	Vector *vector = vector_init(sizeof(int), compare_int);

	for (int i = 0; i < n; i++)
		vector_append(vector, &(int){rand_range(0, n)});

	vector_sort(vector);

	int prev;
	vector_front(vector, &prev);
	for (int i = 1; i < n; i++){
		int curr;
		vector_at(vector, i, &curr);
		assert(prev <= curr);
	}

	vector_free(vector);
}

void string_test(void){
	Vector *vector = vector_init(sizeof(char*), compare_string);

        char* arr[] = {
                "[1] Hello world!",
                "[2] This is a string array!",
                "[3] :-)",
                NULL
        };

        for (char **str = arr; *str; str++){
                vector_append(vector, str);
        }

        for (char **str = arr; *str; str++){
                assert(vector_exists(vector, str));
                char *tmp;
                vector_pop_front(vector, &tmp);
                assert(strcmp(tmp,*str) == 0);
        }

        vector_free(vector);
}

void index_test(void) {
	Vector *vector = vector_init(sizeof(int), compare_int);

        vector_append_array(vector, &(int[]){1,2,3,4,5}, 5);

        assert_index(vector, 0, 1);
        assert_index(vector, 2, 3);
        assert_index(vector, 4, 5);

        assert_index(vector, -1, 5);
        assert_index(vector, -2, 4);
        assert_index(vector, -5, 1);

        assert_noindex(vector, -6);
        assert_noindex(vector, -10);
        assert_noindex(vector, 5);
        assert_noindex(vector, 10);

        vector_free(vector);
}

int main(void){
        int n = 2400;
	int tmp;
        test_start("Vector.c");

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
	assert(n-1 == * (int*) vector_back(vec, &tmp));
	assert(0 == * (int*) vector_front(vec, &tmp));
	assert(vector_indexof(vec, &(int){-20}) == ELEMENT_NOT_FOUND_ERROR);

	assert(vector_indexof(vec, &(int){30}) == (size_t) 30);

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
	assert(123 == * (int*) vector_front(vec, &tmp));
	assert(vector_remove_front(vec));
	assert(456 == * (int*) vector_back(vec, &tmp));
	assert(vector_remove_back(vec));
	assert(size == vector_size(vec));

	/////////////////

	free(get_arr);
	free(get_into);

	for(int i=n-1; i >= 0; i--){
		assert(vector_exists(vec, &i));
		assert(vector_indexof(vec, &i) == (ptrdiff_t) i);
		assert(i == * (int*) vector_at(vec, i, &tmp));
		assert(i == * (int*) vector_get(vec, &i, &tmp));
		assert(vector_remove(vec, &i));
	}
	assert(vector_size(vec) == 0);

	// Set test 2
	assert(vector_append(vec, &(int){1}));
	assert(vector_set_at(vec, 0, &(int){2}));

	assert(2 == * (int*) vector_at(vec, 0, &tmp));

	assert(vector_set(vec, &(int){2}, &(int){3}) != ELEMENT_NOT_FOUND_ERROR);
	assert(3 == * (int*) vector_at(vec, 0, &tmp));
	/////////////////////////////////////////////////////
        vector_clear(vec);
	int nums[] = {1, 2, 3, 4, 5};
	assert(vector_append_array(vec, nums, 5));
	assert(5UL == vector_size(vec));

	// Negative indexing
	assert(5 == * (int*) vector_at(vec, -1, &tmp));
	assert(SUCCESS == vector_set_at(vec, -2, &(int){-123}));
	assert(-123 == * (int*) vector_at(vec, -2, &tmp));
	assert(SUCCESS == vector_set_at(vec, -2, &(int){4}));
	assert(SUCCESS != vector_set_at(vec, -50, &(int){-50}));
	assert(NULL == vector_at(vec, -12, &tmp));

	assert(vector_insert_at(vec, 1, &(int){120}));
	assert(vector_exists(vec, &(int){120}));
	assert(1UL == vector_indexof(vec, &(int){120}));
	vector_remove_at(vec, 1);

	assert(vector_insert(vec, &nums[3], &(int){-89}));
	assert(vector_exists(vec, &(int){-89}));
	assert(3UL == vector_indexof(vec, &(int){-89}));
	vector_remove_at(vec, 3);

	assert(vector_remove_array(vec, nums, 5));
	assert(vector_isempty(vec));

	vector_push_front(vec, &(int){0});
	assert(vector_push_front_array(vec, nums, 5));
	assert(6UL == vector_size(vec));
	assert(0 == * (int*) vector_at(vec, 5, &tmp));
	vector_free(vec);

	destructor_test();
	joins_test();
	dup_test();
	reserve_shrink_test();
	clear_test();
	map_test();
	filter_test();
	reduce_test();
	sort_test();
        string_test();
        index_test();

	test_end("Vector.c");
}
