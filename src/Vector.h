/**
 *  Copyright (C) 2023 - Saúl Valdelvira
 *  License: BSD 3-Clause
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
#include "./util/comparator.h"

typedef struct Vector Vector;

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
void vector_set_comparator(Vector *vector, comparator_function_t cmp);

/**
 * @return the comparator function used by the list
*/
comparator_function_t vector_get_comparator(Vector *vector);

/**
 * Changes the destructor function of the list
 * @param cmp the new destructor function, NULL means no destructor.
*/
void vector_set_destructor(Vector *vector, destructor_function_t destructor);

/**
 * @return the destructor function used by the list, if it has one.
*/
destructor_function_t vector_get_destructor(Vector *vector);

/**
 * @return the data size of the list
*/
size_t vector_get_data_size(Vector *vector);

/**
 * Adds the value inside element to the end of the list
 *
 * @return 1 if the operation is successful
 */
int vector_append(Vector *vector, void *element);

/**
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
 * Inserts all the elements in the array to the given index in the vector.
 * @return 1 if the operation is successful
 */
int vector_insert_array(Vector *vector, ptrdiff_t index, void *array, size_t array_length);

/**
 * Push [array_length] elements to the begining of the list, from the given array.
*/
int vector_push_front_array(Vector *vector, void *array, size_t array_length);

/**
 * Returns the index of the given element in the array of the list
 */
ptrdiff_t vector_indexof(Vector *vector, void *element);

/**
 * @return true if the element exists. Null if it doesn't or if the parameter is NULL
 */
bool vector_exists(Vector *vector, void *element);

/**
 * @return true if the vector is empty
 */
bool vector_isempty(Vector *vector);

/**
 * @return the number of elements in the vector
 */
size_t vector_size(Vector *vector);

/**
 * @return the max number of elements the vector has memory allocated for (currently)
*/
size_t vector_capacity(Vector *vector);

/**
 * Reserves space for, at least, the specified number of elements.
 * @note It does NOT shrink the vector. For that, use vector_shrink
*/
int vector_reserve(Vector *vector, size_t n_elements);

/**
 * Shrinks the vector to fit exactly it's content
*/
int vector_shrink(Vector *vector);

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
int vector_set_at(Vector *vector, ptrdiff_t index, void *element);

/**
 * Inserts in the position of element the value [insert]
*/
int vector_insert(Vector *vector, void *element, void *insert);

/**
 * Inserts element in the position [index]
*/
int vector_insert_at(Vector *vector, ptrdiff_t index, void *element);

/**
 * Replaces element with replacement in the list.
 * @return 1 if the operation is successful
 */
int vector_set(Vector *vector, void *element, void *replacement);

/**
 * @param dest the memory adress to copy the value into. MUST BE INITIALIZED.
 * @return The element at the given index or NULL if it the index is out of bounds
 */
void* vector_get_at(Vector *vector, ptrdiff_t index, void *dest);

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
int vector_swap(Vector *vector, ptrdiff_t index_1, ptrdiff_t index_2);

/**
 * Compares the element at index_1 and index_2
 * @return the result of the comparison
*/
int vector_compare(Vector *vector, ptrdiff_t index_1, ptrdiff_t index_2);

/**
 * Remove vs. Pop
 * The remove functions (remove_at, remove_front, etc.) will call the destructor
 * on the removed elements, if it is defined. The pop functions (pop_at, pop_back, etc.) won't.
 * -> It is assumed that, if a structure has a destructor it is responsible of it's contents,
 *    thus it should take care of "destroying" them.
 * -> It is also assumed that remove means "destroy this element, it's no longer needed"
 *    whereas pop means "extract the element from the strucuture"
 *    That's why the pop functions will return the "popped" element, but the remove ones won't.
*/

/**
 * Removes the element at the given index.
 * @note If defined, the destructor will be called on the removed element.
 * @note If you don't want that, see vector_pop_at
 * @return 1 if the operation is successful.
 */
int vector_remove_at(Vector *vector, ptrdiff_t index);

/**
 * Removes the specified element.
 * @note If defined, the destructor will be called on the removed element.
 * @note If you don't want that, see vector_pop
 * @return 1 if the operation is successful.
 */
int vector_remove(Vector *vector, void *element);

/**
 * Removes the first element in the list.
 * @note If defined, the destructor will be called on the removed element.
 * @note If you don't want that, see vector_pop_front
 * @return 1 if the operation is succesful
*/
int vector_remove_front(Vector *vector);

/**
 * Removes the last element in the list.
 * @note If defined, the destructor will be called on the removed element.
 * @note If you don't want that, see vector_pop_back
 * @return 1 if the operation is successful
*/
int vector_remove_back(Vector *vector);

/**
 * Removes from the first [array_length] elements of the given array.
 * @note If defined, the destructor will be called on the removed elements.
 * @note If you don't want that, see vector_pop_array
 * @return 1 if the operation is successful
*/
int vector_remove_array(Vector *vector, void *array, size_t array_length);

/**
 * Pops the element at the given index.
 * @param dest if not NULL, copies the element into it.
 * @note If you want the destructor to be called on the element, use vector_remove_at instead
 * @return the dest pointer
 */
void* vector_pop_at(Vector *vector, ptrdiff_t index, void *dest);

/**
 * Pops the element
 * @param dest if not NULL, copies the element into it.
 * @note If you want the destructor to be called on the element, use vector_remove instead
 * @return the dest pointer
 */
void* vector_pop(Vector *vector, void *element, void *dest);

/**
 * Pops the first element.
 * @param dest if not NULL, copies the element into it.
 * @note If you want the destructor to be called on the element, use vector_remove_front instead
 * @return the dest pointer
*/
void* vector_pop_front(Vector *vector, void *dest);

/**
 * Pops the last element.
 * @param dest if not NULL, copies the element into it.
 * @note If you want the destructor to be called on the element, use vector_remove_back instead
 * @return the dest pointer
*/
void* vector_pop_back(Vector *vector, void *dest);

/**
 * Pops from the first [array_length] elements of the given array.
 * @param dest an array. If not NULL, copies the elements into it.
 *             Must be large enough to, at least, hold [array_length] elements
 * @note If you want the destructor to be called on the element, use vector_remove_array instead
 * @return the dest pointer
*/
void* vector_pop_array(Vector *vector, void *array, size_t array_length, void *dest);


/**
 * @return a new Vector with the elements of the two given vectors.
 * @note No particular order of elements is guaranteed.
 * @note ATTENTION: The lists must store the same data. At least, they must have
 * the same data size.
 * @note Also, it is assumed that both list have the same comparator functions.
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
