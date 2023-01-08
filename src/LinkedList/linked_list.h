/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 08-01-2023
 * Copyright: Saúl Valdelvira Iglesias (2023)
 * Licensed under the GNU GPL V.3. See /LICENSE file for more info
*/
#pragma once
#ifndef LINKED_LINKED_LIST_H
#define LINKED_LINKED_LIST_H

#include <stddef.h>
#include <stdbool.h>
#include "../definitions.h"

/**
 * Node of a Linked List
*/
typedef struct LLNode LLNode;

/**
 * @brief Linked List structure.
 * 
 * \attention
 * To make this structure "generic", nodes use void pointers to the elements they store.
 * When you delete a node, it is freed from memory. You can choose to also free the 
 * memory alocated for the element the node stores. The behaviour is defined by this parameter.
 * 
 * @param free_on_delete If true, when deleting a node, the element it stores will also be freed.
 * 
 * \note
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
 * \note lnkd_list_append(&list, &ptr[0]);
 * \note lnkd_list_append(&list, &ptr[1]);
 * 
 * \note lnkd_list_remove(&list, &ptr[0]);
 * 
 * \note ****************************************
 * 
 * \note In this last instruction, if free_on_delete is 1, the list will free &ptr[0]. But ptr holds memory for 3 elements so 
 * &ptr[1] and &ptr[2] will also be freed. This will cause trouble if the list tries to access those already fred directions in the future.
 * 
 * \note To avoid this, you ca use the functions defined in "allocate.h", that allocate memory for the most common data types (alloc_int, alloc_char, alloc_float, etc)
 * 
 * \note 
 *  NOTE 2: The default value is 0 (don't free), so also keep in mind that unless you configure the list, that memory is not being 
 *  freed on deletion.
 * */
typedef struct LinkedList {
    LLNode *head;
    LLNode *tail;
    size_t n_elements;
    /**
     * Comparator function for 2 elements
     *   \attention 
     * This function is really important, because it's going to be used to compare two elements in this List. It 
     * should receive two void pointers p1 and p2 and return 1 if p1 > p2, -1 if p1 < p2 or 0 if p1 == p2 
     *   \attention 
     * There are a few functions already implemented for the most common data types (int, char, char *, float, double). 
     * You can find them in the Comparators structure, defined in the "comparator.h" header file.
     *   \note 
     * Example of use: Comparators.integer (function to compare two void pointers representing int pointers).
     */
     int (*compare) (const void*, const void*);
     // Flags: Determine the behaviour when deleting a node
     free_on_delete_t free_on_delete; // Free the element in the node when deleting it
} LinkedList;
    
/**
 * Initializes the linked list 
 * @param cmp pointer to a function that receives two void pointers and returns 1 if the first one is greater than the 
 *              second, 0 if they are equal and -1 if the first one is smaller than the second one
*/
LinkedList lnkd_list_init(size_t size, int (*cmp) (const void*, const void*));

/**
 * \brief 
 * Configures the behavior of the list when deleting a node. 
 * \note
 *  See the LinkedList structure documentation for more info
 * @param free_on_delete If true, the information of the nodes will be also freed on node deletion
*/
static inline void lnkd_list_configure(LinkedList *list, free_on_delete_t free_on_delete){
    list->free_on_delete = free_on_delete;
}

/**
 * \brief
 * Adds the given element to the end of the list. The element must be a pointer to whatever 
 * data type you wish.
 * \attention  It is recommended to only store ONE data type per list, so when you retrait the elements from
 * the list, you know you can safely cast the pointers to the correct data types.
 * \attention But there's no restriction regarding 
 * issue, so technically you can store whatever you want here. This is the C programming language :p
 * \return 1 if the operation is succesful
*/
int lnkd_list_push_back(LinkedList *list, void *element);

/**
 * \brief
 * Adds the given element to the start of the list. The element must be a pointer to whatever 
 * data type you wish.
 * \attention  It is recommended to only store ONE data type per list, so when you retrait the elements from
 * the list, you know you can safely cast the pointers to the correct data types.
 * \attention But there's no restriction regarding 
 * issue, so technically you can store whatever you want here. This is the C programming language :p
 * \return 1 if the operation is succesful
*/
int lnkd_list_push_front(LinkedList *list, void *element);

/**
 * Changes the given element in the list with the replacement, if it exists.
 * 
 * \return  true if it's able to set the new element or true if element does not exists in the list        
*/
bool lnkd_list_set(LinkedList *list, void *element, void *replacement);

/**
 * @return a pointer to the first element in the list that is equal to the parameter element.
*/
void* lnkd_list_get(LinkedList list, void *element);

/**
* @return true if the element exists in the list
*/
bool lnkd_list_exists(LinkedList list, void *element);

/**
* @return true if the list is empty
*/
bool lnkd_list_isempty(LinkedList list);
    
/**
 * Removes the element from the list
 * @return true if it deletes the element, and false if 
 *          the element doesn't exist in the list
*/
bool lnkd_list_remove(LinkedList *list, void *element);
/**
 * Frees the memory allocated on the list.
 * \note See the documentation of the LinkedList structure to learn about how 
 *       the deletion is handled regarding the elements stored in the nodes.
*/
void lnkd_list_free(LinkedList list);

void lnkd_list_reset(LinkedList *list);    

#endif
