/**
 *  Copyright (C) 2022-2023  Saúl Valdelvira Iglesias
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *  Email: saulvaldelvira@gmail.com
 *  Version: 11-01-2023
 */
#pragma once
#ifndef LINKED_LINKED_LIST_H
#define LINKED_LINKED_LIST_H

#include <stddef.h>
#include <stdbool.h>
#include "../Util/comparator.h"

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
LinkedList* lnkd_list_init(size_t data_size, comparator_function_t cmp);

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
size_t lnkd_list_size(LinkedList *list);

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
