#include "list.h"
#include <stdlib.h>
#include <stdio.h>

List list_empty(){
    return list_init(LIST_DEFAULT_SIZE);
}
List list_init(size_t size){
        void **elements = calloc(size, sizeof(void*));
        if(!elements){
            fprintf(stderr, "Error, unable to allocate memory.\n");
            abort();
        }
        return (List) {
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
void list_append(List *list, void *element){
    if(list->n_elements == list->max_elements){
        list->max_elements *= 2;
        list->elements = re_calloc(list->elements, list->n_elements, list->max_elements);
    }
    list->elements[list->n_elements++] = element;
}

void list_set(List *list, size_t index, void *element){
    list->elements[index] = element;
}

void list_remove(List *list, size_t index){
    if(index <= 0 || index >= list->n_elements){
        fprintf(stderr, "Index %lu out of bounds\n", index);
        return;
    }
    for(int i=index; i<list->n_elements-1; i++){
        list->elements[i] = list->elements[i+1];
    }
    list->n_elements--;
}

void* list_get(List list, size_t index){
    return list.elements[index];
}

void list_free(List list, int free_elements){
    if(free_elements){
        for(int i = 0; i < list.n_elements; i++){
            free(list.elements[i]);
        }
    }
    free(list.elements);
}
