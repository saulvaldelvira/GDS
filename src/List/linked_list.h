#pragma once
#ifndef LINKED_LINKED_LIST_H
       
    typedef struct LinkedList {
        struct Node {
            struct Node *next;
            void *info;
        } *root;
        size_t n_elements;
    } LinkedList;
    
    extern List linked_list_empty();
    extern List linked_list_init(size_t size);
    extern void linked_list_append(List *list, void *element);
    extern void linked_list_set(List *list, size_t index, void *element);
    extern void* linked_list_get(List list, size_t index);
    
    #define FREE_ELEMENTS 1
    #define FREE_ARRAY_ONLY 0
    extern void linked_list_free(List list);

#endif
