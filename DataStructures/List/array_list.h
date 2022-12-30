#pragma once
#ifndef LIST_H
#define LIST_H
    #include <stddef.h>

    #define ARRAY_LIST_DEFAULT_SIZE 10

    typedef struct ArrayList {
        void **elements;
        size_t n_elements;
        size_t max_elements;
    } ArrayList;
    
    extern ArrayList arrlist_empty();
    extern ArrayList arrlist_init(size_t size);
    extern void arrlist_append(ArrayList *list, void *element);
    extern void arrlist_set(ArrayList *list, size_t index, void *element);
    extern void* arrlist_get(ArrayList list, size_t index);
    extern void arrlist_remove(ArrayList *list, size_t index);
    
    #define FREE_ELEMENTS 1
    #define FREE_ARRAY_ONLY 0

    extern void arrlist_free(ArrayList list, int free_elements);

#endif