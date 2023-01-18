/**
 *  Copyright (C) 2022-2023  Saúl Valdelvira Iglesias
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *  Email: saulvaldelvira@gmail.com
 *  Version: 17-01-2023
 */
#pragma once
#ifndef AVLTREE_H
#define AVLTREE_H

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
 * Frees the memory allocated for the tree
 * @return 1 if the operation is successful
*/
int avl_free(AVLTree *tree);

/***
 * Resets the tree to it's default state
 * @return the pointer. NULL if anything goes wrong.
*/
AVLTree* avl_reset(AVLTree *tree);

#endif