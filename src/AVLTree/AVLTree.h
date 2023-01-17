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