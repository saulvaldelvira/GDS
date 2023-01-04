/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 31-12-2022
*/
#pragma once
#ifndef LINKED_LINKED_LIST_H
#define LINKED_LINKED_LIST_H

    #include <stddef.h>
    #include <stdbool.h>

    // free_on_delete posible values
    #ifndef FREE_ON_DELETE
        #define FREE_ON_DELETE 1
    #endif

    #ifndef DONT_FREE_ON_DELETE
        #define DONT_FREE_ON_DELETE 0
    #endif

    /**
     * Node of a Linked List
    */
    typedef struct LLNode {
        struct LLNode *next; // Pointer to the next node
        struct LLNode *previous; // Pointer to the previous node
        void *info; // Element stored in this node
    } LLNode;

    /**
     * @brief Linked List structure.
     * 
     * \attention
     * To make this structure "generic", nodes use void pointers to the elements they store.
     * When you delete a node, it is freed from memory. You can choose to also free the 
     * memory alocated for the element the node stores. The behaviour is defined by this parameter.
     * 
     * @param free_on_delete If true, when deleting a node, the element it stores will also be freed.
     * 
     * \note
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
     * \note lnkd_list_append(&list, &ptr[0]);
     * \note lnkd_list_append(&list, &ptr[1]);
     * 
     * \note lnkd_list_remove(&list, &ptr[0]);
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
    typedef struct LinkedList {
        LLNode *head;
        LLNode *tail;
        size_t n_elements;
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
        // Flags: Determine the behaviour when deleting a node
        bool free_on_delete; // Free the element in the node when deleting it
    } LinkedList;
    
    /**
     * Initializes the linked list 
     * @param cmp pointer to a function that receives two void pointers and returns 1 if the first one is greater than the 
     *              second, 0 if they are equal and -1 if the first one is smaller than the second one
    */
    extern LinkedList lnkd_list_init(int (*cmp) (void*, void*));

    /**
     * \brief 
     * Configures the behavior of the list when deleting a node. 
     * \note
     *  See the LinkedList structure documentation for more info
     * @param free_on_delete If true, the information of the nodes will be also freed on node deletion
    */
    extern void lnkd_list_configure(LinkedList *list, bool free_on_delete);

    /**
     * \brief
     * Adds the given element to the end of the list. The element must be a pointer to whatever 
     * data type you wish.
     * \attention  It is recommended to only store ONE data type per list, so when you retrait the elements from
     * the list, you know you can safely cast the pointers to the correct data types.
     * \attention But there's no restriction regarding 
     * issue, so technically you can store whatever you want here. This is the C programming language :p
     * \return 1 if the operation is succesful
    */
    extern int lnkd_list_append(LinkedList *list, void *element);

    /**
     * Changes the given element in the list with the replacement, if it exists.
     * 
     * \return  1 if it's able to set the new element or 0 if element does not exists in the list        
    */
    extern int lnkd_list_set(LinkedList *list, void *element, void *replacement);

    /**
     * @return a pointer to the first element in the list that is equal to the parameter element.
    */
    extern void* lnkd_list_get(LinkedList list, void *element);

    /**
     * @return true if the element exists in the list
    */
    extern bool lnkd_list_exists(LinkedList list, void *element);
    
    /**
     * @return true if the list is empty
    */
    extern bool lnkd_list_isempty(LinkedList list);
    
    /**
     * Removes the element from the list
     * @return 1 if it deletes the element, -1 if the element of the list are NULL and 0 if 
     *          the element doesn't exist in the list
    */
    extern int lnkd_list_remove(LinkedList *list, void *element);

    /**
     * Frees the memory allocated on the list.
     * \note See the documentation of the LinkedList structure to learn about how 
     *       the deletion is handled regarding the elements stored in the nodes.
    */
    extern void lnkd_list_free(LinkedList list);
    
    extern void lnkd_list_reset(LinkedList *list);    

#endif
