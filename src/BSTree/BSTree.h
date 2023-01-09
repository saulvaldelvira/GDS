/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 08-01-2023
 * Copyright: Saúl Valdelvira Iglesias (2023)
 * Licensed under the GNU GPL V.3. See /LICENSE file for more info
*/
#pragma once
#ifndef BSTREE_H
#define BSTREE_H

#include <stdbool.h>
#include <stddef.h>

#define REPEATED_ELEMENT -1
#define NON_EXISTING_ELEMENT -1
#define EMPTY_TREE -1

#include "../definitions.h"

typedef struct BSNode BSNode;

/**
     * @brief BSTree (Binary Search Tree) structure.
     * 
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
    int (*compare) (const void*,const void*);
    size_t n_elements;
    size_t data_size;
} BSTree;

/**
 * Returns an empty new BSTree
 * @param comparator function to compare two elements of the tree. 
 *  Must return -1 if element1 < element2, 1 if element1 > element2 and 0 if element1 == element2
*/
extern BSTree bst_init(size_t data_size, int (*cmp) (const void*, const void*));

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
extern void* bst_get(BSTree tree, void* element, void *dest);

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
extern void* bst_inorder(BSTree tree);

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