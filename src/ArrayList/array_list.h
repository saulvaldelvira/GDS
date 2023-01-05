#pragma once
#ifndef ARRAYLIST_H
#define ARRAYLIST_H
    #include <stddef.h>
    #include <stdbool.h>
    
    #define ARRAY_LIST_DEFAULT_SIZE 10

    // ERRORS

    // free_on_delete posible values
    #ifndef FREE_ON_DELETE
        #define FREE_ON_DELETE 1
    #endif

    #ifndef DONT_FREE_ON_DELETE
        #define DONT_FREE_ON_DELETE 0
    #endif


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
        int (*compare) (void*, void*);
        bool free_on_delete; // Free the element when deleting it
    } ArrayList;
    
    /**
     * Creates an empty ArrayList.
     * @param comp pointer to a function that receives two void pointers and returns 1 if the first one is greater than the 
     *              second, 0 if they are equal and -1 if the first one is smaller than the second one
    */
    extern ArrayList arrlist_empty(int (*cmp) (void*, void*));

    /**
     * Innitializes an arraylist of the given size and with the given comparator function.
    */
    extern ArrayList arrlist_init(size_t size, int (*cmp) (void*, void*));

    /**
     * \brief 
     * Configures the behavior of the list when deleting a node. 
     * \note
     *  See the LinkedList structure documentation for more info
     * @param free_on_delete If true, the elements will be also freed on deletion
    */
    static inline void arrlist_configure(ArrayList *list, bool free_on_delete){
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
    */
    extern int arrlist_indexof(ArrayList list, void *element);

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
    extern int arrlist_set_at(ArrayList *list, size_t index, void *element);

    /**
     * Replaces element with replacement in the list.
     * \return 1 if the operation is sucessful
    */
    extern int arrlist_set(ArrayList *list, void *element, void *replacement);

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
     * \return 1 if the operation is sucessful
    */
    extern int arrlist_remove_at(ArrayList *list, size_t index);

    /**
     * Removes the specified element
     * \return 1 if the operation is sucessful
    */
    extern int arrlist_remove(ArrayList *list, void *element);

    /**
     * 
    */
    extern void arrlist_free(ArrayList list);

    extern void arrlist_reset(ArrayList *list);

#endif