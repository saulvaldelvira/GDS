#include "../src/Stack/stack.h"

#define QUIET_DISABLE
#include "test.h"
#undef QUIET_DISABLE

int main(){
	int n = 10000, temp;
	printf("[Starting Stack Test]\n Workload: %d\n", n);
	TIMESTAMP_START

	Stack *stack = stack_init(sizeof(int), compare_int);
	assert(stack_isempty(stack));
	temp = 12;
	stack_push(stack, &temp);
	assert(!stack_isempty(stack));
	assert(* (int*) stack_peek(stack, &temp) == 12);
	assert(!stack_isempty(stack));
	assert(* (int*) stack_pop(stack, &temp) == 12);
	assert(stack_isempty(stack));

	for(int i=0; i<n; i++){
		assert(stack_push(stack, &i));
		assert(stack_size(stack) == (size_t) i + 1);
	}
	assert(!stack_isempty(stack));

	/// Config test
	stack_configure(stack, compare_allways_true);
	assert(stack_search(stack, cast_int(-64546)));
	stack_configure(stack, compare_char);
	////////////////

	for(int i=n-1; i>=0; i--){
		assert(i == * (int*) stack_peek(stack, &temp));
		assert(i == * (int*) stack_pop(stack, &temp));
		assert(stack_size(stack) == (size_t) i);
	}
	assert(stack_isempty(stack));

	stack = stack_reset(stack);
	int nums[] = {1, 2, 3, 4, 5, 6, 7, 8};
	int pop[8];
	assert(stack_push_array(stack, nums, 8));
	assert(8UL == stack_size(stack));
	assert(stack_pop_array(stack, pop, 8));
	assert(stack_isempty(stack));


	stack_free(stack);

	TIMESTAMP_STOP

	END_MSG(Stack)
}