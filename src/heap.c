/*
 * heap.c - Binary heap implementation.
 * Author: Saúl Valdelvira (2023)
 */
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include "error.h"
#include "./vector.h"
#include "definitions.h"

/*
 * A heap is just a vector with a different interface.
 * To avoid memory waste and indirection, heap_t is an
 * alias of vector_t, instead of being a struct heap { vector_t *vec; }
 * which would've required an extra memory allocation and two pointer
 * dereferences to achieve the same effect.
 * */
#define heap_t vector_t
#include "heap.h"

/// INITIALIZE ////////////////////////////////////////////////////////////////

heap_t* heap_init(size_t data_size, comparator_function_t cmp){
        assert(cmp && data_size > 0);
        return vector_init(data_size, cmp);
}

void heap_set_comparator(heap_t *heap, comparator_function_t cmp){
        if (heap && cmp)
                vector_set_comparator(heap, cmp);
}

void heap_set_destructor(heap_t *heap, destructor_function_t destructor){
        if (heap)
                vector_set_destructor(heap, destructor);
}

///////////////////////////////////////////////////////////////////////////////

/// FILTERING /////////////////////////////////////////////////////////////////

/**
 * Performs a filter up.
 * The filter is made recursively from the given position, going "up"
 * every iteration until we find the "root".
*/
static void filter_up(vector_t *list, size_t pos){
        size_t father = (pos-1) / 2;
        if (pos == 0 || vector_compare(list, pos, father) >= 0)
                return;
        vector_swap(list, pos, father);
        filter_up(list, father);
}

/**
 * Returns the  with the lowest of the childs of a given
 * position (or with a status of -1 if thre is no valid childs)
*/
static ptrdiff_t lowest_child(vector_t *list, size_t pos){
        size_t l_child = pos * 2 + 1;
        size_t r_child = pos * 2 + 2;
        size_t size = vector_size(list);

        ptrdiff_t lowest  = 0;

        if (l_child >= size){
                if (r_child >= size)
                        return -1;
                else
                        lowest = r_child;
        }else if (r_child >= size){
                lowest = l_child;
        }else{
                if (vector_compare(list, l_child, r_child) < 0)
                        lowest = l_child;
                else
                        lowest = r_child;
        }
        if (vector_compare(list, lowest, pos) >= 0)
                return -1;
        return lowest;
}

/**
 * Performs a filter up.
 * The filter is made recursively from the given position, going "down"
 * every iteration until we find the last element.
*/
static void filter_down(vector_t *list, size_t pos){
        if (pos >= vector_size(list))
                return;
        ptrdiff_t lowest = lowest_child(list, pos);
        if (lowest < 0)
                return;
        vector_swap(list, pos, lowest);
        filter_down(list, lowest);
}

///////////////////////////////////////////////////////////////////////////////

//// ADD-REMOVE ///////////////////////////////////////////////////////////////

int heap_add(heap_t *heap, void *element){
        assert(heap && element);
        int status = vector_append(heap, element);
        if (status != GDS_SUCCESS)
                return status;
        filter_up(heap, vector_size(heap)-1);
        return GDS_SUCCESS;
}

int heap_add_array(heap_t *heap, void *array, size_t array_length){
        assert(heap && array);
        // If the heap is empty, we use this piece of code because it
        // uses half as much "filter_down" calls than the other one
        if (vector_size(heap) == 0){
                vector_append_array(heap, array, array_length);
                size_t i = array_length - 1;
                while (i > 0){
                        size_t father = (i - 1) / 2;
                        filter_down(heap, father);
                        // This is because if i is 1 and we substract 2, we get
                        // the max size_t value (since it is unsigned)
                        if (i > 1)
                                i -=2;
                        else
                                i--;
                }
        } else {
                size_t data_size = vector_get_data_size(heap);
                while (array_length-- > 0){
                        int status = heap_add(heap, array);
                        if (status != GDS_SUCCESS)
                                return status;
                        array = void_offset(array, data_size);
                }
        }
        return GDS_SUCCESS;
}

void* heap_pop_min(heap_t *heap, void *dest){
        assert(heap && dest);
        dest = vector_front(heap, dest);
        if (dest != NULL){
                size_t last_pos = vector_size(heap) - 1;
                int status = vector_swap(heap, 0, last_pos);
                if (status != GDS_SUCCESS)
                        return NULL;
                vector_pop_back(heap, dest);
                filter_down(heap, 0);
        }
        return dest;
}

int heap_change_priority(heap_t *heap, void *element, void *replacement){
        assert(heap && element && replacement);
        // Get pos of the element
        ptrdiff_t pos = vector_indexof(heap, element);
        if (pos < 0)
                return pos;
        // Replace with new priority
        int status = vector_set_at(heap, pos, replacement);
        if (status != GDS_SUCCESS)
                return status;
        // Filter (if necessary)
        comparator_function_t comp_func = vector_get_comparator(heap);
        int c = comp_func(element, replacement);
        if (c > 0)
                filter_up(heap, pos);
        else if (c < 0)
                filter_down(heap, pos);
        return GDS_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

/// GET-EXISTS-SIZE ///////////////////////////////////////////////////////////

void* heap_get_array(const heap_t *heap, size_t array_length){
        assert(heap);
        return vector_get_array(heap, array_length);
}

void* heap_get_into_array(const heap_t *heap, void *array, size_t array_length){
        assert(heap);
        return vector_get_into_array(heap, array, array_length);
}

void* heap_peek(const heap_t *heap, void *dest){
        assert(heap && dest);
        return vector_at(heap, 0, dest);
}

void heap_clear(heap_t *heap){
        if (heap)
                vector_clear(heap);
}

int heap_remove(heap_t *heap, void *element){
        assert(heap && element);
        ptrdiff_t index = vector_indexof(heap, element);
        if (index < 0)
                return index;
        size_t n_elements = vector_size(heap) - 1;
        int status = vector_swap(heap, index, n_elements);
        if (status != GDS_SUCCESS){
                return status;
        }
        vector_pop_back(heap, NULL);
        filter_down(heap, index);
        return GDS_SUCCESS;
}

bool heap_exists(const heap_t *heap, void *element){
        assert(heap && element);
        return vector_exists(heap, element);
}

size_t heap_size(const heap_t *heap){
        return heap ? vector_size(heap) : 0;
}

bool heap_isempty(const heap_t *heap){
        return heap ? vector_isempty(heap) : true;
}

///////////////////////////////////////////////////////////////////////////////

//// FREE /////////////////////////////////////////////////////////////////////

__inline
static void _heap_free(heap_t *heap){
        vector_free(heap);
}

void (heap_free)(heap_t *h, ...){
        if (!h)
                return;
        va_list arg;
        va_start(arg, h);
        do {
                _heap_free(h);
                h = va_arg(arg, heap_t*);
        } while (h);
        va_end(arg);
}
