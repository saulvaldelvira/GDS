#pragma once
#ifndef LIST_H
#define LIST_H
    #include <stddef.h>

    #define LIST_DEFAULT_SIZE 10

    typedef struct List {
        void **elements;
        size_t n_elements;
        size_t max_elements;
    } List;
    
    extern List list_empty();
    extern List list_init(size_t size);
    extern void list_append(List *list, void *element);
    extern void list_set(List *list, size_t index, void *element);
    extern void* list_get(List list, size_t index);
    extern void list_remove(List *list, size_t index);
    
    #define FREE_ELEMENTS 1
    #define FREE_ARRAY_ONLY 0

    extern void list_free(List list, int free_elements);

#endif