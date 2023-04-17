/**
 *  Copyright (C) 2023 - Saúl Valdelvira
 *
 *  This library is free software. You can modify
 *  and/or redistribute it under the terms of the
 *  GNU Lesser General Public License version 2.1
 *  See <https://www.gnu.org/licenses/>
 *
 *  Email: saulvaldelvira@gmail.com
 */
#pragma once
#ifndef VECTOR_H
#define VECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>

#include "./Util/index_t.h"
#include "./Util/comparator.h"

typedef struct _Vector Vector;

/**
 * Creates a Vector.
 * @param data_size the size (in bytes of the data stored)
 * @param comp pointer to a function that receives two void pointers and returns 1 if the first one is greater than the
 *              second, 0 if they are equal and -1 if the first one is smaller than the second one
 */
Vector* vector_init(size_t data_size, comparator_function_t cmp);

/**
 * Changes the comparator function of the list
 * @param cmp the new comparator function
*/
void vector_configure(Vector *vector, comparator_function_t cmp);

/**
 * @return the data size of the list
*/
size_t vector_get_data_size(Vector *vector);

/**
 * @return the comparator function used by the list
*/
comparator_function_t vector_get_comparator(Vector *vector);

/**
 * \brief
 * Adds the value inside element to the end of the list
 *
 * @return 1 if the operation is successful
 */
int vector_append(Vector *vector, void *element);

/**
 * \brief
 * Adds the value inside element into the list
 *
 * @return 1 if the operation is successful
 */
int vector_push_front(Vector *vector, void *element);

/**
 * Appends [array_length] elements to the end of list, from the given array.
*/
int vector_append_array(Vector *vector, void *array, size_t array_length);

/**
 * Appends [array_length] elements to the list, from the given array.
*/
int vector_push_front_array(Vector *vector, void *array, size_t array_length);

/**
 * Returns the index of the given element in the array of the list
 * @return index_t index, wich contains the index, and a status variable that determines if the index is valid or not.
 */
index_t vector_indexof(Vector *vector, void *element);

/**
 * @return true if the element exists. Null if it doesn't or if the parameter is NULL
 */
bool vector_exists(Vector *vector, void *element);

/**
 * @return true if the list is empty
 */
bool vector_isempty(Vector *vector);

/**
 * @return the number of elements in the list
 */
size_t vector_size(Vector *vector);

/**
 * Reserves space for the specified number of elements.
 * @note It does NOT shrink the vector.
*/
int vector_reserve(Vector *vector, size_t n_elements);

/**
 * Fills the vector with copies of the given template element.
*/
int vector_populate(Vector *vector, void *template);

/**
 * Calls func for every element in the vector.
*/
int vector_process(Vector *vector, int (*func) (void *,void *), void *args);

/**
 * Replaces the element at the given index with the element passed as a parameter
 */
int vector_set_at(Vector *vector, size_t index, void *element);

/**
 * Inserts in the position of element the value [insert]
*/
int vector_insert(Vector *vector, void *element, void *insert);

/**
 * Inserts element in the position [index]
*/
int vector_insert_at(Vector *vector, size_t index, void *element);

/**
 * Replaces element with replacement in the list.
 * @return 1 if the operation is successful
 */
int vector_set(Vector *vector, void *element, void *replacement);

/**
 * @param dest the memory adress to copy the value into. MUST BE INITIALIZED.
 * @return The element at the given index or NULL if it the index is out of bounds
 */
void* vector_get_at(Vector *vector, size_t index, void *dest);

/**
 * @param dest the memory adress to copy the value into. MUST BE INITIALIZED.
 * @return The first ocurrence of the given element or NULL if it doesn't exist in the list
 */
void* vector_get(Vector *vector, void *element, void *dest);

/**
 * @return The first element in the list, or NULL if it does not exists
*/
void* vector_get_front(Vector *vector, void *dest);

/**
 * @return The last element in the list, or NULL if it does not exists
*/
void* vector_get_back(Vector *vector, void *dest);

/**
 * Gets the first [array_length] elements from the list into the array
*/
void* vector_get_into_array(Vector *vector, void *array, size_t array_length);

/**
 * Allocates an array of [array_length] elements and fills it with
 * the first [array_length] elements fom the list
 * @param array_length the number of elements to get. Pass GET_ALL_ELEMENTS to get all the elements in the list
*/
void* vector_get_array(Vector *vector, size_t array_length);

/**
 * Swaps the element at index_1 and index_2
*/
int vector_swap(Vector *vector, size_t index_1, size_t index_2);

/**
 * Compares the element at index_1 and index_2
 * @return the result of the comparison
*/
int vector_compare(Vector *vector, size_t index_1, size_t index_2);

/**
 * Removes the element at the given index
 * @return 1 if the operation is successful.
 */
int vector_remove_at(Vector *vector, size_t index);

/**
 * Removes the specified element
 * @return 1 if the operation is successful.
 */
int vector_remove(Vector *vector, void *element);

/**
 * Removes the first element in the list and copies it into dest
 * @return the pointer dest, or NULL if error
*/
void* vector_pop_front(Vector *vector, void *dest);

/**
 * Removes the last element in the list and copies it into dest
 * @return the pointer dest, or NULL if error
*/
void* vector_pop_back(Vector *vector, void *dest);

/**
 * Removes from the first [array_length] elements of the array.
*/
int vector_remove_array(Vector *vector, void *array, size_t array_length);

/**
 * @return a new Vector with the elements of the two given lists.
 * @note No particular order of elements is guaranteed.
 * @note ATTENTION: The lists must store the same data. At least, they must have
 * the same data size. Also, the comparator function of list_1 will be taken,
 * because it is assumed that both list have the same comparator functions.
*/
Vector* vector_join(Vector *vector_1, Vector *vector_2);

/**
 * Frees the vector from memory
 * @return 1 if the operation is successful
 */
int vector_free(Vector *vector);

/**
 * Frees multiple vector at once.
 * @n number of pointers to free.
*/
void vector_free_all(unsigned int n, ...);

/**
 * Frees the vector from memory and resets it to it's original state
 * @return the same pointer if sucess, NULL if the operation fails
 */
Vector* vector_reset(Vector *vector);

#ifdef __cplusplus
}
#endif

#endif
