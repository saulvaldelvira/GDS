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
#ifndef ARRAYLIST_H
#define ARRAYLIST_H
#include <stddef.h>
#include <stdbool.h>
#define ARRAY_LIST_DEFAULT_SIZE 12

#include "../Util/index_t.h"
#include "../Util/comparator.h"

typedef struct _ArrayList ArrayList;
	
/**
 * Creates an empty ArrayList.
 * @param data_size the size (in bytes of the data stored)
 * @param comp pointer to a function that receives two void pointers and returns 1 if the first one is greater than the 
 *              second, 0 if they are equal and -1 if the first one is smaller than the second one
 */
ArrayList* arrlist_empty(size_t data_size, comparator_function_t cmp);

/**
 * Innitializes an arraylist with a custom max number of elements
 * @param max_elements the initial number of elements in the list
 * @param data_size the size (in bytes of the data stored)
 * @param comp pointer to a function that receives two void pointers and returns 1 if the first one is greater than the 
 *              second, 0 if they are equal and -1 if the first one is smaller than the second one
 */
ArrayList* arrlist_init(size_t data_size, size_t max_elements, comparator_function_t cmp);

/**
 * \brief
 * Adds the value inside element into the list
 * 
 * \return 1 if the operation is successful
 */
int arrlist_append(ArrayList *list, void *element);

/**
 * Returns the index of the given element in the array of the list
 * @return index_t index, wich contains the index, and a status variable that determines if the index is valid or not.
 */
index_t arrlist_indexof(ArrayList *list, void *element);

/**
 * @return true if the element exists. Null if it doesn't or if the parameter is NULL
 */
bool arrlist_exists(ArrayList *list, void *element);

/**
 * @return true if the list is empty
 */
bool arrlist_isempty(ArrayList *list);

/**
 * @return the number of elements in the list
 */
size_t arrlist_n_elements(ArrayList *list);

/**
 * Replaces the element at the given index with the element passed as a parameter
 */
int arrlist_set_at(ArrayList *list, size_t index, void *element);

/**
 * Replaces element with replacement in the list.
 * \return 1 if the operation is successful
 */
int arrlist_set(ArrayList *list, void *element, void *replacement);



/**
 * @param dest the memory adress to copy the value into. MUST BE INITIALIZED.
 * \return The element at the given index or NULL if it the index is out of bounds
 */
void* arrlist_get_at(ArrayList *list, size_t index, void *dest);

/**
 * @param dest the memory adress to copy the value into. MUST BE INITIALIZED.
 * \return The first ocurrence of the given element or NULL if it doesn't exist in the list
 */
void* arrlist_get(ArrayList *list, void *element, void *dest);

/**
 * Removes the element at the given index
 * \return 1 if the operation is successful.
 */
int arrlist_remove_at(ArrayList *list, size_t index);

/**
 * Removes the specified element
 * \return 1 if the operation is successful.
 */
int arrlist_remove(ArrayList *list, void *element);

/**
 * Frees the list from memory
 * @return 1 if the operation is successful
 */
int arrlist_free(ArrayList *list);

/**
 * Frees the list from memory and resets it to it's original state
 * @return the same pointer if sucess, NULL if the operation fails
 */
ArrayList* arrlist_reset(ArrayList *list);

#endif