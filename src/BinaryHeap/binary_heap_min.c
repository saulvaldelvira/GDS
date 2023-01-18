#include "binary_heap_min.h"
#include "../Util/error.h"
#include <stdlib.h>
#include <stdio.h>

#define MINHEAP_DEFAULT_ELEMENTS 8

struct _MinHeap {
        size_t n_elements;
        size_t max_elements;
        comparator_function_t compare;
        size_t data_size;
        void *elements;
};

MinHeap* minheap_init(size_t data_size, comparator_function_t cmp){
        if (!cmp){
                printerr_null_param(minheap_init);
                return NULL;
        }
        MinHeap *heap = malloc(sizeof(*heap));
        if (!heap){
                printerr_allocation(minheap_init);
                return NULL;
        }
        heap->elements = malloc(MINHEAP_DEFAULT_ELEMENTS * data_size);
        if (!heap->elements){
                printerr_allocation(minheap_init);
                free(heap);
                return NULL;
        }
        heap->compare = cmp;
        heap->data_size = data_size;
        heap->n_elements = 0;
        heap->max_elements = MINHEAP_DEFAULT_ELEMENTS;
        return heap;
}

int minheap_add(MinHeap *heap, void *element){

}

void* minheap_get(MinHeap *heap, void *dest){

}

void* minheap_peek(MinHeap *heap, void *dest){

}

int minheap_remove(MinHeap *heap, void *element){

}

bool minheap_exists(MinHeap *heap, void *element){

}

size_t minheap_size(MinHeap *heap){

}

bool minheap_isempty(MinHeap *heap){
        
}

int minheap_free(MinHeap *heap){

}

MinHeap* minheap_reset(MinHeap *heap){

}
