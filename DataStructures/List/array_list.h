#pragma once
#ifndef LIST_H
#define LIST_H
    #include <stddef.h>
    #include <stdbool.h>

    #define ARRAY_LIST_DEFAULT_SIZE 10

    #define INDEX_NOT_FOUND -1
    #define ALLOCATION_ERROR -2
    #define NULL_PARAMETER -3

    /**
     * ArrayList.
    */
    typedef struct ArrayList {
        void **elements;
        size_t n_elements;
        size_t max_elements;
        /**
         * Comparator function for 2 elements
        *   \attention 
        * This function is really important, because it's going to be used to compare two elements in this List. It 
        * should receive two void pointers p1 and p2 and return 1 if p1 > p2, -1 if p1 < p2 or 0 if p1 == p2 
        *   \attention 
        * There are a few functions already implemented for the most common data types (int, char, char *, float, double). 
        * You can find them in the Comparators structure, defined in the "comparator.h" header file.
        *   \note 
        * Example of use: Comparators.integer (function to compare two void pointers representing int pointers).
        */
        bool (*comp) (void*, void*);
    } ArrayList;
    
    /**
     * Creates an empty ArrayList.
     * @param comp pointer to a function that receives two void pointers and returns 1 if the first one is greater than the 
     *              second, 0 if they are equal and -1 if the first one is smaller than the second one
    */
    extern ArrayList arrlist_empty(bool (*comp) (void*, void*));

    /**
     * Innitializes an arraylist of the given size and with the given comparator function.
    */
    extern ArrayList arrlist_init(size_t size, bool (*comp) (void*, void*));

    extern int arrlist_append(ArrayList *list, void *element);

    extern int arrlist_indexof(ArrayList list, void *element);

    extern int arrlist_set_at(ArrayList *list, size_t index, void *element);
    extern int arrlist_set(ArrayList *list, void *element, void *replacement);

    extern void* arrlist_get_at(ArrayList list, size_t index);
    extern void* arrlist_get(ArrayList list, void *element);

    extern int arrlist_remove_at(ArrayList *list, size_t index);
    extern int arrlist_remove(ArrayList *list, void *element);
    
    #define FREE_ELEMENTS 1
    #define FREE_ARRAY_ONLY 0

    extern void arrlist_free(ArrayList list, int free_elements);

#endif