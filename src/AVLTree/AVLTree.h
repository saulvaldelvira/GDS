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
#ifndef AVLTREE_H
#define AVLTREE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include "../Util/comparator.h"

typedef struct _AVLTree AVLTree;

/**
 * Initializes an AVLTree.
 * @param data_size the size of the data type being stored
 * @param cmp the comprator function for two elements in the tree
*/
AVLTree* avl_init(size_t data_size, comparator_function_t cmp);

/**
 * Changes the comparator function of the tree
 * @param cmp the new comparator function
*/
void avl_configure(AVLTree *tree, comparator_function_t cmp);

/**
 * Adds the elements to the tree
 * @return 1 if the operation is successful
*/
int avl_add(AVLTree *tree, void *element);

/**
 * Adds [array_length] elements from array
 * @return 1 if the operation is successful
*/
int avl_add_array(AVLTree *tree, void *array, size_t array_length);

/**
 * Removes the elements from the tree
 * @return 1 if the operation is successful
*/
int avl_remove(AVLTree *tree, void *element);

/**
 * Removes [array_length] elements from array
 * @return 1 if the operation is successful
*/
int avl_remove_array(AVLTree *tree, void *array, size_t array_length);

/**
 * @return true if the elements exists in the tree
*/
bool avl_exists(AVLTree *tree, void *element);

/**
 * Copies the element into dest, if existing.
 * @return dest if it succesfuly finds the element
*/
void* avl_get(AVLTree *tree, void *element, void *dest);

/**
 * @return the number of elements in the tree
*/
size_t avl_size(AVLTree *tree);

/**
 * @return the height of the tree
*/
int avl_height(AVLTree *tree);

/**
 * @return true if the tree is empty
*/
bool avl_isempty(AVLTree *tree);

/**
 * @return an array with the elements in the tree traversed Pre Order
 * @note Remember to free the returned pointer when finished
*/
void* avl_preorder(AVLTree *tree);

/**
 * @return an array with the elements in the tree traversed In Order
 * @note Remember to free the returned pointer when finished
*/
void* avl_inorder(AVLTree *tree);

/**
 * @return an array with the elements in the tree traversed Post Order
 * @note Remember to free the returned pointer when finished
*/
void* avl_postorder(AVLTree *tree);

/**
 * @return a new AVLTree with the elements of the two given trees.
 * @note No particular order of elements is guaranteed.
 * @note ATTENTION: The trees must store the same data. At least, they must have
 * the same data size. Also, the comparator function of tree_1 will be taken,
 * because it is assumed that both trees have the same comparator functions.
*/
AVLTree* avl_join(AVLTree *tree_1, AVLTree *tree_2);

/**
 * Copies the max value in the tree to dest
*/
void* avl_max(AVLTree *tree, void *dest);

/**
 * Copies the min value in the tree to dest
*/
void* avl_min(AVLTree *tree, void *dest);

/**
 * Copies to dest the max value in the tree, starting in [node]
*/
void* avl_max_from(AVLTree *tree, void *element, void *dest);

/**
 * Copies to dest the min value in the tree, starting in [node]
*/
void* avl_min_from(AVLTree *tree, void *element, void *dest);

/**
 * Frees the memory allocated for the tree
 * @return 1 if the operation is successful
*/
int avl_free(AVLTree *tree);

/***
 * Resets the tree to it's default state
 * @return the pointer. NULL if anything goes wrong.
*/
AVLTree* avl_reset(AVLTree *tree);

#ifdef __cplusplus
}
#endif

#endif // AVLTREE_H
