/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 30-12-2022
*/
#pragma once
#ifndef LINKED_LINKED_LIST_H
#define LINKED_LINKED_LIST_H

    #include <stddef.h>
    #include <stdbool.h>

    /**
     * Node of a Linked List
    */
    typedef struct LLNode {
        struct LLNode *next; // Pointer tto the next node
        struct LLNode *previous; // Pointer to the previous node
        void *info; // Element stored in this node
    } LLNode;

    #define FREE_ON_NODE_REMOVE 1
    #define DONT_FREE_ON_NODE_REMOVE 0
    #define FREE_ELEMENTS_ON_LIST_REMOVE 1
    #define DONT_FREE_ELEMENTS_ON_LIST_REMOVE 0

    /**
     * @brief Linked List structure.
     * 
     * \attention
     * To make this list "generic", nodes use void pointers to the elements they store.
     * When you delete a node, it is freed from memory. You can choose to also free the 
     * memory alocated for the element that node stores. The behaviour is defined by this two parameters.
     * 
     * @param free_on_node_delete If true, when deleting a node, the element it stores will also be freed.
     * @param free_on_list_delete If true, when deleting the list (by calling lnkd_list_free), all the elements it stores will also be freed.
     * 
     * \note
     * Sometimes you'll need the memory to be deleted, and sometimes that memory isn't allocated so you 
     * don't need to free those void pointers.
     * BE REALLY CAREFUL, because if you set those parameters to true and add elements to the list that don't need 
     * to be freed, you'll cause a segmentation fault.
     * \note
     *  The default value is 0 (don't free), so also keep in mind that unless you configure the list, that memory is not being 
     *  freed on deletion.
     * 
    */
    typedef struct LinkedList {
        LLNode *root;
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
        bool (*comp) (void*, void*);
        // Flags: Determine the behaviour when deleting a node
        bool free_elements_on_node_remove; // Free the element in the node when deleting it
        bool free_elements_on_list_remove; // Free the element in the node when freeing the List
    } LinkedList;
    
    /**
     * Initializes the linked list 
     * @param comp pointer to a function that receives two void pointers and returns 1 if the first one is greater than the 
     *              second, 0 if they are equal and -1 if the first one is smaller than the second one
    */
    extern LinkedList lnkd_list_init(bool (*comp) (void*, void*));

    /**
     * \brief 
     * Configures the behavior of the list when deleting a node. 
     * \note
     *  See the LinkedList structure documentation for more info
     * @param free_on_node_delete If true, the information of the nodes will be also freed on node deletion
     * @param free_on_list_delete If true, the elements in the list will also be freed along its nodes.
    */
    extern void lnkd_list_configure(LinkedList *list, bool free_on_node_delete, bool free_on_list_delete);

    /**
     * \brief
     * Adds the given element to the end of the list. The element must be a pointer to whatever 
     * data type you wish.
     * \attention  It is recommended to only store ONE data type per list, so when you retrait the elements from
     * the list, you know you can safely cast the pointers to the correct data types.
     * \attention But there's no restriction regarding 
     * issue, so technically you can store whatever you want here. This is the C programming language :p
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
     * Removes the element from the list
     * @return 1 if it deletes the element, -1 if the element of the list are NULL and 0 if 
     *          the element doesn't exist in the list
    */
    extern int lnkd_list_remove(LinkedList *list, void *element);

    /**
     * Frees the memory allocated to the list.
     * \note See the documentation of the LinkedList structure to learn about how 
     *       the deletion is handled regarding the elements stored in the nodes.
    */
    extern void lnkd_list_free(LinkedList list);
    

    

#endif
