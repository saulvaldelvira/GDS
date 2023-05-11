/**
 *  Copyright (C) 2023 - Saúl Valdelvira
 *
 *  This library is free software. You can modify
 *  and/or redistribute it under the terms of the
 *  GNU Lesser General Public License version 2.1
 *  See <https://www.gnu.org/licenses/>
 *
 *  Email: saulvaldelvira@gmail.com
 */
#ifndef BTREE_H
#define BTREE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include "./Util/comparator.h"

typedef struct BTree BTree;

BTree* btree_init(size_t data_size, int K, comparator_function_t cmp);

/**
 * Changes the comparator function of the tree
 * @param cmp the new comparator function
*/
void btree_configure(BTree *tree, comparator_function_t cmp);

/**
 * Adds an element to the tree
 * Returns 1 on success
*/
int btree_add(BTree *tree, void *element);

/**
 * Adds [array_size] elements from [array] to the tree.
 * Returns 1 on success.
*/
int btree_add_array(BTree *tree, void *array, size_t array_size);

/**
 * Removes an element to the tree
 * Returns 1 on success
*/
int btree_remove(BTree *tree, void *element);

/**
 * Removes [array_size] elements from [array] to the tree.
 * Returns 1 on success.
*/
int btree_remove_array(BTree *tree, void *array, size_t array_size);

/**
 * Copies into dest the given element, if exists.
 * Else, returns NULL and no value is copied into dest.
*/
void* btree_get(BTree *tree, void *element, void *dest);

/**
 * Returns true if the element exists in the tree.
*/
bool btree_exists(BTree *tree, void *element);

/**
 * Frees the memory allocated for the tree.
*/
int btree_free(BTree *tree);

/**
 * Frees multiple trees at once.
 * @n number of pointers to free.
*/
void btree_free_all(unsigned int n, ...);

/**
 * Resets the tree to it's initial state.
 * This means, it clears all the data it stores, but unlike
 * the free function, leaves it ready to store new data.
*/
BTree* btree_reset(BTree *tree);

#ifdef __cplusplus
}
#endif

#endif
