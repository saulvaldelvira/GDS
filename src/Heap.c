/*
 * Heap.c - Binary Heap implementation.
 * Author: Saúl Valdelvira (2023)
 */
#include "Heap.h"
#define GDS_ENABLE_ERROR_MACROS
#include "./util/error.h"
#include "./Vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

/**
 * Heap struct
 * @headerfile Heap.h <GDS/Heap.h>
 * @see Heap.h
*/
struct Heap {
	Vector *elements;	///< Vector to hold the elements of the heap
};

/// INITIALIZE ////////////////////////////////////////////////////////////////

Heap* heap_init(size_t data_size, comparator_function_t cmp){
	if (!cmp){
		printerr_null_param();
		return NULL;
	}
	if (data_size == 0){
		printerr_data_size();
		return NULL;
	}
	Heap *heap = malloc(sizeof(*heap));
	assert(heap);
	heap->elements = vector_init(data_size, cmp);
	return heap;
}

void heap_set_comparator(Heap *heap, comparator_function_t cmp){
	if (!heap || !cmp)
		printerr_null_param();
	else
		vector_set_comparator(heap->elements, cmp);
}

void heap_set_destructor(Heap *heap, destructor_function_t destructor){
	if (!heap)
		printerr_null_param();
	else
		vector_set_destructor(heap->elements, destructor);
}

///////////////////////////////////////////////////////////////////////////////

/// FILTERING /////////////////////////////////////////////////////////////////

/**
 * Performs a filter up.
 * The filter is made recursively from the given position, going "up"
 * every iteration until we find the "root".
*/
static void filter_up(Vector *list, size_t pos){
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
static ptrdiff_t lowest_child(Vector *list, size_t pos){
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
static void filter_down(Vector *list, size_t pos){
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

int heap_add(Heap *heap, void *element){
	if (!heap || !element){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	int status = vector_append(heap->elements, element);
	if (status != SUCCESS)
		return status;
	filter_up(heap->elements, vector_size(heap->elements)-1);
	return SUCCESS;
}

int heap_add_array(Heap *heap, void *array, size_t array_length){
	if (!heap || !array){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	// If the heap is empty, we use this piece of code because it
	// uses half as much "filter_down" calls than the other one
	if (vector_size(heap->elements) == 0){
		vector_append_array(heap->elements, array, array_length);
		size_t i = array_length - 1;
		while (i > 0){
			size_t father = (i - 1) / 2;
			filter_down(heap->elements, father);
			// This is because if i is 1 and we substract 2, we get
			// the max size_t value (since it is unsigned)
			if (i > 1)
				i -=2;
			else if(i == 1)
				i--;
		}
	} else {
		size_t data_size = vector_get_data_size(heap->elements);
		while (array_length-- > 0){
			int status = heap_add(heap, array);
			if (status != SUCCESS)
				return status;
			array = void_offset(array, data_size);
		}
	}
	return SUCCESS;
}

void* heap_pop_min(Heap *heap, void *dest){
	if (!heap || !dest){
		printerr_null_param();
		return NULL;
	}
	dest = vector_get_front(heap->elements, dest);
	if (dest != NULL){
		size_t last_pos = vector_size(heap->elements) - 1;
		int status = vector_swap(heap->elements, 0, last_pos);
		if (status != SUCCESS)
			return NULL;
		vector_pop_back(heap->elements, NULL);
		filter_down(heap->elements, 0);
	}
	return dest;
}

int heap_change_priority(Heap *heap, void *element, void *replacement){
	if (!heap || !element || !replacement){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	// Get pos of the element
	ptrdiff_t pos = vector_indexof(heap->elements, element);
	if (pos < 0)
		return pos;
	// Replace with new priority
	int status = vector_set_at(heap->elements, pos, replacement);
	if (status != SUCCESS)
		return status;
	// Filter (if necessary)
	comparator_function_t comp_func = vector_get_comparator(heap->elements);
	int c = comp_func(element, replacement);
	if (c > 0)
		filter_up(heap->elements, pos);
	else if (c < 0)
		filter_down(heap->elements, pos);
	return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

/// GET-EXISTS-SIZE ///////////////////////////////////////////////////////////

void* heap_get_array(Heap *heap, size_t array_length){
	if (!heap){
		printerr_null_param();
		return NULL;
	}
	return vector_get_array(heap->elements, array_length);
}

void* heap_get_into_array(Heap *heap, void *array, size_t array_length){
	if (!heap){
		printerr_null_param();
		return NULL;
	}
	return vector_get_into_array(heap->elements, array, array_length);
}

void* heap_peek(Heap *heap, void *dest){
	if (!heap || !dest){
		printerr_null_param();
		return NULL;
	}
	return vector_get_at(heap->elements, 0, dest);
}

int heap_remove(Heap *heap, void *element){
	if (!heap || !element){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	ptrdiff_t index = vector_indexof(heap->elements, element);
	if (index < 0)
		return index;
	size_t n_elements = vector_size(heap->elements) - 1;
	int status = vector_swap(heap->elements, index, n_elements);
	if (status != SUCCESS){
		return status;
	}
	vector_pop_back(heap->elements, NULL);
	filter_down(heap->elements, index);
	return SUCCESS;
}

bool heap_exists(Heap *heap, void *element){
	if (!heap || !element){
		printerr_null_param();
		return false;
	}
	return vector_exists(heap->elements, element);
}

size_t heap_size(Heap *heap){
	if (!heap){
		printerr_null_param();
		return 0; /// ?? change ???
	}
	return vector_size(heap->elements);
}

bool heap_isempty(Heap *heap){
	if (!heap){
		printerr_null_param();
		return false;
	}
	return vector_isempty(heap->elements);
}

///////////////////////////////////////////////////////////////////////////////

//// FREE /////////////////////////////////////////////////////////////////////

int heap_free(Heap *heap){
	if (!heap){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	vector_free(heap->elements);
	free(heap);
	return SUCCESS;
}

void heap_free_all(unsigned int n, ...){
	va_list arg;
	va_start(arg, n);
	for (unsigned int i = 0; i < n; i++){
		Heap *ptr = va_arg(arg, Heap*);
		heap_free(ptr);
	}
	va_end(arg);
}

Heap* heap_reset(Heap *heap){
	if (!heap){
		printerr_null_param();
		return NULL;
	}
	heap->elements = vector_reset(heap->elements);
	if (!heap->elements){
		free(heap);
		return ERROR;
	}
	return heap;
}
