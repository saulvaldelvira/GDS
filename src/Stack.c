/*
 * Stack.c - Stack Implementation
 * Author: Saúl Valdelvira (2023)
 */
#include "Stack.h"
#include <stdlib.h>
#include <stdio.h>
#include "./Vector.h"
#include "./util/definitions.h"
#include <string.h>
#include <stdarg.h>
#include <assert.h>

struct Stack {
        Vector *elements; ///< Elements of the Stack
};

/// INITIALIZE ////////////////////////////////////////////////////////////////

Stack* stack_init(size_t data_size, comparator_function_t cmp){
        assert(cmp && data_size > 0);
        Stack *stack = malloc(sizeof(*stack));
        if (!stack) return NULL;
        stack->elements = vector_init(data_size,cmp);
        if (!stack->elements){
                free(stack);
                return NULL;
        }
        return stack;
}

void stack_set_comparator(Stack *stack, comparator_function_t cmp){
        vector_set_comparator(stack->elements, cmp);
}

void stack_set_destructor(Stack *stack, destructor_function_t destructor){
        vector_set_destructor(stack->elements, destructor);
}

///////////////////////////////////////////////////////////////////////////////

/// PUSH-POP //////////////////////////////////////////////////////////////////////

int stack_push(Stack *stack, void *element){
        assert(stack && element);
        return vector_append(stack->elements, element);
}

int stack_push_array(Stack *stack, void *array, size_t array_length){
        assert(stack && array);
        return vector_append_array(stack->elements, array, array_length);
}

void* stack_pop(Stack *stack, void *dest){
        assert(stack && dest);
        return vector_pop_back(stack->elements, dest);
}

size_t stack_pop_array(Stack *stack, void *array, size_t array_length){
        assert(stack && array);
        size_t data_size = vector_get_data_size(stack->elements);
        for (size_t i = 0; i < array_length; i++){
                // When the stack is empty, return
                if (!vector_pop_back(stack->elements, array))
                        return i;
                array = void_offset(array, data_size);
        }
        return array_length;
}

///////////////////////////////////////////////////////////////////////////////

/// PEEK //////////////////////////////////////////////////////////////////////

void* stack_peek(Stack *stack, void *dest){
        assert(stack && dest);
        return vector_back(stack->elements, dest);
}

bool stack_exists(Stack *stack, void *element){
        assert(stack && element);
        return vector_exists(stack->elements, element);
}

int stack_remove(Stack *stack, void *element){
        assert(stack && element);
        return vector_remove(stack->elements, element);
}

size_t stack_size(Stack *stack){
        return vector_size(stack->elements);
}

bool stack_isempty(Stack *stack){
        return vector_isempty(stack->elements);
}

///////////////////////////////////////////////////////////////////////////////

/// FREE //////////////////////////////////////////////////////////////////////

void stack_free(Stack *stack){
        if (stack){
                vector_free(stack->elements);
                free(stack);
        }
}

void stack_free_all(unsigned int n, ...){
        va_list arg;
        va_start(arg, n);
        for (unsigned int i = 0; i < n; i++){
                Stack *ptr = va_arg(arg, Stack*);
                stack_free(ptr);
        }
        va_end(arg);
}

void stack_clear(Stack *stack){
        if (stack)
                vector_clear(stack->elements);
}
