#include "test.h"
#include "../include/avl_tree.h"
#include "../src/definitions.h"

void print_preord(avl_t *tree){
	void *preord = avl_preorder(tree);
	for (size_t i = 0; i < avl_size(tree); i++){
		void *tmp = void_offset(preord, i * sizeof(int));
		printf("%d-",* (int*) tmp);
	}
	printf("\n");
	free(preord);
}

void assert_preord(avl_t *tree, int exp[]){
	void *preord = avl_preorder(tree);
	for (size_t i = 0; i < avl_size(tree); i++){
		void *tmp = void_offset(preord, i * sizeof(int));
		assert(exp[i] == * (int*) tmp);
	}
	free(preord);
}

void join_test(void){
	avl_t *t1 = avl_init(sizeof(int), compare_int);
	avl_t *t2 = avl_init(sizeof(int), compare_int);

	for (int i = 0; i < 10; i++){
		avl_add(t1, &i);
	}

	for (int i = 10; i < 20; i++){
		avl_add(t2, &i);
	}

	avl_t *joint = avl_join(t1, t2);

	for (int i = 0; i < 20; i++){
		assert(avl_exists(joint, &i));
	}

	avl_free(t1, t2, joint);
}

void destructor_test(void){
	avl_t *avl = avl_init(sizeof(int*), compare_lesser);
	avl_set_destructor(avl, destroy_ptr);
	for (int i = 0; i < 1024; i++){
		int *ptr = malloc(sizeof(int));
		assert(avl_add(avl, &ptr) == GDS_SUCCESS);
	}
	avl_free(avl);
}

int main(void){
	test_start("avl_tree.c");

	int tmp = 0;

	avl_t *t = avl_init(sizeof(int), compare_int);
	assert(avl_size(t) == 0UL);
	assert(avl_isempty(t));
	assert(-1 == avl_height(t));
	assert(avl_get(t, &(int){12}, &tmp) == NULL);

	int nums[] = {10, 6, 15, 3, 9, 14, 20, 2, 4, 7, 12};

	assert(avl_add_array(t, nums, 7UL));
	int max;
	assert(20 == * (int*) avl_max(t, &max));
	int min;
	assert(3 == * (int*) avl_min(t, &min));

	int exp1 [] = {10, 6, 3, 9, 15, 14, 20};
	assert_preord(t, exp1);

	assert(!avl_isempty(t));

	assert(avl_add_array(t, &nums[7], 4UL));

	int exp2 [] = {10, 6, 3, 2, 4, 9, 7, 15, 14, 12, 20};
	assert_preord(t, exp2);

	assert(3 == avl_height(t));

	assert(avl_add(t, &(int){1}));
	assert(1 == * (int*) avl_min(t, &min));
	int exp3 [] = {10, 6, 3, 2, 1, 4, 9, 7, 15, 14, 12, 20};
	assert_preord(t, exp3);

	assert(4 == avl_height(t));

	assert(avl_remove(t, &(int){20}));
	int exp4 [] = {6, 3, 2, 1, 4, 10, 9, 7, 14, 12, 15};
	assert_preord(t, exp4);

	assert(avl_remove(t, &(int){4}));
	int exp5 [] = {6, 2, 1, 3, 10, 9, 7, 14, 12, 15};
	assert_preord(t, exp5);

	assert(avl_remove(t, &(int){10}));
	int exp6 [] = {6, 2, 1, 3, 9, 7, 14, 12, 15};
	assert_preord(t, exp6);

	assert(avl_remove(t, &(int){9}));
	int exp7 [] = {6, 2, 1, 3, 14, 7, 12, 15};
	assert_preord(t, exp7);

	assert(avl_remove(t, &(int){6}));
	int exp8 [] = {3, 2, 1, 14, 7, 12, 15};
	assert_preord(t, exp8);

	assert(avl_remove(t, &(int){3}));
	int exp9 [] = {7, 2, 1, 14, 12, 15};
	assert_preord(t, exp9);

	avl_clear(t);

	assert(avl_add_array(t, nums, 11UL));
	assert(avl_remove_array(t, nums, 11UL));
	assert(avl_isempty(t));

	avl_free(t);

	join_test();

	destructor_test();


	test_end("avl_tree.c");
	return 0;
}
