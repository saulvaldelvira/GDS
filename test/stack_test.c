#include "../src/Stack/stack.h"

#define COMPARATOR_ENABLE
#define TIMESTAMP_ENABLE
#include "test.h"
#undef COMPARATOR_ENABLE
#undef TIMESTAMP_ENABLE

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
                stack_push(stack, &i);
        }
        assert(!stack_isempty(stack));
        for(int i=n-1; i>=0; i--){
                assert(i == * (int*) stack_peek(stack, &temp));
                assert(i == * (int*) stack_pop(stack, &temp));
        }
        assert(stack_isempty(stack));
        stack_free(stack);

        TIMESTAMP_STOP

        END_MSG(Stack)
}