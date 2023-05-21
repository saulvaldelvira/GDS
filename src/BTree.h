/**
 *  Copyright (C) 2023 - Saúl Valdelvira
 *  License: BSD 3-Clause
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
void btree_set_comparator(BTree *tree, comparator_function_t cmp);

/**
 * Sets the destructor function of the tree
 * @param destructor the new destructor function. NULL means no destructor
*/
void dict_set_destructor(BTree *tree, destructor_function_t destructor);

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
