/*
 * vector.h - vector_t definition.
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
#include "compare.h"

typedef struct vector vector_t;

/**
 * Creates a vector_t.
 * @param data_size the size (in bytes) of the data stored
 * @param cmp pointer to a function that receives two void pointers and returns 1 if the first one is greater than the
 *              second, 0 if they are equal and -1 if the first one is smaller than the second one
 */
vector_t* vector_init(size_t data_size, comparator_function_t cmp);

/**
 * Creates a vector_t with the given initial capacity.
 * @param data_size the size (in bytes) of the data stored
 * @param cmp pointer to a function that receives two void pointers and returns 1 if the first one is greater than the
 *              second, 0 if they are equal and -1 if the first one is smaller than the second one
 * @param capacity initial capacity of the vector_t.
 */
vector_t* vector_with_capacity(size_t data_size, comparator_function_t cmp, size_t capacity);

/**
 * Changes the comparator function of the vector
 * @param cmp the new comparator function
*/
void vector_set_comparator(vector_t *vector, comparator_function_t cmp);

/**
 * @return the comparator function used by the vector
*/
comparator_function_t vector_get_comparator(const vector_t *vector);

/**
 * Changes the destructor function of the vector
 * @param destructor the new destructor function. NULL means no destructor.
*/
void vector_set_destructor(vector_t *vector, destructor_function_t destructor);

/**
 * @return the destructor function used by the vector,
 *         or NULL if it doesn't have one
*/
destructor_function_t vector_get_destructor(const vector_t *vector);

/**
 * @return the data size of the vector
*/
size_t vector_get_data_size(const vector_t *vector);

/**
 * Adds the element to the end of the vector
 * @return 1 if the operation is successful
 */
int vector_append(vector_t *vector, void *element);

/**
 * Adds the element to the front of the vector
 * @return 1 if the operation is successful
 */
int vector_push_front(vector_t *vector, void *element);

/**
 * Appends a batch of elements to the end of the vector.
 * @param array source array
 * @param array_length number of elements to copy from array into the vector.
 * @return 1 if the operation is successful
*/
int vector_append_array(vector_t *vector, const void *array, size_t array_length);

/**
 * Inserts a batch of elements into the vector.
 * @param index index to insert the element
 * @param array source array
 * @param array_length number of elements to copy from array into the vector.
 * @return 1 if the operation is successful
*/
int vector_insert_array(vector_t *vector, ptrdiff_t index, const void *array, size_t array_length);

/**
 * Appends a batch of elements to the front of the vector.
 * @param array source array
 * @param array_length number of elements to copy from array into the vector.
 * @return 1 if the operation is successful
*/
int vector_push_front_array(vector_t *vector, const void *array, size_t array_length);

/**
 * @return the index of the element in the vector.
 */
ptrdiff_t vector_indexof(const vector_t *vector, void *element);

/**
 * @return true if the element exists inside the vector.
 */
bool vector_exists(const vector_t *vector, void *element);

/**
 * @return true if the vector is empty.
 */
bool vector_isempty(const vector_t *vector);

/**
 * @return the number of elements in the vector.
 */
size_t vector_size(const vector_t *vector);

/**
 * @return the capacity of the vector.
*/
size_t vector_capacity(const vector_t *vector);

/**
 * Reserves space for a certain number of elements.
 * @param n_elements number of elements to reserve space for.
 * @note It does NOT change the number of elements, it just reserves
 *       the space. For that, use vector_resize
 * @note It does NOT shrink the vector. For that, use vector_shrink.
*/
int vector_reserve(vector_t *vector, size_t n_elements);

/**
 * Resizes the vector to the given number of elements.
 * @param n_elements new number of elements.
 * @param [OPTIONAL] constructor. This function will set the
 *                   new default values.
 * @note It does NOT shrink the vector. For that, use vector_shrink.
 * @note If a destructor function is set, make sure you properly initialize
 * the new elements with a constructor functions.
*/
int vector_resize(vector_t *vector, size_t n_elements, constructor_function_t constructor);

/**
 * Shrinks the vector to fit exactly it's content.
*/
int vector_shrink(vector_t *vector);

/**
 * Applies 'func' to every element in the vector
 * @param func function to apply
 * @param args (optional) passed to func as the second parameter
*/
void vector_map(vector_t *vector, void (*func) (void *,void *), void *args);

/**
* Returns a new vector_t containing all the elements that match the
* a given predicate.
* @param func a function that receives an element and returns
*        true if it must be added to the result vector.
*/
vector_t* vector_filter(vector_t *vector, bool (*func) (void*));

/**
 * Performs the Quick Sort algorithm on the vector_t.
*/
void vector_sort(vector_t *vector);

/**
 * Reduces all element of the vector into a single element.
 * @param func function that receives an element as first parameter and
 *             the accumulated element (i.e. the result of calling func
 *               on all the previous elements) as it's second parameter.
 * @param dest address to store the result into
 * @return the dest pointer
 */
void* vector_reduce(vector_t *vector, void (*func) (const void*,void*), void *dest);

/**
 * Replaces the element at the given index with replacement.
 */
int vector_set_at(vector_t *vector, ptrdiff_t index, void *replacement);

/**
 * Inserts an element in the position of another.
 * Same as:
 *     vector_insert_at(vector, vector_indexof(element), insert);
*/
int vector_insert(vector_t *vector, void *element, void *insert);

/**
 * Inserts element in the given index.
*/
int vector_insert_at(vector_t *vector, ptrdiff_t index, void *element);

/**
 * Replaces element with replacement.
 * @return 1 if the operation is successful
 */
int vector_set(vector_t *vector, void *element, void *replacement);

/**
 * Copies into dest the element at index.
 * @param[out] dest the memory address to copy the value into.
 * @return the dest pointer, or NULL if error.
 */
void* vector_at(const vector_t *vector, ptrdiff_t index, void *dest);

/**
 * Same as:
 *      vector_at(vector, vector_indexof(element), dest);
 * @param[out] dest the memory address to copy the value into.
 * @return the dest pointer, or NULL if error.
 */
void* vector_get(const vector_t *vector, void *element, void *dest);

/**
 * Copies into dest the first element in the vector.
 * @return dest, or NULL if the vector is empty.
*/
void* vector_front(const vector_t *vector, void *dest);

/**
 * Copies into dest the last element in the vector.
 * @return dest, or NULL if the vector is empty.
*/
void* vector_back(const vector_t *vector, void *dest);

void* vector_at_ref(vector_t *self, ptrdiff_t index);
void* vector_get_ref(vector_t *vector, void *element);
void* vector_front_ref(vector_t *vector);
void* vector_back_ref(vector_t *vector);

/**
 * Returns the inner buffer behind the vector
 */
void* vector_get_buffer(vector_t *self);

/**
 * Copies the first [array_length] elements from the vector into the array
*/
void* vector_get_into_array(const vector_t *vector, void *array, size_t array_length);

/**
 * Allocates an array of [array_length] elements and fills it with
 * the first [array_length] elements fom the vector.
 * @param array_length the number of elements to get.
 *        Pass 0 to get all the elements in the vector
*/
void* vector_get_array(const vector_t *vector, size_t array_length);

/**
 * Swaps the element at index_1 and index_2
*/
int vector_swap(vector_t *vector, ptrdiff_t index_1, ptrdiff_t index_2);

/**
 * Compares the element at index_1 and index_2
 * @return the result of the comparison
*/
int vector_compare(const vector_t *vector, ptrdiff_t index_1, ptrdiff_t index_2);

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
int vector_remove_at(vector_t *vector, ptrdiff_t index);

/**
 * Removes the specified element.
 * @note If defined, the destructor will be called on the removed element.
 * @note If you don't want that, see #vector_pop
 * @return 1 if the operation is successful.
 */
int vector_remove(vector_t *vector, void *element);

/**
 * Removes the first element in the vector.
 * @note If defined, the destructor will be called on the removed element.
 * @note If you don't want that, see #vector_pop_front
 * @return 1 if the operation is succesful
*/
int vector_remove_front(vector_t *vector);

/**
 * Removes the last element in the vector.
 * @note If defined, the destructor will be called on the removed element.
 * @note If you don't want that, see #vector_pop_back
 * @return 1 if the operation is successful
*/
int vector_remove_back(vector_t *vector);

/**
 * Removes from the vector the first [array_length] elements of the given array.
 * @note If defined, the destructor will be called on the removed elements.
 * @note If you don't want that, see #vector_pop_array
 * @return 1 if the operation is successful
*/
int vector_remove_array(vector_t *vector, void *array, size_t array_length);

/**
 * Pops the element at the given index.
 * @param[out] dest if not NULL, copies the element into it.
 * @note If you want the destructor to be called on the element, use #vector_remove_at instead
 * @return the dest pointer
 */
void* vector_pop_at(vector_t *vector, ptrdiff_t index, void *dest);

/**
 * Pops the element
 * @param[out] dest if not NULL, copies the element into it.
 * @note If you want the destructor to be called on the element, use #vector_remove instead
 * @return the dest pointer
 */
void* vector_pop(vector_t *vector, void *element, void *dest);

/**
 * Pops the first element.
 * @param[out] dest if not NULL, copies the element into it.
 * @note If you want the destructor to be called on the element, use #vector_remove_front instead
 * @return the dest pointer
*/
void* vector_pop_front(vector_t *vector, void *dest);

/**
 * Pops the last element.
 * @param[out] dest if not NULL, copies the element into it.
 * @note If you want the destructor to be called on the element, use #vector_remove_back instead
 * @return the dest pointer
*/
void* vector_pop_back(vector_t *vector, void *dest);

/**
 * Pops from the vector the first [array_length] elements of the given array.
 * @param[out] dest an array. If not NULL, copies the elements into it.
 *             Must be large enough to, at least, hold [array_length] elements
 * @note If you want the destructor to be called on the element, use #vector_remove_array instead
 * @return the dest pointer
*/
void* vector_pop_array(vector_t *vector, void *array, size_t array_length, void *dest);

/**
 * @return A copy of the vector.
*/
vector_t* vector_dup(vector_t *vector);

/**
 * Removes all the elements in the vector, without freeing the
 * internal buffer (i.e. without shrinking it's capacity).
*/
void vector_clear(vector_t *vector);

/**
 * @returns a new vector, with all the elements of vector_1 and vector_2.
*/
vector_t* vector_join(const vector_t *vector_1, const vector_t *vector_2);

void vector_free(vector_t *v, ...);

/**
 * Frees all the given vectors.
 */
#define vector_free(...) vector_free(__VA_ARGS__, 0L)

/**
 * Frees the vector and resets it to it's original state.
 */
void vector_reset(vector_t *vector);

/* ITERATOR */
typedef struct vector_iterator_t {
        const vector_t *vector;
        ptrdiff_t next, prev;
} vector_iterator_t;

/*
 * Create an iterator of the given vector.
 * CAUTION:
 * DO NOT use this iterator after the original vector has been freed.
 * */
vector_iterator_t vector_iterator(const vector_t *vector);
vector_iterator_t vector_iterator_from_back(const vector_t *vector);

void* vector_it_next(vector_iterator_t *it, void *dst);
void* vector_it_prev(vector_iterator_t *it, void *dst);
void* vector_it_peek_next(vector_iterator_t *it, void *dst);
void* vector_it_peek_prev(vector_iterator_t *it, void *dst);
bool vector_it_has_next(vector_iterator_t *it);
bool vector_it_has_prev(vector_iterator_t *it);

#ifdef __cplusplus
}
#endif

#endif
