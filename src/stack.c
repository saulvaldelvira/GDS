/*
 * stack.c - stack Implementation
 * Author: Saúl Valdelvira (2023)
 */
#include <stdlib.h>
#include "./vector.h"
#include "definitions.h"
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#define stack_t vector_t
#include "stack.h"

/// INITIALIZE ////////////////////////////////////////////////////////////////

stack_t* stack_init(size_t data_size, comparator_function_t cmp){
        assert(cmp && data_size > 0);
        return vector_init(data_size, cmp);
}

void stack_set_comparator(stack_t *stack, comparator_function_t cmp){
        vector_set_comparator(stack, cmp);
}

void stack_set_destructor(stack_t *stack, destructor_function_t destructor){
        vector_set_destructor(stack, destructor);
}

///////////////////////////////////////////////////////////////////////////////

/// PUSH-POP //////////////////////////////////////////////////////////////////////

int stack_push(stack_t *stack, void *element){
        assert(stack && element);
        return vector_append(stack, element);
}

int stack_push_array(stack_t *stack, void *array, size_t array_length){
        assert(stack && array);
        return vector_append_array(stack, array, array_length);
}

void* stack_pop(stack_t *stack, void *dest){
        assert(stack && dest);
        return vector_pop_back(stack, dest);
}

size_t stack_pop_array(stack_t *stack, void *array, size_t array_length){
        assert(stack && array);
        size_t data_size = vector_get_data_size(stack);
        for (size_t i = 0; i < array_length; i++){
                // When the stack is empty, return
                if (!vector_pop_back(stack, array))
                        return i;
                array = void_offset(array, data_size);
        }
        return array_length;
}

///////////////////////////////////////////////////////////////////////////////

/// PEEK //////////////////////////////////////////////////////////////////////

void* stack_peek(const stack_t *stack, void *dest){
        assert(stack && dest);
        return vector_back(stack, dest);
}

bool stack_exists(const stack_t *stack, void *element){
        assert(stack && element);
        return vector_exists(stack, element);
}

int stack_remove(stack_t *stack, void *element){
        assert(stack && element);
        return vector_remove(stack, element);
}

size_t stack_size(const stack_t *stack){
        return vector_size(stack);
}

bool stack_isempty(const stack_t *stack){
        return vector_isempty(stack);
}

///////////////////////////////////////////////////////////////////////////////

/// FREE //////////////////////////////////////////////////////////////////////

static void _stack_free(stack_t *stack){
        if (stack){
                vector_free(stack);
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
                vector_clear(stack);
}
