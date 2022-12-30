#include "array_list.h"
#include <stdlib.h>
#include <stdio.h>

ArrayList arrlist_empty(){
    return arrlist_init(ARRAY_LIST_DEFAULT_SIZE);
}
ArrayList arrlist_init(size_t size){
        void **elements = calloc(size, sizeof(void*));
        if(!elements){
            fprintf(stderr, "Error, unable to allocate memory.\n");
            abort();
        }
        return (ArrayList) {
            .elements = elements,
            .n_elements = 0,
            .max_elements = size
        };
}
static void** re_calloc(void **arr, size_t size, size_t new_size){
    void **tmp = calloc(new_size, sizeof(void*));
    if(!tmp){
        fprintf(stderr, "Error, unable to allocate memory.\n");
        abort();
    }
    for(int i = 0; i < size; i++){
        tmp[i] = arr[i];
    }
    free(arr);
    return tmp;
}
void arrlist_append(ArrayList *list, void *element){
    if(list->n_elements == list->max_elements){
        list->max_elements *= 2;
        list->elements = re_calloc(list->elements, list->n_elements, list->max_elements);
    }
    list->elements[list->n_elements++] = element;
}

void arrlist_set(ArrayList *list, size_t index, void *element){
    list->elements[index] = element;
}

void arrlist_remove(ArrayList *list, size_t index){
    if(index <= 0 || index >= list->n_elements){
        fprintf(stderr, "Index %lu out of bounds\n", index);
        return;
    }
    for(int i=index; i<list->n_elements-1; i++){
        list->elements[i] = list->elements[i+1];
    }
    list->n_elements--;
}

void* arrlist_get(ArrayList list, size_t index){
    return list.elements[index];
}

void arrlist_free(ArrayList list, int free_elements){
    if(free_elements){
        for(int i = 0; i < list.n_elements; i++){
            free(list.elements[i]);
        }
    }
    free(list.elements);
}
