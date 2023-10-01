/*
 * Stack.h - Stack definition.
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
#include "./util/compare.h"

/**
 * The Stack works in a similar way to a LinkedList. But in this case, the elements are allways added in the head.
 * This is because a Stack is a Data Structure that follows a LIFO (Last In First Out) dynamic. The last element added
 * is the first one that goes out.
*/
typedef struct Stack Stack;

/**
 * Initializes an empty Stack.
 * @param data_size the size of the data being stored.
 * @param comp the comparator funtion for two elements
*/
Stack* stack_init(size_t data_size, comparator_function_t cmp);

/**
 * Sets the comparator function of the stack
 * @param cmp the new comparator function.
*/
void stack_set_comparator(Stack *stack, comparator_function_t cmp);

/**
 * Sets the comparator function of the stack
 * @param destructor the new destructor function. NULL means no destructor.
*/
void stack_set_destructor(Stack *stack, destructor_function_t destructor);

/**
 * Pushes the given element to the top of the stack
 * @return 1 if the operation is successful
*/
int stack_push(Stack *stack, void *element);

/**
 * Pushes [array_length] elements from array
*/
int stack_push_array(Stack *stack, void *array, size_t array_length);

/**
 * @return the last pushed element (and deletes it from the stack)
*/
void* stack_pop(Stack *stack, void *dest);

/**
 * Pops [dest_length] elements into array_dest
 * Returns the number of copied elements elements.
*/
size_t stack_pop_array(Stack *stack, void *array, size_t array_length);

/**
 * @return the last pushed element (without deleting it)
*/
void* stack_peek(Stack *stack, void *dest);

/**
 * @return true if the element exists in the stack
*/
bool stack_exists(Stack *stack, void *element);

/**
 * Removes the element from the stack.
 * @return 1 if the element is successfuly deleted
*/
int stack_remove(Stack *stack, void *element);

/**
 * Removes all the elements from the stack
*/
void stack_clear(Stack *stack);

/**
 * @return the number of elements in the Stack
*/
size_t stack_size(Stack *stack);

/**
 * @return true if the stack is empty
*/
bool stack_isempty(Stack *stack);

/**
 * Frees the memory allocated for the stack.
*/
void stack_free(Stack *stack);

/**
 * Frees multiple stacks at once.
 * @param n number of pointers to free.
*/
void stack_free_all(unsigned int n, ...);

#ifdef __cplusplus
}
#endif

#endif
