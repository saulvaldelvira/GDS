/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 11-01-2023
 * Copyright (C) 2022-2023 Saúl Valdelvira
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

/**
 * @brief BSTree (Binary Search Tree) structure.
 */
typedef struct _BSTree BSTree;

/**
 * Returns an empty new BSTree
 * @param data_size size of the data stored.
 * @param comparator function to compare two elements of the tree. 
 *  Must return -1 if element1 < element2, 1 if element1 > element2 and 0 if element1 == element2
*/
BSTree* bst_init(size_t data_size, int (*cmp) (const void*, const void*));

/**
 * @return 1 if the operation is successful
*/
int bst_add(BSTree *tree, void *element);

/**
 * @return 1 if the operation is successful
*/
int bst_remove(BSTree *tree, void *element);

/**
 * @return the element, if it can find it, or NULL if it does not exists in the tree
*/
void* bst_get(BSTree *tree, void* element, void *dest);

/**
 * @return true if the element exists in the tree
*/
bool bst_exists(BSTree *tree, void *element);

/**
 * @return the number of elements in the tree
*/
size_t bst_n_elements(BSTree *tree);

/**
 * @return true if the tree is empty
*/
bool bst_isempty(BSTree *tree);

/**
 * Frees the memory allocated for this tree
 * @return 1 if the operation is successful
*/
int bst_free(BSTree *tree);

/**
 * Frees the memory allocated for this tree and resets it to it's original state
 * @return the same pointer if sucess, NULL if the operation fails
*/
BSTree* bst_reset(BSTree *tree);

/**
 * @return an array with all the elements of the tree traversed pre-order.
 * @note the size of the array will naturally be the number of elements in the tree, so there is no need to return it.
*/
void** bst_preorder(BSTree *tree);

/**
 * @return an array with all the elements of the tree traversed in-order.
 * @note the size of the array will naturally be the number of elements in the tree, so there is no need to return it.
*/
void* bst_inorder(BSTree *tree);

/**
 * @return an array with all the elements of the tree traversed post-order.
 * @note the size of the array will naturally be the number of elements in the tree, so there is no need to return it.
*/
void** bst_postorder(BSTree *tree);


#endif