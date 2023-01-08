/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 08-01-2023
 * Copyright: Saúl Valdelvira Iglesias (2023)
 * Licensed under the GNU GPL V.3. See /LICENSE file for more info
*/
#include "array_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Util/checks.h"
#include <memory.h>

ArrayList arrlist_empty(size_t data_size, int (*cmp) (const void*, const void*)){
    return arrlist_init(data_size, ARRAY_LIST_DEFAULT_SIZE, cmp);
}
ArrayList arrlist_init(size_t data_size, size_t size, int (*cmp) (const void*, const void*)){
    void *elements = malloc(ARRAY_LIST_DEFAULT_SIZE * data_size);
    CHECK_MEMORY(elements, arrlist_init, (ArrayList) {0})
    return (ArrayList) {
        .elements = elements,
        .data_size = data_size,
        .n_elements = 0,
        .max_elements = size,
        .compare = cmp,
        .free_on_delete = DontFreeOnDelete
    };
}

int arrlist_append(ArrayList *list, void *element){
    if(list->n_elements == list->max_elements){ // If the list is empty, double the array size
        list->max_elements *= 2;
        list->elements = realloc(list->elements, list->max_elements * list->data_size);
        if(list->elements == NULL){
            return ALLOCATION_ERROR;
        }
    }

    if(!memmove(offset(list->elements, list->n_elements, list->data_size), element, list->data_size)){
        fprintf(stderr, "ERROR: could not append element\n");
        return -1;
    }
    list->n_elements++;
    return 1;
}

index_t arrlist_indexof(ArrayList list, void *element){
    void *ptr; // Current element in the iteration
    for (size_t i=0; i<list.n_elements; i++){
        ptr = offset(list.elements, i, list.data_size);
        if ((*list.compare) (ptr, element) == 0){
            return index_t(i);
        }
    }
    return INDEX_NOT_FOUND;
}

bool arrlist_exists(ArrayList list, void *element){
    return arrlist_indexof(list, element).status == 1;
}

bool arrlist_isempty(ArrayList list){
    return list.n_elements == 0;
}

int arrlist_set_at(ArrayList *list, size_t index, void *element){
    CHECK_NULL(list, arrlist_set_at, INDEX_OUT_OF_BOUNDS)
    CHECK_BOUNDS(index, list->n_elements, arrlist_set_at, INDEX_OUT_OF_BOUNDS);

    if(!memmove(offset(list->elements, index, list->data_size), element, list->data_size)){
        fprintf(stderr, "ERROR: arrlist_set_at\n");
        return -1;

    }
    return -1;
}

int arrlist_set(ArrayList *list, void *element, void *replacement){
    CHECK_NULL(list, arrlist_set, INDEX_OUT_OF_BOUNDS)
    void *ptr;
    for (size_t i=0; i < list->n_elements; i++){
        ptr = offset(list->elements, i, list->data_size);
        if ((*list->compare) (ptr, element) == 0){
            if(memmove(ptr, replacement, list->data_size)){
                return 1;
            }else{
                return -1;
            }
        }
    }
    return INDEX_OUT_OF_BOUNDS;
}

void* arrlist_get_at(ArrayList list, size_t index, void *dest){
    CHECK_BOUNDS(index, list.n_elements, arrlist_get_at, NULL)
    return memcpy(dest, offset(list.elements, index, list.data_size), list.data_size);
}

void* arrlist_get(ArrayList list, void *element, void *dest){
    void *ptr;
    for (size_t i = 0; i < list.n_elements; i++){
        ptr = offset(list.elements, i, list.data_size);
        if((*list.compare) (ptr, element) == 0){
            return memcpy(dest, ptr, list.data_size);
        }
    }
    return NULL;
}

int arrlist_remove_at(ArrayList *list, size_t index){
    CHECK_NULL(list, arrlist_remove_at, INDEX_OUT_OF_BOUNDS)
    CHECK_BOUNDS(index, list->n_elements, arrlist_remove_at, INDEX_OUT_OF_BOUNDS)

    if (index < list->n_elements - 1){
        size_t leftover = (list->n_elements - index - 1) * list->data_size;
        if(!memmove(offset(list->elements, index, list->data_size), offset(list->elements, index+1, list->data_size), leftover)){
            fprintf(stderr, "ERROR: arrlist_remove at\n");
            return -1;
        }
    }
    list->n_elements--;
    return 1;
}

int arrlist_remove(ArrayList *list, void *element){
    CHECK_NULL(list, arrlist_remove, INDEX_OUT_OF_BOUNDS)
    index_t i = arrlist_indexof(*list, element);
    if(i.status){
        return arrlist_remove_at(list, i.index);
    }else{
        return INDEX_OUT_OF_BOUNDS;
    }
 }

void arrlist_free(ArrayList list){
    free(list.elements);
}

void arrlist_reset(ArrayList *list){
    arrlist_free(*list);
    list->elements = malloc(ARRAY_LIST_DEFAULT_SIZE * list->data_size);
    CHECK_MEMORY(list->elements, arrlist_reset, ;)
    list->n_elements = 0;
    list->max_elements = ARRAY_LIST_DEFAULT_SIZE;
}
