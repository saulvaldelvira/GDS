#pragma once
#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H
#include <stddef.h>
#include <stdbool.h>
#include "../Util/definitions.h"
#include "../Util/comparator.h"

typedef struct _MinHeap MinHeap;

MinHeap* minheap_init(size_t data_size, comparator_function_t cmp);
int minheap_add(MinHeap *heap, void *element);
void* minheap_get(MinHeap *heap, void *dest);
void* minheap_peek(MinHeap *heap, void *dest);
int minheap_remove(MinHeap *heap, void *element);
bool minheap_exists(MinHeap *heap, void *element);
size_t minheap_size(MinHeap *heap);
bool minheap_isempty(MinHeap *heap);

int minheap_free(MinHeap *heap);
MinHeap* minheap_reset(MinHeap *heap);

#endif