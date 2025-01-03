/*
 * heap.h - Binary heap_t definition.
 * Author: Saúl Valdelvira (2023)
 */
#pragma once
#ifndef HEAP_H
#define HEAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include "compare.h"
#include "attrs.h"

#ifndef heap_t
typedef void* heap_t;
#endif

/**
 * @return a new Binary Heap with the given data size and comparator function
*/
NONNULL()
heap_t* heap_init(size_t data_size, comparator_function_t cmp);

/**
 * Sets the comparator function of the heap
 * @param cmp the new comparator function
*/
NONNULL()
void heap_set_comparator(heap_t *heap, comparator_function_t cmp);

/**
 * Sets the destructor function of the heap
*/
NONNULL(1)
void heap_set_destructor(heap_t *heap, destructor_function_t destructor);

/**
 * Adds the element into de heap
*/
NONNULL()
int heap_add(heap_t *heap, void *element);

/**
 * Adds[array_length] elements from [array] into de heap
*/
NONNULL()
int heap_add_array(heap_t *heap, void *array, size_t array_length);

/**
 * Gets the min element in the array.
*/
NONNULL(1)
void* heap_pop_min(heap_t *heap, void *dest);

/**
 * Allocs an array with [array_length] elements and fills it with the elements in the heap
 * @param array_length the number of elements to get. Pass 0 to get all the elements in the heap
*/
NONNULL()
void* heap_get_array(const heap_t *heap, size_t array_length);

/**
 * Fills [array] with the first [array_length] elements from the heap.
 * @param array_length the number of elements to get. Pass GET_ALL_ELEMENTS to get all the elements in the heap
*/
NONNULL()
void* heap_get_into_array(const heap_t *heap, void *array, size_t array_length);

/**
 * Copies to dest the min element in the heap, without deleting it
 * @return dest, or NULL if error.
*/
NONNULL()
void* heap_peek(const heap_t *heap, void *dest);

/**
 * Removes an element from the heap.
 * @return 1 if the operation is successful
*/
NONNULL()
int heap_remove(heap_t *heap, void *element);

/**
 * Removes all the elements from the heap.
*/
NONNULL()
void heap_clear(heap_t *heap);

/**
 * Changes the priority of element to replacement
*/
NONNULL()
int heap_change_priority(heap_t *heap, void *element, void *replacement);

/**
 * @return true if the element exists in the heap
*/
NONNULL()
bool heap_exists(const heap_t *heap, void *element);

/**
 * @return the number of elements in the heap
*/
NONNULL()
size_t heap_size(const heap_t *heap);

/**
 * @return true if the heap is empty
*/
NONNULL()
bool heap_isempty(const heap_t *heap);

NONNULL()
void heap_free(heap_t *h, ...);

/**
 * Frees all the given heaps.
 */
#define heap_free(...) heap_free(__VA_ARGS__, 0L)

#ifdef __cplusplus
}
#endif

#endif // HEAP_H
