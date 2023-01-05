/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 02/01/2023
*/
#pragma once
#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include <stdbool.h>

// ERRORS
#ifndef ALLOCATION_ERROR
    #define ALLOCATION_ERROR -2
#endif

#ifndef NULL_PARAMETER
    #define NULL_PARAMETER -3
#endif

// free_on_delete posible values
#ifndef FREE_ON_DELETE
    #define FREE_ON_DELETE 1
#endif

#ifndef DONT_FREE_ON_DELETE
    #define DONT_FREE_ON_DELETE 0
#endif

typedef struct StackNode {
        void* info;
        struct StackNode *next;
} StackNode;

/**
 * The Stack works in a similar way to a LinkedList. But in this case, the elements are allways added in the head.
 * This is because a Stack is a Data Structure that follows a LIFO (Last In First Out) dynamic. The last element added 
 * is the first one that goes out.
*/
typedef struct Stack {
        StackNode *head;
        // Comparator function
        int (*compare) (void*, void*);
        bool free_on_delete;
} Stack;

/**
 * Initializes an empty Stack
 * @param comp the comparator funtion for two elements
*/
extern Stack stack_init(int (*cmp) (void*, void*));

/**
 * Sets the value of stack->free_on_delete to the parameter free_on_delete
 * See the defined macros FREE_ON_DELETE (1) and DONT_FREE_ON_DELETE (0)
*/
static inline void stack_configure(Stack *stack, int free_on_delete){
    stack->free_on_delete = free_on_delete;
}

/**
 * Pushes the given element to the top of the stack
*/
extern int stack_push(Stack *stack, void *element);

/**
 * @return the last pushed element (and delete it from the stack)
*/
extern void* stack_pop(Stack *stack);

/**
 * @return the last pushed element (without deleting it)
*/
extern void* stack_peek(Stack stack);

/**
 * @return true if the element exists in the stack 
*/
extern bool stack_search(Stack stack, void *element);

/**
 * @return true if the stack is empty
*/
extern bool stack_isempty(Stack stack);

/**
 * Frees the memory allocated for the stack. 
 * @note The behviour of this routine towards the memory for the elements of the Stack (the void pointers) is defined by 
 * stack.free_on_delete. See the Stack struct documentation for more info.
*/
extern void stack_free(Stack stack);

extern void stack_reset(Stack *stack);

#endif