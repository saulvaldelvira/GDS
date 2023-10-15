/*
 * Vector.h - Vector definition.
 * Author: Saúl Valdelvira (2023)
 */
#pragma once
#ifndef VECTOR_H
#define VECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include "./util/compare.h"

typedef struct Vector Vector;

/**
 * Creates a Vector.
 * @param data_size the size (in bytes of the data stored)
 * @param comp pointer to a function that receives two void pointers and returns 1 if the first one is greater than the
 *              second, 0 if they are equal and -1 if the first one is smaller than the second one
 */
Vector* vector_init(size_t data_size, comparator_function_t cmp);

/**
 * Changes the comparator function of the vector
 * @param cmp the new comparator function
*/
void vector_set_comparator(Vector *vector, comparator_function_t cmp);

/**
 * @return the comparator function used by the vector
*/
comparator_function_t vector_get_comparator(Vector *vector);

/**
 * Changes the destructor function of the vector
 * @param cmp the new destructor function. NULL means no destructor.
*/
void vector_set_destructor(Vector *vector, destructor_function_t destructor);

/**
 * @return the destructor function used by the vector,
 *         or NULL if it doesn't have one
*/
destructor_function_t vector_get_destructor(Vector *vector);

/**
 * @return the data size of the vector
*/
size_t vector_get_data_size(Vector *vector);

/**
 * Adds the element to the end of the vector
 * @return 1 if the operation is successful
 */
int vector_append(Vector *vector, void *element);

/**
 * Adds the element to the front of the vector
 * @return 1 if the operation is successful
 */
int vector_push_front(Vector *vector, void *element);

/**
 * Appends a batch of elements to the end of the vector.
 * @param array source array
 * @param array_length number of elements to copy from array into the vector.
 * @return 1 if the operation is successful
*/
int vector_append_array(Vector *vector, void *array, size_t array_length);

/**
 * Inserts a batch of elements into the vector.
 * @param index index to insert the element
 * @param array source array
 * @param array_length number of elements to copy from array into the vector.
 * @return 1 if the operation is successful
*/
int vector_insert_array(Vector *vector, ptrdiff_t index, void *array, size_t array_length);

/**
 * Appends a batch of elements to the front of the vector.
 * @param array source array
 * @param array_length number of elements to copy from array into the vector.
 * @return 1 if the operation is successful
*/
int vector_push_front_array(Vector *vector, void *array, size_t array_length);

/**
 * @return the index of the element in the vector.
 */
ptrdiff_t vector_indexof(Vector *vector, void *element);

/**
 * @return true if the element exists inside the vector.
 */
bool vector_exists(Vector *vector, void *element);

/**
 * @return true if the vector is empty.
 */
bool vector_isempty(Vector *vector);

/**
 * @return the number of elements in the vector.
 */
size_t vector_size(Vector *vector);

/**
 * @return the capacity of the vector.
*/
size_t vector_capacity(Vector *vector);

/**
 * Reserves space for a certain number of elements.
 * @param n_elements number of elements to reserve space for.
 * @note It does NOT shrink the vector. For that, use vector_shrink.
*/
int vector_reserve(Vector *vector, size_t n_elements);

/**
 * Shrinks the vector to fit exactly it's content.
*/
void vector_shrink(Vector *vector);

/**
 * Fills the vector with copies of a template element.
*/
int vector_populate(Vector *vector, void *templ);

/**
 * Calls func for every element in the vector.
 * @param func a function that will be called upon every element
 *        in the vector. It's first argument is the element, and the second
 *        is args. Returns 1 for success.
 * @param args will be passed to func. It can be NULL.
*/
int vector_process(Vector *vector, int (*func) (void *,void *), void *args);

/**
 * Replaces the element at the given index with replacement.
 */
int vector_set_at(Vector *vector, ptrdiff_t index, void *replacement);

/**
 * Inserts an element in the position of another.
 * Same as:
 *     vector_insert_at(vector, vector_indexof(element), insert);
*/
int vector_insert(Vector *vector, void *element, void *insert);

/**
 * Inserts element in the given index.
*/
int vector_insert_at(Vector *vector, ptrdiff_t index, void *element);

/**
 * Replaces element with replacement.
 * @return 1 if the operation is successful
 */
int vector_set(Vector *vector, void *element, void *replacement);

/**
 * Copies into dest the element at index.
 * @param[out] dest the memory address to copy the value into.
 * @return the dest pointer, or NULL if error.
 */
void* vector_at(Vector *vector, ptrdiff_t index, void *dest);

/**
 * Same as:
 *      vector_at(vector, vector_indexof(element), dest);
 * @param[out] dest the memory address to copy the value into.
 * @return the dest pointer, or NULL if error.
 */
void* vector_get(Vector *vector, void *element, void *dest);

/**
 * Copies into dest the first element in the vector.
 * @return dest, or NULL if the vector is empty.
*/
void* vector_front(Vector *vector, void *dest);

/**
 * Copies into dest the last element in the vector.
 * @return dest, or NULL if the vector is empty.
*/
void* vector_back(Vector *vector, void *dest);

/**
 * Copies the first [array_length] elements from the vector into the array
*/
void* vector_get_into_array(Vector *vector, void *array, size_t array_length);

/**
 * Allocates an array of [array_length] elements and fills it with
 * the first [array_length] elements fom the vector.
 * @param array_length the number of elements to get.
 *        Pass 0 to get all the elements in the vector
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

/*
 * Remove vs. Pop
 * The remove functions (remove_at, remove_front, etc.) will call
 * the destructor on the removed elements, if it is defined.
 * The pop functions (pop_at, pop_back, etc.) won't.
 * If no destructor function is defined, the functions have the same behavior.
*/

/**
 * Removes the element at the given index.
 * @note If defined, the destructor will be called on the removed element.
 * @note If you don't want that, see #vector_pop_at
 * @return 1 if the operation is successful.
 */
int vector_remove_at(Vector *vector, ptrdiff_t index);

/**
 * Removes the specified element.
 * @note If defined, the destructor will be called on the removed element.
 * @note If you don't want that, see #vector_pop
 * @return 1 if the operation is successful.
 */
int vector_remove(Vector *vector, void *element);

/**
 * Removes the first element in the vector.
 * @note If defined, the destructor will be called on the removed element.
 * @note If you don't want that, see #vector_pop_front
 * @return 1 if the operation is succesful
*/
int vector_remove_front(Vector *vector);

/**
 * Removes the last element in the vector.
 * @note If defined, the destructor will be called on the removed element.
 * @note If you don't want that, see #vector_pop_back
 * @return 1 if the operation is successful
*/
int vector_remove_back(Vector *vector);

/**
 * Removes from the vector the first [array_length] elements of the given array.
 * @note If defined, the destructor will be called on the removed elements.
 * @note If you don't want that, see #vector_pop_array
 * @return 1 if the operation is successful
*/
int vector_remove_array(Vector *vector, void *array, size_t array_length);

/**
 * Pops the element at the given index.
 * @param[out] dest if not NULL, copies the element into it.
 * @note If you want the destructor to be called on the element, use #vector_remove_at instead
 * @return the dest pointer
 */
void* vector_pop_at(Vector *vector, ptrdiff_t index, void *dest);

/**
 * Pops the element
 * @param[out] dest if not NULL, copies the element into it.
 * @note If you want the destructor to be called on the element, use #vector_remove instead
 * @return the dest pointer
 */
void* vector_pop(Vector *vector, void *element, void *dest);

/**
 * Pops the first element.
 * @param[out] dest if not NULL, copies the element into it.
 * @note If you want the destructor to be called on the element, use #vector_remove_front instead
 * @return the dest pointer
*/
void* vector_pop_front(Vector *vector, void *dest);

/**
 * Pops the last element.
 * @param[out] dest if not NULL, copies the element into it.
 * @note If you want the destructor to be called on the element, use #vector_remove_back instead
 * @return the dest pointer
*/
void* vector_pop_back(Vector *vector, void *dest);

/**
 * Pops from the vector the first [array_length] elements of the given array.
 * @param[out] dest an array. If not NULL, copies the elements into it.
 *             Must be large enough to, at least, hold [array_length] elements
 * @note If you want the destructor to be called on the element, use #vector_remove_array instead
 * @return the dest pointer
*/
void* vector_pop_array(Vector *vector, void *array, size_t array_length, void *dest);

/**
 * @return A copy of the vector.
*/
Vector* vector_dup(Vector *vector);

/**
 * Removes all the elements in the vector, without freeing the
 * internal buffer (i.e. without shrinking it's capacity).
*/
void vector_clear(Vector *vector);

/**
 * @returns a new vector, with all the elements of vector_1 and vector_2.
*/
Vector* vector_join(Vector *vector_1, Vector *vector_2);

/**
 * Frees the vector.
 */
void vector_free(Vector *vector);

/**
 * Frees multiple vectors at once.
 * @param n number of vectors to free.
*/
void vector_free_all(unsigned int n, ...);

/**
 * Frees the vector and resets it to it's original state.
 */
void vector_reset(Vector *vector);

#ifdef __cplusplus
}
#endif

#endif
