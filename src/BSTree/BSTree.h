#pragma once
#ifndef BSTREE_H
#define BSTREE_H

#include <stdbool.h>
#include <stddef.h>

#define REPEATED_ELEMENT -1
#define NON_EXISTING_ELEMENT -1
#define EMPTY_TREE -1

// free_on_delete posible values
#ifndef free_on_delete_defined
    typedef enum free_on_delete { FreeOnDelete=1, DontFreeOnDelete=0} free_on_delete_t;
    #define free_on_delete_defined
#endif

typedef struct BSNode BSNode;

/**
     * @brief BSTree (Binary Search Tree) structure.
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
     * \note bst_add(&list, &ptr[0]);
     * \note bst_add(&list, &ptr[1]);
     * 
     * \note bst_remove(&list, &ptr[0]);
     * 
     * \note ****************************************
     * 
     * \note In this last instruction, if free_on_delete is 1, the tree will free &ptr[0]. But ptr holds memory for 3 elements so 
     * &ptr[1] and &ptr[2] will also be freed. This will cause trouble if the tree tries to access those already fred directions in the future.
     * 
     * \note To avoid this, you can use the functions defined in "allocate.h", that allocate memory for the most common data types (alloc_int, alloc_char, alloc_float, etc)
     * 
     * \note 
     *  NOTE 2: The default value is 0 (don't free), so also keep in mind that unless you configure the tree, that memory is not being 
     *  freed on deletion.
     * */
typedef struct BSTree {
    BSNode *root;
    /**
    * Comparator function for 2 elements
    *   \attention 
    * This function is really important, because it's going to be used to compare two elements in this tree. It 
    * should receive two void pointers p1 and p2 and return 1 if p1 > p2, -1 if p1 < p2 or 0 if p1 == p2 
    *   \attention 
    * There are a few functions already implemented for the most common data types (int, char, char *, float, double). 
    * You can find them in the Comparators structure, defined in the "comparator.h" header file.
    *   \note 
    * Example of use: Comparators.integer (function to compare two void pointers representing int pointers).
    */
    int (*compare) (void*,void*);
    size_t n_elements;
    free_on_delete_t free_on_delete; // Free the element when deleting it
} BSTree;

/**
 * Returns an empty new BSTree
 * @param comparator function to compare two elements of the tree. 
 *  Must return -1 if element1 < element2, 1 if element1 > element2 and 0 if element1 == element2
*/
extern BSTree bst_init(int (*cmp) (void*,void*));

 /**
     * \brief 
     * Configures the behavior of the tree when deleting a node. 
     * \note
     *  See the BSTree structure documentation for more info
     * @param free_on_delete If true, the information of the nodes will be also freed on node deletion
    */
static inline void bst_configure(BSTree *tree, int free_on_delete){
    tree->free_on_delete = free_on_delete;
}

/**
 * @return 1 if it sucessfuly adds removes the element from the tree
*/
extern int bst_add(BSTree *tree, void *element);

/**
 * @return 1 if it sucessfuly removes the element from the tree
*/
extern int bst_remove(BSTree *tree, void *element);

/**
 * @return the element, if it can find it, or NULL if it does not exists in the tree
*/
extern void* bst_get(BSTree tree, void* element);

/**
 * @return true if the element exists in the tree
*/
extern bool bst_exists(BSTree tree, void *element);

/**
 * @return true if the tree is empty
*/
extern bool bst_isempty(BSTree tree);

/**
 * Frees the memory allocated for this tree
*/
extern void bst_free(BSTree tree);

/**
 * Frees the memory allocated for this tree and resets it to it's original state
*/
extern void bst_reset(BSTree *tree);

/**
 * Returns an array with all the elements of the tree traversed in-order.
 * @note the size of the array will obviously be the number of elements in the tree, so there is no need to return it.
*/
extern void** bst_inorder(BSTree tree);

/**
 * Returns an array with all the elements of the tree traversed pre-order.
 * @note the size of the array will obviously be the number of elements in the tree, so there is no need to return it.
*/
extern void** bst_preorder(BSTree tree);

/**
 * Returns an array with all the elements of the tree traversed post-order.
 * @note the size of the array will obviously be the number of elements in the tree, so there is no need to return it.
*/
extern void** bst_postorder(BSTree tree);


#endif