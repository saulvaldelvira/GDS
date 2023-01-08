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

ArrayList arrlist_empty(int (*cmp) (const void*, const void*)){
    return arrlist_init(ARRAY_LIST_DEFAULT_SIZE, cmp);
}
ArrayList arrlist_init(size_t size, int (*cmp) (const void*, const void*)){
    void **elements = calloc(ARRAY_LIST_DEFAULT_SIZE, sizeof(void*));
    CHECK_MEMORY(elements, arrlist_init, (ArrayList){})
    return (ArrayList) {
        .elements = elements,
        .n_elements = 0,
        .max_elements = size,
        .compare = cmp,
        .free_on_delete = DontFreeOnDelete
    };
}

/**
 * Reallocs the array of void pointers. It's the same as realloc but the new array is also allocated with 
 * calloc instead of malloc. The purpose is that empty pointers in the list have the value 0
*/
static void** re_calloc(void **arr, size_t size, size_t new_size){
    void **tmp = calloc(new_size, sizeof(void*));
    CHECK_MEMORY(tmp, arrlist_append, NULL)
    for(size_t i = 0; i < size; i++){
        tmp[i] = arr[i];
    }
    free(arr);
    return tmp;
}
int arrlist_append(ArrayList *list, void *element){
    if(list->n_elements == list->max_elements){ // If the list is empty, double the array size
        list->max_elements *= 2;
        list->elements = re_calloc(list->elements, list->n_elements, list->max_elements);
        if(list->elements == NULL){
            return ALLOCATION_ERROR;
        }
    }
    list->elements[list->n_elements++] = element;
    return 1;
}

index_t arrlist_indexof(ArrayList list, void *element){
    for (size_t i=0; i<list.n_elements; i++){
        if ((*list.compare) (list.elements[i], element) == 0){
            return index_t_i(i);
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

index_t arrlist_set_at(ArrayList *list, size_t index, void *element){
    CHECK_NULL(list == NULL || element == NULL, arrlist_set_at, INDEX_NOT_FOUND)
    CHECK_BOUNDS(index, list->n_elements, arrlist_set_at, INDEX_NOT_FOUND);
    void *ret; // Element in the given index
    if (list->free_on_delete == FreeOnDelete){ // If we have to free the element
        free(list->elements[index]);
        ret = NULL; // The return index_t's element will be NULL since that memory will alrready be free
    }else{
        ret = list->elements[index]; // Else we return that substituted element with the result
    }
    list->elements[index] = element;
    return index_t_e(ret);
}

index_t arrlist_set(ArrayList *list, void *element, void *replacement){
    CHECK_NULL(list == NULL || element == NULL || replacement == NULL, arrlist_set, INDEX_NOT_FOUND)
    for (size_t i=0; i < list->n_elements; i++){
        if ((*list->compare) (list->elements[i], element) == 0){
            if(list->free_on_delete == FreeOnDelete){
                free(list->elements[i]);
            }
            list->elements[i] = replacement;
            return index_t_i(i);
        }
    }
    return INDEX_NOT_FOUND;
}

void* arrlist_get_at(ArrayList list, size_t index){
    CHECK_BOUNDS(index, list.n_elements, arrlist_get_at, NULL)
    return list.elements[index];
}

void* arrlist_get(ArrayList list, void *element){
    CHECK_NULL(element == NULL, arrlist_get, NULL)
    for (size_t i = 0; i < list.n_elements; i++){
        if((*list.compare) (list.elements[i], element) == 0){
            return list.elements[i];
        }
    }
    return NULL;
}

index_t arrlist_remove_at(ArrayList *list, size_t index){
    CHECK_NULL(list == NULL, arrlist_remove_at, INDEX_NOT_FOUND)
    CHECK_BOUNDS(index, list->n_elements, arrlist_remove_at, INDEX_NOT_FOUND)
    void *e;
    if(list->free_on_delete == FreeOnDelete){
        free(list->elements[index]);
        e = NULL;
    }else {
        e = list->elements[index];
    }
    for(size_t i=index; i<list->n_elements-1; i++){
        list->elements[i] = list->elements[i+1];
    }
    list->n_elements--;
    return index_t_e(e);
}

index_t arrlist_remove(ArrayList *list, void *element){
    CHECK_NULL(list == NULL || element == NULL, arrlist_remove, INDEX_NOT_FOUND)
    index_t i = arrlist_indexof(*list, element);
    if(i.status){
        return arrlist_remove_at(list, i.value.index);
    }else{
        return INDEX_NOT_FOUND;
    }
 }

void arrlist_free(ArrayList list){
    if(list.free_on_delete == FreeOnDelete){
        for(size_t i = 0; i < list.n_elements; i++){
            free(list.elements[i]);
        }
    }
    free(list.elements);
}

void arrlist_reset(ArrayList *list){
    arrlist_free(*list);
    list->elements = calloc(ARRAY_LIST_DEFAULT_SIZE, sizeof(void*));
    CHECK_MEMORY(list->elements, arrlist_reset, ;)
    list->n_elements = 0;
    list->max_elements = ARRAY_LIST_DEFAULT_SIZE;
}
