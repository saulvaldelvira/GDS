#include "../src/Stack/stack.h"

#define COMPARATOR_ENABLE
#define TIMESTAMP_ENABLE
#include "test.h"
#undef COMPARATOR_ENABLE
#undef TIMESTAMP_ENABLE

int main(){
        int n = 10000000;
        printf("[Starting Stack Test]\n Workload: %d\n", n);
        TIMESTAMP_START

        Stack stack = stack_init(Comparator.integer);
        stack_configure(&stack, FreeOnDelete);
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
        assert(!stack_isempty(stack));
        for(int i=n-1; i>=0; i--){
                assert(i == * (int*) stack_peek(stack));
                assert(i == * (int*) stack_pop(&stack));
        }
        assert(stack_isempty(stack));

        TIMESTAMP_STOP

        END_MSG(Stack)
}