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
#ifndef BINARY_HEAP_MIN_H
#define BINARY_HEAP_MIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include "./Util/definitions.h"
#include "./Util/comparator.h"

typedef struct _Heap Heap;

/**
 * @return a new Binary Heap of mins with the given data size and comparator function
*/
Heap* heap_init(size_t data_size, comparator_function_t cmp);

/**
 * Changes the comparator function of the heap
 * @param cmp the new comparator function
*/
void heap_configure(Heap *heap, comparator_function_t cmp);

/**
 * Adds the element into de heap
*/
int heap_add(Heap *heap, void *element);

/**
 * Adds[array_length] elements from [array] into de heap
*/
int heap_add_array(Heap *heap, void *array, size_t array_length);

/**
 * Gets the min element in the array.
*/
void* heap_pop_min(Heap *heap, void *dest);

/**
 * Allocs an array with [array_length] elements and fills it with the elements in the heap
 * @param array_length the number of elements to get. Pass GET_ALL_ELEMENTS to get all the elements in the heap
*/
void* heap_get_array(Heap *heap, size_t array_length);

/**
 * Fills [array] with the first [array_length] elements from the heap.
 * @param array_length the number of elements to get. Pass GET_ALL_ELEMENTS to get all the elements in the heap
*/
void* heap_get_into_array(Heap *heap, void *array, size_t array_length);

/**
 * Copies to dest the min element in the heap, without deleting it
 * @return dest, or NULL if error.
*/
void* heap_peek(Heap *heap, void *dest);

/**
 * Removes an element from the heap.
 * @return 1 if the operation is successful
*/
int heap_remove(Heap *heap, void *element);

/**
 * Changes the priority of element to replacement
*/
int heap_change_priority(Heap *heap, void *element, void *replacement);

/**
 * @return true if the element exists in the heap
*/
bool heap_exists(Heap *heap, void *element);

/**
 * @return the number of elements in the heap
*/
size_t heap_size(Heap *heap);

/**
 * @return true if the heap is empty
*/
bool heap_isempty(Heap *heap);

/**
 * Frees the memory asociated with the heap
 * @return 1 if the operation is successful
*/
int heap_free(Heap *heap);

/**
 * Resets the heap to its original state.
 * @return heap, or NULL if error.
*/
Heap* heap_reset(Heap *heap);

#ifdef __cplusplus
}
#endif

#endif
