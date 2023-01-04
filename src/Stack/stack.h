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
 * 
 *  \attention
     * To make this structure "generic", it uses void pointers to the elements it stores.
     * When you delete an element, you can choose to also free the 
     * memory alocated for that element "free(void* element)". The behaviour is defined by this parameter.
     * 
     * @param free_on_delete If true, when deleting an element (stack_pop), it's memory is freed.
     * 
     *   \note
     * Sometimes you'll need the memory to be deleted, and sometimes that memory isn't allocated so you 
     * don't need to free those void pointers.
     * BE REALLY CAREFUL, because if you set those parameters to true and add elements to the list that don't need 
     * to be freed, you'll cause a segmentation fault. Also, it could cause trouble to add elements wich memory has been allocated at
     * the same time. 
     * \note .
     * \note ***********CODE EXAMPLE****************
     * \note int *ptr = malloc (3 * sizeof(int));
     * \note ptr[0] = 0;
     * \note ptr[1] = 1;
     * \note ptr[2] = 2;
     * 
     * \note stack_push(&stack, &ptr[0]);
     * \note stack_push(&list, &ptr[1]);
     * 
     * \note stack_pop(&list);
     * 
     * \note ****************************************
     * 
     * \note In this last instruction, if free_on_delete is 1, the stack will free &ptr[0]. But ptr holds memory for 3 elements so 
     * &ptr[1] and &ptr[2] will also be freed. This will cause trouble if the stack tries to access those already fred directions in the future.
     * 
     * \note To avoid this, you ca use the functions defined in "allocate.h", that allocate memory for the most common data types (alloc_int, alloc_char, alloc_float, etc)
     * 
     * \note 
     *  NOTE 2: The default value is 0 (don't free), so also keep in mind that unless you configure the list, that memory is not being 
     *  freed on pop.
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
extern void stack_configure(Stack *stack, int free_on_delete);

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