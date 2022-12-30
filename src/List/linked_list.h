#pragma once
#ifndef LINKED_LINKED_LIST_H
    
    #include <stddef.h>
    #include <stdbool.h>

    typedef struct LLNode {
        struct LLNode *next;
        void *info;
    } LLNode;

    typedef struct LinkedList {
        LLNode *root;
        size_t n_elements;
        // Comparator function for nodes
        bool (*comp) (void*, void*);
    } LinkedList;
    
    extern LinkedList linked_list_init(bool (*comp) (void*, void*));
        
    extern void linked_list_append(LinkedList *list, void *element);

    extern int linked_list_set(LinkedList *list, void *element, void *replacement);
    extern void* linked_list_get(LinkedList list, void *element);
    
    #define FREE_ELEMENTS 1
    #define FREE_ARRAY_ONLY 0
    
    extern void linked_list_free(LinkedList list);
    extern bool linked_list_exists(LinkedList list, void *element);
#endif
