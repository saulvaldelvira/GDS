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

/**
 * @brief Linked List structure.
 */
typedef struct _LinkedList LinkedList;
    
/**
 * Initializes the linked list 
 * @param data_size size of the data stored.
 * @param cmp pointer to a function that receives two void pointers and returns 1 if the first one is greater than the 
 *              second, 0 if they are equal and -1 if the first one is smaller than the second one
*/
LinkedList* lnkd_list_init(size_t data_size, int (*cmp) (const void*, const void*));

/**
 * Adds the given element to the end of the list
 * @return 1 if the operation is successful
*/
int lnkd_list_push_back(LinkedList *list, void *element);

/**
 * Adds the given element to the start of the list
 * @return 1 if the operation is successful
*/
int lnkd_list_push_front(LinkedList *list, void *element);

/**
 * Changes the given element in the list with the replacement, if it exists.
 * 
 * @return 1 if the operation is successful
*/
int lnkd_list_set(LinkedList *list, void *element, void *replacement);

/**
 * @param dest adress to store the reuslt in. MUST BE INITIALIZED.
 * @return a pointer to the first element in the list that is equal to the parameter element.
*/
void* lnkd_list_get(LinkedList *list, void *element, void *dest);

/**
* @return true if the element exists in the list
*/
bool lnkd_list_exists(LinkedList *list, void *element);

/**
 * @return the number of elements in the list
*/
size_t lnkd_list_n_elements(LinkedList *list);

/**
* @return true if the list is empty
*/
bool lnkd_list_isempty(LinkedList *list);
    
/**
 * Removes the element from the list
 * @return 1 if the operation is successful
*/
int lnkd_list_remove(LinkedList *list, void *element);

/**
 * Frees the memory allocated on the list.
*/
int lnkd_list_free(LinkedList *list);

/**
 * Frees the memory allocated on the list, and resets it to it's initial state
 * @return the same pointer if sucess, NULL if the operation fails
*/
LinkedList* lnkd_list_reset(LinkedList *list);    

#endif
