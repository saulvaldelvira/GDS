#pragma once
#ifndef BINARY_HEAP_MIN_H
#define BINARY_HEAP_MIN_H
#include <stddef.h>
#include <stdbool.h>
#include "../Util/definitions.h"
#include "../Util/comparator.h"

typedef struct _MinHeap MinHeap;

/**
 * @return a new Binary Heap of mins with the given data size and comparator function
*/
MinHeap* minheap_init(size_t data_size, comparator_function_t cmp);

/**
 * Adds the element into de heap
*/
int minheap_add(MinHeap *heap, void *element);

/**
 * Adds[array_length] elements from [array] into de heap
*/
int minheap_add_array(MinHeap *heap, void *array, size_t array_length);

/**
 * Gets the min element in the array.
*/
void* minheap_get_min(MinHeap *heap, void *dest);

/**
 * Allocs an array with [array_length] elements and fills it with the elements in the heap
 * @param array_length the number of elements to get. Pass GET_ALL_ELEMENTS to get all the elements in the heap
*/
void* minheap_get_array(MinHeap *heap, size_t array_length);

/**
 * Fills [array] with the first [array_length] elements from the heap.
 * @param array_length the number of elements to get. Pass GET_ALL_ELEMENTS to get all the elements in the heap
*/
void* minheap_get_into_array(MinHeap *heap, void *array, size_t array_length);

/**
 * Copies to dest the min element in the heap, without deleting it
 * @return dest, or NULL if error.
*/
void* minheap_peek(MinHeap *heap, void *dest);

/**
 * Removes an element from the heap.
 * @return 1 if the operation is successful
*/
int minheap_remove(MinHeap *heap, void *element);

/**
 * @return true if the element exists in the heap
*/
bool minheap_exists(MinHeap *heap, void *element);

/**
 * @return the number of elements in the heap
*/
size_t minheap_size(MinHeap *heap);

/**
 * @return true if the heap is empty
*/
bool minheap_isempty(MinHeap *heap);

/**
 * Frees the memory asociated with the heap
 * @return 1 if the operation is successful
*/
int minheap_free(MinHeap *heap);

/**
 * Resets the heap to its original state.
 * @return heap, or NULL if error.
*/
MinHeap* minheap_reset(MinHeap *heap);

#endif