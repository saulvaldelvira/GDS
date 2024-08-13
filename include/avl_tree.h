/*
 * avl_tree.h - avl_t definition.
 * Author: Saúl Valdelvira (2023)
 */
#pragma once
#ifndef AVLTREE_H
#define AVLTREE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include "compare.h"

typedef struct avl_t avl_t;

/**
 * Initializes an avl_t.
 * @param data_size the size of the data type being stored
 * @param cmp the comprator function for two elements in the tree
*/
avl_t* avl_init(size_t data_size, comparator_function_t cmp);

/**
 * Changes the comparator function of the tree
 * @param cmp the new comparator function
*/
void avl_set_comparator(avl_t *tree, comparator_function_t cmp);

/**
 * Sets the destructor function of the tree
 * @param destructor the new destructor function. NULL means no destructor
*/
void avl_set_destructor(avl_t *tree, destructor_function_t destructor);

/**
 * Adds the elements to the tree
 * @return 1 if the operation is successful
*/
int avl_add(avl_t *tree, void *element);

/**
 * Adds [array_length] elements from array
 * @return 1 if the operation is successful
*/
int avl_add_array(avl_t *tree, void *array, size_t array_length);

/**
 * Removes the elements from the tree
 * @return 1 if the operation is successful
*/
int avl_remove(avl_t *tree, void *element);

/**
 * Removes [array_length] elements from array
 * @return 1 if the operation is successful
*/
int avl_remove_array(avl_t *tree, void *array, size_t array_length);

/**
 * @return true if the elements exists in the tree
*/
bool avl_exists(const avl_t *tree, void *element);

/**
 * Copies the element into dest, if existing.
 * @return dest if it succesfuly finds the element
*/
void* avl_get(const avl_t *tree, void *element, void *dest);

/**
 * @return the number of elements in the tree
*/
size_t avl_size(const avl_t *tree);

/**
 * @return the height of the tree
*/
int avl_height(const avl_t *tree);

/**
 * @return true if the tree is empty
*/
bool avl_isempty(const avl_t *tree);

/**
 * @return an array with the elements in the tree traversed Pre Order
 * @note Remember to free the returned pointer when finished
*/
void* avl_preorder(const avl_t *tree);

/**
 * @return an array with the elements in the tree traversed In Order
 * @note Remember to free the returned pointer when finished
*/
void* avl_inorder(const avl_t *tree);

/**
 * @return an array with the elements in the tree traversed Post Order
 * @note Remember to free the returned pointer when finished
*/
void* avl_postorder(const avl_t *tree);

/**
 * @return a new avl_t with the elements of the two given trees.
 * @note No particular order of elements is guaranteed.
 * @note ATTENTION: The trees must store the same data. At least, they must have
 * the same data size. Also, the comparator function of tree_1 will be taken,
 * because it is assumed that both trees have the same comparator functions.
*/
avl_t* avl_join(const avl_t *tree_1, const avl_t *tree_2);

/**
 * Get the max value inside the tree
 * @param[out] dest address to copy the element into
 * @return dest pointer, or NULL if error
*/
void* avl_max(const avl_t *tree, void *dest);

/**
 * Get the min value inside the tree
 * @param[out] dest address to copy the element into
 * @return dest pointer, or NULL if error
*/
void* avl_min(const avl_t *tree, void *dest);

/**
 * Get the max value inside the tree, starting in element
 * @param element element to start the search from
 * @param[out] dest address to copy the element into
 * @return dest pointer, or NULL if error
*/
void* avl_max_from(const avl_t *tree, void *element, void *dest);

/**
 * Get the min value inside the tree, starting in element
 * @param element element to start the search from
 * @param[out] dest address to copy the element into
 * @return dest pointer, or NULL if error
*/
void* avl_min_from(const avl_t *tree, void *element, void *dest);

void avl_free(avl_t *t, ...);

/**
 * Frees all the given trees.
 */
#define avl_free(...) avl_free(__VA_ARGS__, 0L)

/**
 * Removes all elements from the tree
*/
void avl_clear(avl_t *tree);

#ifdef __cplusplus
}
#endif

#endif // AVLTREE_H
