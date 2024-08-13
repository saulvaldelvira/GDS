#include "../include/stack.h"
#include "test.h"

void destructor_test(void){
	stack_t *stack = stack_init(sizeof(int*), compare_lesser);
	stack_set_destructor(stack, destroy_ptr);
	for (int i = 0; i < 1024; i++){
		int *ptr = malloc(sizeof(int));
		assert(stack_push(stack, &ptr) == SUCCESS);
	}
	stack_free(stack);
}

int main(void){
	int n = 10000, tmp;
	test_start("stack.c");


	stack_t *stack = stack_init(sizeof(int), compare_int);
	assert(stack_isempty(stack));
	assert(stack_pop(stack, &tmp) == NULL);

	tmp = 12;
	stack_push(stack, &tmp);
	assert(!stack_isempty(stack));
	assert(* (int*) stack_peek(stack, &tmp) == 12);
	assert(!stack_isempty(stack));
	assert(* (int*) stack_pop(stack, &tmp) == 12);
	assert(stack_isempty(stack));

	for(int i=0; i<n; i++){
		assert(stack_push(stack, &i));
		assert(stack_size(stack) == (size_t) i + 1);
	}
	assert(!stack_isempty(stack));

	/// Config test
	stack_set_comparator(stack, compare_equal);
	assert(stack_exists(stack, &(int){-64546}));
	stack_set_comparator(stack, compare_char);
	////////////////

	for(int i=n-1; i>=0; i--){
		assert(i == * (int*) stack_peek(stack, &tmp));
		assert(i == * (int*) stack_pop(stack, &tmp));
		assert(stack_size(stack) == (size_t) i);
	}
	assert(stack_isempty(stack));

	stack_clear(stack);
	int nums[] = {1, 2, 3, 4, 5, 6, 7, 8};
	int pop[8];
	assert(stack_push_array(stack, nums, 8));
	assert(8UL == stack_size(stack));
	assert(stack_pop_array(stack, pop, 8));
	assert(stack_isempty(stack));

	stack_free(stack);

	destructor_test();


	test_end("stack.c");
}
