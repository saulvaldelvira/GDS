/*
 * stack.h - stack_t definition.
 * Author: Saúl Valdelvira (2023)
 */
#pragma once
#ifndef STACK_H
#define STACK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include "compare.h"
#include "attrs.h"

#ifndef stack_t
typedef struct stack stack_t;
#endif

/**
 * Initializes an empty stack_t.
 * @param data_size the size of the data being stored.
 * @param comp the comparator funtion for two elements
*/
NONNULL()
stack_t* stack_init(size_t data_size, comparator_function_t cmp);

/**
 * Sets the comparator function of the stack
 * @param cmp the new comparator function.
*/
NONNULL()
void stack_set_comparator(stack_t *stack, comparator_function_t cmp);

/**
 * Sets the comparator function of the stack
 * @param destructor the new destructor function. NULL means no destructor.
*/
NONNULL(1)
void stack_set_destructor(stack_t *stack, destructor_function_t destructor);

/**
 * Pushes the given element to the top of the stack
 * @return 1 if the operation is successful
*/
NONNULL()
int stack_push(stack_t *stack, void *element);

/**
 * Pushes [array_length] elements from array
*/
NONNULL()
int stack_push_array(stack_t *stack, void *array, size_t array_length);

/**
 * @return the last pushed element (and deletes it from the stack)
*/
NONNULL()
void* stack_pop(stack_t *stack, void *dest);

/**
 * Pops [dest_length] elements into array_dest
 * Returns the number of copied elements elements.
*/
NONNULL()
size_t stack_pop_array(stack_t *stack, void *array, size_t array_length);

/**
 * @return the last pushed element (without deleting it)
*/
NONNULL()
void* stack_peek(const stack_t *stack, void *dest);

/**
 * @return true if the element exists in the stack
*/
NONNULL()
bool stack_exists(const stack_t *stack, void *element);

/**
 * Removes the element from the stack.
 * @return 1 if the element is successfuly deleted
*/
NONNULL()
int stack_remove(stack_t *stack, void *element);

/**
 * Removes all the elements from the stack
*/
NONNULL()
void stack_clear(stack_t *stack);

/**
 * @return the number of elements in the stack_t
*/
NONNULL()
size_t stack_size(const stack_t *stack);

/**
 * @return true if the stack is empty
*/
NONNULL()
bool stack_isempty(const stack_t *stack);

NONNULL()
void stack_free(stack_t *v, ...);

/**
 * Frees all the given stacks.
 */
#define stack_free(...) stack_free(__VA_ARGS__, 0L)

#ifdef __cplusplus
}
#endif

#endif
