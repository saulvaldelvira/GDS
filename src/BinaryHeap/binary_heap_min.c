/**
 *  Copyright (C) 2022-2023  Saúl Valdelvira Iglesias
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *  Email: saulvaldelvira@gmail.com
 *  Version: 18-01-2023
 */
#include "binary_heap_min.h"
#include "../Util/error.h"
#include "../Vector/vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

struct _MinHeap {
        Vector *elements;
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
        heap->elements = vector_empty(data_size, cmp);
        if (!heap->elements){
                printerr_allocation(minheap_init);
                free(heap);
                return NULL;
        }
        return heap;
}

void minheap_configure(MinHeap *heap, comparator_function_t cmp){
        if (!heap || !cmp){
                printerr_null_param(minheap_configure);
                return;
        }
        vector_configure(heap->elements, cmp);
}

/**
 * Performs a filter up.
 * The filter is made recursively from the given position, going "up"
 * every iteration until we find the "root".
*/
static void filter_up(Vector *list, size_t pos){
        size_t father = (pos-1) / 2;
        if (pos == 0 || vector_compare(list, pos, father) >= 0){
                return;
        }
        vector_swap(list, pos, father);
        filter_up(list, father);
}

/**
 * Returns an index_t with the lowest of the childs of a given
 * position (or with a status of -1 if thre is no valid childs)
*/
static index_t lowest_child(Vector *list, size_t pos){
        size_t l_child = pos * 2 + 1;
        size_t r_child = pos * 2 + 2;
        size_t size = vector_size(list);

        index_t lowest  = {0,SUCCESS};

        if (l_child >= size){
                if (r_child >= size){
                        lowest.status = -1;
                        return lowest;
                }else {
                        lowest.value = r_child;
                }
        }else if (r_child >= size){
                lowest.value = l_child;
        }else{
                if (vector_compare(list, l_child, r_child) < 0){
                        lowest.value = l_child;
                }else{
                        lowest.value = r_child;
                }
        }

        if (vector_compare(list, lowest.value, pos) >= 0){
                lowest.status = -1;
                return lowest;
        }
        return lowest;
}

/**
 * Performs a filter up.
 * The filter is made recursively from the given position, going "down"
 * every iteration until we find the last element.
*/
static void filter_down(Vector *list, size_t pos){
        if (pos >= vector_size(list)){
                return;
        }
        index_t lowest = lowest_child(list, pos);
        if (lowest.status != SUCCESS){
                return;
        }
        vector_swap(list, pos, lowest.value);
        filter_down(list, lowest.value);
}

int minheap_add(MinHeap *heap, void *element){
        if (!heap || !element){
                printerr_null_param(minheap_add);
                return NULL_PARAMETER_ERROR;
        }
        int status = vector_append(heap->elements, element);
        if (status != SUCCESS){
                return status;
        }
        filter_up(heap->elements, vector_size(heap->elements)-1);
        return SUCCESS;
}

int minheap_add_array(MinHeap *heap, void *array, size_t array_length){
        if (!heap || !array){
                printerr_null_param(minheap_add_array);
                return NULL_PARAMETER_ERROR;
        }
        // If the heap is empty, we use this piece of code because it
        // uses half as much "filter_down" calls than the other one
        if (vector_size(heap->elements) == 0){
                vector_append_array(heap->elements, array, array_length);
                size_t father;
                for (size_t i = array_length-1; i > 0; ){
                        father = (i-1) / 2;
                        filter_down(heap->elements, father);

                        // This is because if i is 1 and we substract 2, we get
                        // the max size_t value (since it is unsigned)
                        if (i > 1){
                                i -=2;
                        }else if(i == 1){
                                i--;
                        }
                }
        } else {
                size_t data_size = vector_get_data_size(heap->elements);
                void *tmp;
                int status;
                for (size_t i = 0; i < array_length; i++){
                        tmp = void_offset(array, i * data_size);
                        status = minheap_add(heap, tmp);
                        if (status != SUCCESS){
                                return status;
                        }
                }
        }
        return SUCCESS;
}

void* minheap_pop_min(MinHeap *heap, void *dest){
        if (!heap || !dest){
                printerr_null_param(minheap_pop_min);
                return NULL;
        }
        dest = vector_get_at(heap->elements, 0, dest);

        if (dest != NULL){
                size_t n_elements = vector_size(heap->elements) - 1;
                int status = vector_swap(heap->elements, 0, n_elements);
                if (status != SUCCESS){
                        return NULL;
                }
                vector_remove_at(heap->elements, n_elements);

                filter_down(heap->elements, 0);
        }
        return dest;
}

void* minheap_get_array(MinHeap *heap, size_t array_length){
        if (!heap){
                printerr_null_param(minheap_get_array);
                return NULL;
        }
        return vector_get_array(heap->elements, array_length);
}

void* minheap_get_into_array(MinHeap *heap, void *array, size_t array_length){
        if (!heap){
                printerr_null_param(minheap_get_array);
                return NULL;
        }
        return vector_get_into_array(heap->elements, array, array_length);
}

void* minheap_peek(MinHeap *heap, void *dest){
        if (!heap || !dest){
                printerr_null_param(minheap_peek);
                return NULL;
        }
        return vector_get_at(heap->elements, 0, dest);
}

int minheap_remove(MinHeap *heap, void *element){
        if (!heap || !element){
                printerr_null_param(minheap_remove);
                return NULL_PARAMETER_ERROR;
        }
        index_t index = vector_indexof(heap->elements, element);
        if (index.status != SUCCESS){
                return index.status;
        }
        size_t n_elements = vector_size(heap->elements) - 1;
        int status = vector_swap(heap->elements, index.value, n_elements);
        if (status != SUCCESS){
                return status;
        }
        vector_remove_at(heap->elements, n_elements);

        filter_down(heap->elements, index.value);
        return SUCCESS;
}

bool minheap_exists(MinHeap *heap, void *element){
        if (!heap || !element){
                printerr_null_param(minheap_exists);
                return false;
        }
        return vector_exists(heap->elements, element);
}

size_t minheap_size(MinHeap *heap){
        if (!heap){
                printerr_null_param(minheap_size);
                return 0; /// ?? change ???
        }
        return vector_size(heap->elements);
}

bool minheap_isempty(MinHeap *heap){
        if (!heap){
                printerr_null_param(minheap_isempty);
                return false;
        }
        return vector_isempty(heap->elements);
}

int minheap_free(MinHeap *heap){
        if (!heap){
                printerr_null_param(minheap_free);
                return NULL_PARAMETER_ERROR;
        }
        int status = vector_free(heap->elements);
        if (status != SUCCESS){
                free(heap);
                return status;
        }
        free(heap);
        return SUCCESS;
}

MinHeap* minheap_reset(MinHeap *heap){
        if (!heap){
                printerr_null_param(minheap_reset);
                return NULL;
        }
        heap->elements = vector_reset(heap->elements);
        if (!heap->elements){
                free(heap);
                return ERROR;
        }
        return heap;
}
