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
 *  Version: 17-01-2023
 */
#pragma once
#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H
#include <stddef.h>
#include <stdbool.h>
#define ARRAY_LIST_DEFAULT_SIZE 12

#include "../Util/index_t.h"
#include "../Util/comparator.h"

typedef struct _DynamicArray DynamicArray;
	
/**
 * Creates an empty DynamicArray.
 * @param data_size the size (in bytes of the data stored)
 * @param comp pointer to a function that receives two void pointers and returns 1 if the first one is greater than the 
 *              second, 0 if they are equal and -1 if the first one is smaller than the second one
 */
DynamicArray* din_arr_empty(size_t data_size, comparator_function_t cmp);

/**
 * Innitializes an DynamicArray with a custom max number of elements
 * @param max_elements the initial number of elements in the list
 * @param data_size the size (in bytes of the data stored)
 * @param comp pointer to a function that receives two void pointers and returns 1 if the first one is greater than the 
 *              second, 0 if they are equal and -1 if the first one is smaller than the second one
 */
DynamicArray* din_arr_init(size_t data_size, size_t max_elements, comparator_function_t cmp);

/**
 * @return the data size of the list
*/
size_t din_arr_get_data_size(DynamicArray *list);

/**
 * @return the comparator function used by the list
*/
comparator_function_t din_arr_get_comparator(DynamicArray *list);

/**
 * \brief
 * Adds the value inside element to the end of the list
 * 
 * @return 1 if the operation is successful
 */
int din_arr_append(DynamicArray *list, void *element);

/**
 * \brief
 * Adds the value inside element into the list
 * 
 * @return 1 if the operation is successful
 */
int din_arr_push_front(DynamicArray *list, void *element);

/**
 * Appends [array_length] elements to the end of list, from the given array.
*/
int din_arr_append_array(DynamicArray *list, void *array, size_t array_length);

/**
 * Appends [array_length] elements to the list, from the given array.
*/
int din_arr_push_front_array(DynamicArray *list, void *array, size_t array_length);


/**
 * Returns the index of the given element in the array of the list
 * @return index_t index, wich contains the index, and a status variable that determines if the index is valid or not.
 */
index_t din_arr_indexof(DynamicArray *list, void *element);

/**
 * @return true if the element exists. Null if it doesn't or if the parameter is NULL
 */
bool din_arr_exists(DynamicArray *list, void *element);

/**
 * @return true if the list is empty
 */
bool din_arr_isempty(DynamicArray *list);

/**
 * @return the number of elements in the list
 */
size_t din_arr_size(DynamicArray *list);

/**
 * Replaces the element at the given index with the element passed as a parameter
 */
int din_arr_set_at(DynamicArray *list, size_t index, void *element);

/**
 * Replaces element with replacement in the list.
 * @return 1 if the operation is successful
 */
int din_arr_set(DynamicArray *list, void *element, void *replacement);

/**
 * @param dest the memory adress to copy the value into. MUST BE INITIALIZED.
 * @return The element at the given index or NULL if it the index is out of bounds
 */
void* din_arr_get_at(DynamicArray *list, size_t index, void *dest);

/**
 * @param dest the memory adress to copy the value into. MUST BE INITIALIZED.
 * @return The first ocurrence of the given element or NULL if it doesn't exist in the list
 */
void* din_arr_get(DynamicArray *list, void *element, void *dest);

/**
 * Gets the first [array_length] elements from the list into the array
*/
void* din_arr_get_into_array(DynamicArray *list, void *array, size_t array_length);

/**
 * Allocates an array of [array_length] elements and fills it with
 * the first [array_length] elements fom the list
 * @param array_length the number of elements to get. Pass GET_ALL_ELEMENTS to get all the elements in the list
*/
void* din_arr_get_array(DynamicArray *list, size_t array_length);

/**
 * Swaps the element at index_1 and index_2
*/
int din_arr_swap(DynamicArray *list, size_t index_1, size_t index_2);

/**
 * Compares the element at index_1 and index_2
 * @return the result of the comparison
*/
int din_arr_compare(DynamicArray *list, size_t index_1, size_t index_2);

/**
 * Removes the element at the given index
 * @return 1 if the operation is successful.
 */
int din_arr_remove_at(DynamicArray *list, size_t index);

/**
 * Removes the specified element
 * @return 1 if the operation is successful.
 */
int din_arr_remove(DynamicArray *list, void *element);

/**
 * Removes from the first [array_length] elements of the array.
*/
int din_arr_remove_array(DynamicArray *list, void *array, size_t array_length);

/**
 * @return a new DynamicArray with the elements of the two given lists.
 * @note No particular order of elements is guaranteed.
 * @note ATTENTION: The lists must store the same data. At least, they must have 
 * the same data size. Also, the comparator function of list_1 will be taken, 
 * because it is assumed that both list have the same comparator functions.
*/
DynamicArray* din_arr_join(DynamicArray *list_1, DynamicArray *list_2);

/**
 * Frees the list from memory
 * @return 1 if the operation is successful
 */
int din_arr_free(DynamicArray *list);

/**
 * Frees the list from memory and resets it to it's original state
 * @return the same pointer if sucess, NULL if the operation fails
 */
DynamicArray* din_arr_reset(DynamicArray *list);

#endif