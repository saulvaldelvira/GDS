#pragma once
#ifndef AVLTREE_H
#define AVLTREE_H

#include <stddef.h>
#include <stdbool.h>
#include "../Util/comparator.h"

typedef struct _AVLTree AVLTree;

AVLTree* avl_init(size_t data_size, comparator_function_t cmp);
int avl_add(AVLTree *tree, void *element);
int avl_remove(AVLTree *tree, void *element);
bool avl_exists(AVLTree *tree, void *element);
void* avl_get(AVLTree *tree, void *element, void *dest);
size_t avl_size(AVLTree *tree);
bool avl_isempty(AVLTree *tree);

void* avl_preorder(AVLTree *tree);
void* avl_inorder(AVLTree *tree);
void* avl_postorder(AVLTree *tree);

int avl_free(AVLTree *tree);
AVLTree* avl_reset(AVLTree *tree);

#endif