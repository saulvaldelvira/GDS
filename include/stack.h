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

/**
 * The stack_t works in a similar way to a linked_list. But in this case, the elements are allways added in the head.
 * This is because a stack_t is a Data Structure that follows a LIFO (Last In First Out) dynamic. The last element added
 * is the first one that goes out.
*/
typedef struct stack_t stack_t;

/**
 * Initializes an empty stack_t.
 * @param data_size the size of the data being stored.
 * @param comp the comparator funtion for two elements
*/
stack_t* stack_init(size_t data_size, comparator_function_t cmp);

/**
 * Sets the comparator function of the stack
 * @param cmp the new comparator function.
*/
void stack_set_comparator(stack_t *stack, comparator_function_t cmp);

/**
 * Sets the comparator function of the stack
 * @param destructor the new destructor function. NULL means no destructor.
*/
void stack_set_destructor(stack_t *stack, destructor_function_t destructor);

/**
 * Pushes the given element to the top of the stack
 * @return 1 if the operation is successful
*/
int stack_push(stack_t *stack, void *element);

/**
 * Pushes [array_length] elements from array
*/
int stack_push_array(stack_t *stack, void *array, size_t array_length);

/**
 * @return the last pushed element (and deletes it from the stack)
*/
void* stack_pop(stack_t *stack, void *dest);

/**
 * Pops [dest_length] elements into array_dest
 * Returns the number of copied elements elements.
*/
size_t stack_pop_array(stack_t *stack, void *array, size_t array_length);

/**
 * @return the last pushed element (without deleting it)
*/
void* stack_peek(const stack_t *stack, void *dest);

/**
 * @return true if the element exists in the stack
*/
bool stack_exists(const stack_t *stack, void *element);

/**
 * Removes the element from the stack.
 * @return 1 if the element is successfuly deleted
*/
int stack_remove(stack_t *stack, void *element);

/**
 * Removes all the elements from the stack
*/
void stack_clear(stack_t *stack);

/**
 * @return the number of elements in the stack_t
*/
size_t stack_size(const stack_t *stack);

/**
 * @return true if the stack is empty
*/
bool stack_isempty(const stack_t *stack);

void stack_free(stack_t *v, ...);

/**
 * Frees all the given stacks.
 */
#define stack_free(...) stack_free(__VA_ARGS__, 0L)

#ifdef __cplusplus
}
#endif

#endif
