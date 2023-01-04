#include "array_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Util/checks.h"

ArrayList arrlist_empty(int (*cmp) (void*, void*)){
    return arrlist_init(ARRAY_LIST_DEFAULT_SIZE, cmp);
}
ArrayList arrlist_init(size_t size, int (*cmp) (void*, void*)){
    void **elements = calloc(ARRAY_LIST_DEFAULT_SIZE, sizeof(void*));
    CHECK_MEMORY(elements, arrlist_init, (ArrayList){})
    return (ArrayList) {
        .elements = elements,
        .n_elements = 0,
        .max_elements = size,
        .compare = cmp,
        .free_on_delete = DONT_FREE_ON_DELETE
    };
}

void arrlist_configure(ArrayList *list, bool free_on_delete){
    list->free_on_delete = free_on_delete;
}

static void** re_calloc(void **arr, size_t size, size_t new_size){
    void **tmp = calloc(new_size, sizeof(void*));
    CHECK_MEMORY(tmp, arrlist_append, NULL)
    for(int i = 0; i < size; i++){
        tmp[i] = arr[i];
    }
    free(arr);
    return tmp;
}
int arrlist_append(ArrayList *list, void *element){
    if(list->n_elements == list->max_elements){
        list->max_elements *= 2;
        list->elements = re_calloc(list->elements, list->n_elements, list->max_elements);
        if(list->elements == NULL){
            return ALLOCATION_ERROR;
        }
    }
    list->elements[list->n_elements++] = element;
    return 1;
}

int arrlist_indexof(ArrayList list, void *element){
    for (int i=0; i<list.n_elements; i++){
        if ((*list.compare) (list.elements[i], element) == 0){
            return i;
        }
    }
    return INDEX_NOT_FOUND;
}

bool arrlist_exists(ArrayList list, void *element){
    return arrlist_indexof(list, element) != INDEX_NOT_FOUND;
}

bool arrlist_isempty(ArrayList list){
    return list.n_elements == 0;
}

int arrlist_set_at(ArrayList *list, size_t index, void *element){
    CHECK_NULL(list == NULL || element == NULL, arrlist_set_at)
    CHECK_BOUNDS(index, 0, list->n_elements, arrlist_set_at, INDEX_NOT_FOUND);
    if (list->free_on_delete == FREE_ON_DELETE){
        free(list->elements[index]);
    }
    list->elements[index] = element;
    return 1;
}

int arrlist_set(ArrayList *list, void *element, void *replacement){
    CHECK_NULL(list == NULL || element == NULL || replacement == NULL, arrlist_set)
    for (int i=0; i < list->n_elements; i++){
        if ((*list->compare) (list->elements[i], element) == 0){
            if(list->free_on_delete == FREE_ON_DELETE){
                free(list->elements[i]);
            }
            list->elements[i] = replacement;
            return i;
        }
    }
    return INDEX_NOT_FOUND;
}

void* arrlist_get_at(ArrayList list, size_t index){
    CHECK_BOUNDS(index, 0, list.n_elements, arrlist_get_at, NULL)
    return list.elements[index];
}

void* arrlist_get(ArrayList list, void *element){
    CHECK_NULL(element == NULL, arrlist_get)
    for (int i = 0; i < list.n_elements; i++){
        if((*list.compare) (list.elements[i], element) == 0){
            return list.elements[i];
        }
    }
    return NULL;
}

int arrlist_remove_at(ArrayList *list, size_t index){
    CHECK_NULL(list == NULL, arrlist_remove_at)
    CHECK_BOUNDS(index, 0, list->n_elements, arrlist_remove_at, INDEX_NOT_FOUND)

    if(list->free_on_delete == FREE_ON_DELETE){
        free(list->elements[index]);
    }
    for(int i=index; i<list->n_elements-1; i++){
        list->elements[i] = list->elements[i+1];
    }
    list->n_elements--;
    return 1;
}

int arrlist_remove(ArrayList *list, void *element){
    CHECK_NULL(list == NULL || element == NULL, arrlist_remove)
    int i = arrlist_indexof(*list, element);
    if(i!=INDEX_NOT_FOUND){
        return arrlist_remove_at(list, i);
    }else{
        return INDEX_NOT_FOUND;
    }
 }

void arrlist_free(ArrayList list){
    if(list.free_on_delete == FREE_ON_DELETE){
        for(int i = 0; i < list.n_elements; i++){
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
