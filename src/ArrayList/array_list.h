/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 08-01-2023
 * Copyright: Saúl Valdelvira Iglesias (2023)
 * Licensed under the GNU GPL V.3. See /LICENSE file for more info
*/
#pragma once
#ifndef ARRAYLIST_H
#define ARRAYLIST_H
    #include <stddef.h>
    #include <stdbool.h>
    #include "../definitions.h"
    #define ARRAY_LIST_DEFAULT_SIZE 12

    /**
     * Represents a position on the list. Used in methods to return a value alongside a status identifier of the operation result.
    */
    typedef struct index{
        size_t index;
        int status;
    } index_t;

    #define index_t(i)    (index_t) {.index = i, .status = 1}
    #define INDEX_NOT_FOUND (index_t) {.index = 0, -1} // An index_t struct with status set to -1, to represent a failure in the procedure

    /**
     * @brief Array List structure.
     **/
    typedef struct ArrayList {
        size_t n_elements;
        size_t max_elements;
        size_t data_size;
        
        // Comparator function for 2 elements
        int (*compare) (const void*, const void*);
        void *elements;
    } ArrayList;
    
    /**
     * Creates an empty ArrayList.
     * @param data_size the size (in bytes of the data stored)
     * @param comp pointer to a function that receives two void pointers and returns 1 if the first one is greater than the 
     *              second, 0 if they are equal and -1 if the first one is smaller than the second one
    */
    extern ArrayList arrlist_empty(size_t data_size, int (*cmp) (const void*, const void*));

    /**
     * Innitializes an arraylist with a custom max number of elements
     * @param max_elements the initial number of elements in the list
     * @param data_size the size (in bytes of the data stored)
     * @param comp pointer to a function that receives two void pointers and returns 1 if the first one is greater than the 
     *              second, 0 if they are equal and -1 if the first one is smaller than the second one
    */
    extern ArrayList arrlist_init(size_t data_size, size_t max_elements, int (*cmp) (const void*, const void*));

    /**
     * \brief
     * Adds the value inside element into the list
     * 
     * \return 1 if the operation is successful
    */
    extern int arrlist_append(ArrayList *list, void *element);

    /**
     * Returns the index of the given element in the array of the list
     * @return index_t index, wich contains the index, and a status variable that determines if the index is valid or not.
    */
    extern index_t arrlist_indexof(ArrayList list, void *element);

    /**
     * @return true if the element exists. Null if it doesn't or if the parameter is NULL
    */
   extern bool arrlist_exists(ArrayList list, void *element);

    /**
     * @return true if the list is empty
    */
    extern bool arrlist_isempty(ArrayList list);

    /**
     * Replaces the element at the given index with the element passed as a parameter
    */
    extern int arrlist_set_at(ArrayList *list, size_t index, void *element);

    /**
     * Replaces element with replacement in the list.
     * \return 1 if the operation is successful
    */
    extern int arrlist_set(ArrayList *list, void *element, void *replacement);

    /**
     * @param dest the memory adress to copy the value into. MUST BE INITIALIZED.
     * \return The element at the given index or NULL if it the index is out of bounds
    */
    extern void* arrlist_get_at(ArrayList list, size_t index, void *dest);

    /**
     * @param dest the memory adress to copy the value into. MUST BE INITIALIZED.
     * \return The first ocurrence of the given element or NULL if it doesn't exist in the list
    */
    extern void* arrlist_get(ArrayList list, void *element, void *dest);

    /**
     * Removes the element at the given index
     * \return 1 if the operation is successful.
    */
    extern int arrlist_remove_at(ArrayList *list, size_t index);

    /**
     * Removes the specified element
     * \return 1 if the operation is successful.
    */
    extern int arrlist_remove(ArrayList *list, void *element);

    /**
     * Frees the list from memory
    */
    extern void arrlist_free(ArrayList list);

    /**
     * Frees the list from memory and resets it to it's original state
     * @return 1 if the operation is successful
    */
    extern int arrlist_reset(ArrayList *list);

#endif