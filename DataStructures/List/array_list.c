#include "array_list.h"
#include <stdlib.h>
#include <stdio.h>

ArrayList arrlist_empty(bool (*comp) (void*, void*)){
    return arrlist_init(ARRAY_LIST_DEFAULT_SIZE, *comp);
}
ArrayList arrlist_init(size_t size, bool (*comp) (void*, void*)){
    void **elements = calloc(size, sizeof(void*));
    if(!elements){
        fprintf(stderr, "Error, unable to allocate memory.\n");
        abort();
    }
    return (ArrayList) {
        .elements = elements,
        .n_elements = 0,
        .max_elements = size,
        .comp = comp
    };
}

static void** re_calloc(void **arr, size_t size, size_t new_size){
    void **tmp = calloc(new_size, sizeof(void*));
    if(!tmp){
        fprintf(stderr, "Error, unable to allocate memory.\n");
        return NULL;
    }
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
        if ((*list.comp) (list.elements[i], element)){
            return i;
        }
    }
    return -1;
}

int arrlist_set_at(ArrayList *list, size_t index, void *element){
    if (list == NULL || element == NULL){
        fprintf(stderr, "List and element can't be NULL\n");
        return NULL_PARAMETER;
    }
    if (index <= 0 || index >= list->n_elements){
        fprintf(stderr, "Index %lu out of bounds\n", index);
        return INDEX_NOT_FOUND;
    }
    list->elements[index] = element;
    return 1;
}

int arrlist_set(ArrayList *list, void *element, void *replacement){
    if (list == NULL || element == NULL || replacement == NULL){
        fprintf(stderr, "List and elements can't be NULL\n");
        return NULL_PARAMETER;
    }
    for (int i=0; i < list->n_elements; i++){
        if ((*list->comp) (list->elements[i], element)){
            list->elements[i] = replacement;
            return i;
        }
    }
    return INDEX_NOT_FOUND;
}

void* arrlist_get_at(ArrayList list, size_t index){
    return list.elements[index];
}

void* arrlist_get(ArrayList list, void *element){
    if (element == NULL){
        fprintf(stderr, "Element can't be NULL\n");
        return NULL_PARAMETER;
    }
    for (int i = 0; i < list.n_elements; i++){
        if((*list.comp) (list.elements[i], element)){
            return list.elements[i];
        }
    }
    return INDEX_NOT_FOUND;
}

int arrlist_remove_at(ArrayList *list, size_t index){
    if (list == NULL){
        fprintf(stderr, "List can't be NULL\n");
        return NULL_PARAMETER;
    }
    if(index <= 0 || index >= list->n_elements){
        fprintf(stderr, "Index %lu out of bounds\n", index);
        return INDEX_NOT_FOUND;
    }
    for(int i=index; i<list->n_elements-1; i++){
        list->elements[i] = list->elements[i+1];
    }
    list->n_elements--;
    return 1;
}

int arrlist_remove(ArrayList *list, void *element){
    if (list == NULL || element == NULL){
        fprintf(stderr, "List and element can't be NULL\n");
        return NULL_PARAMETER;
    }
    int i = arrlist_indexof(*list, element);
    if(i!=INDEX_NOT_FOUND){
        return arrlist_remove_at(list, i);
    }else{
        return INDEX_NOT_FOUND;
    }
 }

void arrlist_free(ArrayList list, int free_elements){
    if(free_elements){
        for(int i = 0; i < list.n_elements; i++){
            free(list.elements[i]);
        }
    }
    free(list.elements);
}
