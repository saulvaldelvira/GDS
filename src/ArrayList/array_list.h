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
    #define ARRAY_LIST_DEFAULT_SIZE 10

    /**
     * Represents a position on the list. Used in methods to return a value alongside a status identifier of the operation result.
    */
    typedef struct index{
        union {
            size_t index;
            void *element;
        } value;
        int status;
    } index_t;

    #define index_t_i(i)    (index_t) {.value.index = i, .status = 1}
    #define index_t_e(e)    (index_t) {.value.element = e, .status = 1}
    #define INDEX_NOT_FOUND (index_t) {.value.element = NULL, -1} // An index_t struct with status set to -1, to represent a failure in the procedure

    /**
     * @brief Array List structure.
     * 
     * \attention
     * To make this structure "generic", it uses void pointers to the elements it stores.
     * When you delete an element, you can choose to also free the 
     * memory alocated for that element "free(void* element)". The behaviour is defined by this parameter.
     * 
     * @param free_on_delete If true, when deleting an element, it's memory is freed.
     * 
     *   \note
     * Sometimes you'll need the memory to be deleted, and sometimes that memory isn't allocated so you 
     * don't need to free those void pointers.
     * BE REALLY CAREFUL, because if you set those parameters to true and add elements to the list that don't need 
     * to be freed, you'll cause a segmentation fault. Also, it could cause trouble to add elements wich memory has been allocated at
     * the same time. 
     * \note .
     * \note ***********CODE EXAMPLE****************
     * \note int *ptr = malloc (3 * sizeof(int));
     * \note ptr[0] = 0;
     * \note ptr[1] = 1;
     * \note ptr[2] = 2;
     * 
     * \note arrlist_append(&list, &ptr[0]);
     * \note arrlist_append(&list, &ptr[1]);
     * 
     * \note arrlist_remove(&list, &ptr[0]);
     * 
     * \note ****************************************
     * 
     * \note In this last instruction, if free_on_delete is 1, the list will free &ptr[0]. But ptr holds memory for 3 elements so 
     * &ptr[1] and &ptr[2] will also be freed. This will cause trouble if the list tries to access those already fred directions in the future.
     * 
     * \note To avoid this, you ca use the functions defined in "allocate.h", that allocate memory for the most common data types (alloc_int, alloc_char, alloc_float, etc)
     * 
     * \note 
     *  NOTE 2: The default value is 0 (don't free), so also keep in mind that unless you configure the list, that memory is not being 
     *  freed on deletion.
     * */
    typedef struct ArrayList {
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
        int (*compare) (const void*, const void*);
        free_on_delete_t free_on_delete; // Free the element when deleting it
        void **elements;
    } ArrayList;
    
    /**
     * Creates an empty ArrayList.
     * @param comp pointer to a function that receives two void pointers and returns 1 if the first one is greater than the 
     *              second, 0 if they are equal and -1 if the first one is smaller than the second one
    */
    extern ArrayList arrlist_empty(int (*cmp) (const void*, const void*));

    /**
     * Innitializes an arraylist of the given size and with the given comparator function.
    */
    extern ArrayList arrlist_init(size_t size, int (*cmp) (const void*, const void*));

    /**
     * \brief 
     * Configures the behavior of the list when deleting a node. 
     * \note
     *  See the LinkedList structure documentation for more info
     * @param free_on_delete If true, the elements will be also freed on deletion
    */
    static inline void arrlist_configure(ArrayList *list, free_on_delete_t free_on_delete){
        list->free_on_delete = free_on_delete;
    }

    /**
     * \brief
     * Adds the given element to the end of the list. The element must be a pointer to whatever 
     * data type you wish.
     * \attention  It is recommended to only store ONE data type per list, so when you retrait the elements from
     * the list, you know you can safely cast the pointers to the correct data types.
     * \attention But there's no restriction regarding 
     * issue, so technically you can store whatever you want here. This is the C programming language :p
     * 
     * \return 1 if the operation is succesful
    */
    extern int arrlist_append(ArrayList *list, void *element);

    /**
     * Returns the index of the given element in the array of the list
     * @return index_t index, wich contains the index, and a status variable that determines if the 
     *  index and element are valid or not.
    */
    extern index_t arrlist_indexof(ArrayList list, void *element);

    /**
     * @return true if the element exists
    */
   extern bool arrlist_exists(ArrayList list, void *element);

    /**
     * @return true if the list is empty
    */
    extern bool arrlist_isempty(ArrayList list);

    /**
     * Replaces the element at the given index with the element passed as a parameter
    */
    extern index_t arrlist_set_at(ArrayList *list, size_t index, void *element);

    /**
     * Replaces element with replacement in the list.
     * \return an index_t type variable, in wich status will be 1 if the operation has been sucessful. Also, this
     * variable contains the index in wich the operation has been performed. 
    */
    extern index_t arrlist_set(ArrayList *list, void *element, void *replacement);

    /**
     * \return The element at the given index or NULL if it the index is out of bounds
    */
    extern void* arrlist_get_at(ArrayList list, size_t index);

    /**
     * \return The first ocurrence of the given element or NULL if it doesn't exist in the list
    */
    extern void* arrlist_get(ArrayList list, void *element);

    /**
     * Removes the element at the given index
     * \return an index_t type variable, in wich status will be 1 if the operation has been sucessful. Also, this
     * variable contains the element in that index (if it hasn't been freed from memory) 
    */
    extern index_t arrlist_remove_at(ArrayList *list, size_t index);

    /**
     * Removes the specified element
     * \return an index_t type variable, in wich status will be 1 if the operation has been sucessful. Also, this
     * variable contains the index of that element (if it could find it in the list) 
    */
    extern index_t arrlist_remove(ArrayList *list, void *element);

    /**
     * Frees the list from memory
    */
    extern void arrlist_free(ArrayList list);

    /**
     * Frees the list from memory and resets it to it's original state
    */
    extern void arrlist_reset(ArrayList *list);

#endif