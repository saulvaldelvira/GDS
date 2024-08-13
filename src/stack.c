/*
 * stack.c - stack_t Implementation
 * Author: Saúl Valdelvira (2023)
 */
#include "stack.h"
#include <stdlib.h>
#include "./vector.h"
#include "definitions.h"
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include "gdsmalloc.h"

struct stack_t {
        vector_t *elements;       ///< Elements of the stack_t
};

/// INITIALIZE ////////////////////////////////////////////////////////////////

stack_t* stack_init(size_t data_size, comparator_function_t cmp){
        assert(cmp && data_size > 0);
        stack_t *stack = gdsmalloc(sizeof(*stack));
        if (!stack) return NULL;
        stack->elements = vector_init(data_size,cmp);
        if (!stack->elements){
                free(stack);
                return NULL;
        }
        return stack;
}

void stack_set_comparator(stack_t *stack, comparator_function_t cmp){
        vector_set_comparator(stack->elements, cmp);
}

void stack_set_destructor(stack_t *stack, destructor_function_t destructor){
        vector_set_destructor(stack->elements, destructor);
}

///////////////////////////////////////////////////////////////////////////////

/// PUSH-POP //////////////////////////////////////////////////////////////////////

int stack_push(stack_t *stack, void *element){
        assert(stack && element);
        return vector_append(stack->elements, element);
}

int stack_push_array(stack_t *stack, void *array, size_t array_length){
        assert(stack && array);
        return vector_append_array(stack->elements, array, array_length);
}

void* stack_pop(stack_t *stack, void *dest){
        assert(stack && dest);
        return vector_pop_back(stack->elements, dest);
}

size_t stack_pop_array(stack_t *stack, void *array, size_t array_length){
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

void* stack_peek(stack_t *stack, void *dest){
        assert(stack && dest);
        return vector_back(stack->elements, dest);
}

bool stack_exists(stack_t *stack, void *element){
        assert(stack && element);
        return vector_exists(stack->elements, element);
}

int stack_remove(stack_t *stack, void *element){
        assert(stack && element);
        return vector_remove(stack->elements, element);
}

size_t stack_size(stack_t *stack){
        return vector_size(stack->elements);
}

bool stack_isempty(stack_t *stack){
        return vector_isempty(stack->elements);
}

///////////////////////////////////////////////////////////////////////////////

/// FREE //////////////////////////////////////////////////////////////////////

static void _stack_free(stack_t *stack){
        if (stack){
                vector_free(stack->elements);
                free(stack);
        }
}

void (stack_free)(stack_t *s, ...){
        if (!s)
                return;
        va_list arg;
        va_start(arg, s);
        do {
                _stack_free(s);
                s = va_arg(arg, stack_t*);
        } while (s);
        va_end(arg);
}

void stack_clear(stack_t *stack){
        if (stack)
                vector_clear(stack->elements);
}
