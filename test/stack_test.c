#include "test.h"
#include "../src/Stack/stack.h"

int main(){
        int n = 10000000;
        printf("[Starting Stack Test with a load of %d]\n...\n", n);
        double tmp = get_time(), total;
        Stack stack = stack_init(Comparators.integer);
        stack_configure(&stack, FREE_ON_DELETE);
        assert(stack_isempty(stack));
        stack_push(&stack, alloc_int(12));
        assert(!stack_isempty(stack));
        assert(* (int*) stack_peek(stack) == 12);
        assert(!stack_isempty(stack));
        assert(* (int*) stack_pop(&stack) == 12);
        assert(stack_isempty(stack));

        for(int i=0; i<n; i++){
                stack_push(&stack, alloc_int(i));
        }
        for(int i=n-1; i>=0; i--){
                assert(i == * (int*) stack_peek(stack));
                assert(i == * (int*) stack_pop(&stack));
        }
        assert(stack_isempty(stack));

        total = get_time() - tmp;

        printf("[Stack Test Finished in %.3f seconds]\n", total);
}