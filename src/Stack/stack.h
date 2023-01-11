/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 11-01-2023
 * Copyright (C) 2022-2023 Saúl Valdelvira
 * Licensed under the GNU GPL V.3. See /LICENSE file for more info
*/
#pragma once
#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include <stdbool.h>
#include "../Util/comparator.h"

// ERRORS
#ifndef ALLOCATION_ERROR
	#define ALLOCATION_ERROR -2
#endif

#ifndef NULL_PARAMETER
	#define NULL_PARAMETER -3
#endif

/**
 * The Stack works in a similar way to a LinkedList. But in this case, the elements are allways added in the head.
 * This is because a Stack is a Data Structure that follows a LIFO (Last In First Out) dynamic. The last element added 
 * is the first one that goes out.
*/
typedef struct _Stack Stack;

/**
 * Initializes an empty Stack.
 * @param data_size the size of the data being stored.
 * @param comp the comparator funtion for two elements
*/
Stack* stack_init(size_t data_size, comparator_function_t cmp);


/**
 * Pushes the given element to the top of the stack
 * @return 1 if the operation is successful
*/
int stack_push(Stack *stack, void *element);

/**
 * @return the last pushed element (and deletes it from the stack)
*/
void* stack_pop(Stack *stack, void *dest);

/**
 * @return the last pushed element (without deleting it)
*/
void* stack_peek(Stack *stack, void *dest);

/**
 * @return true if the element exists in the stack 
*/
bool stack_search(Stack *stack, void *element);

/**
 * @return true if the stack is empty
*/
bool stack_isempty(Stack *stack);

/**
 * Frees the memory allocated for the stack.
 * @return true 1 if he operation is successful
*/
int stack_free(Stack *stack);

/**
 * Frees the memory allocated for the stack AND resets it to the initial state of a stack
 * @return the same pointer if sucess, NULL if the operation fails
*/
Stack* stack_reset(Stack *stack);

#endif