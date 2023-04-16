/**
 *  Copyright (C) 2023 - Saúl Valdelvira
 *
 *  This program is free software. You can modify
 *  and/or redistribute it under the terms of the
 *  GNU General Public License version 2, or newer.
 *  See <https://www.gnu.org/licenses/>.
 *
 *  Email: saulvaldelvira@gmail.com
 *  Version: 17-01-2023
 */
#pragma once
#ifndef BSTREE_H
#define BSTREE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include "./Util/comparator.h"

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
BSTree* bst_init(size_t data_size, comparator_function_t cmp);

/**
 * Changes the comparator function of the tree
 * @param cmp the new comparator function
*/
void bst_configure(BSTree *tree, comparator_function_t cmp);

/**
 * @return 1 if the operation is successful
*/
int bst_add(BSTree *tree, void *element);

/**
 * Adds the first [array_length] elements of the array to the tree
*/
int bst_add_array(BSTree *tree, void *array, size_t array_length);

/**
 * @return 1 if the operation is successful
*/
int bst_remove(BSTree *tree, void *element);

/**
 * Removes from the tree the first [array_length] elements of the array
*/
int bst_remove_array(BSTree *tree, void *array, size_t array_length);

/**
 * @return the element, if it can find it, or NULL if it does not exists in the tree
*/
void* bst_get(BSTree *tree, void* element, void *dest);

/**
 * Copies the max value in the tree to dest
*/
void* bst_max(BSTree *tree, void *dest);

/**
 * Copies the min value in the tree to dest
*/
void* bst_min(BSTree *tree, void *dest);

/**
 * Copies to dest the max value in the tree, starting in [node]
*/
void* bst_max_from(BSTree *tree, void *element, void *dest);

/**
 * Copies to dest the min value in the tree, starting in [node]
*/
void* bst_min_from(BSTree *tree, void *element, void *dest);

/**
 * @return true if the element exists in the tree
*/
bool bst_exists(BSTree *tree, void *element);

/**
 * @return the number of elements in the tree
*/
size_t bst_size(BSTree *tree);

/**
 * @return true if the tree is empty
*/
bool bst_isempty(BSTree *tree);

/**
 * @return a new BSTree with the elements of the two given trees.
 * @note No particular order of elements is guaranteed.
 * @note ATTENTION: The trees must store the same data. At least, they must have
 * the same data size. Also, the comparator function of tree_1 will be taken,
 * because it is assumed that both trees have the same comparator functions.
*/
BSTree* bst_join(BSTree *tree_1, BSTree *tree_2);

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
void* bst_preorder(BSTree *tree);

/**
 * @return an array with all the elements of the tree traversed in-order.
 * @note the size of the array will naturally be the number of elements in the tree, so there is no need to return it.
*/
void* bst_inorder(BSTree *tree);

/**
 * @return an array with all the elements of the tree traversed post-order.
 * @note the size of the array will naturally be the number of elements in the tree, so there is no need to return it.
*/
void* bst_postorder(BSTree *tree);

#ifdef __cplusplus
}
#endif

#endif
