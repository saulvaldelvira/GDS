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
 *  Version: 29-01-2023
 */
#ifndef BTREE_H
#define BTREE_H

#include <stddef.h>
#include <stdbool.h>
#include "../Util/comparator.h"

typedef struct _BTree BTree;

BTree* btree_init(size_t data_size, int K, comparator_function_t cmp);

int btree_add(BTree *tree, void *element);
int btree_remove(BTree *tree, void *element);
void* btree_get(BTree *tree, void *element, void *dest);
bool btree_exists(BTree *tree, void *element);

/*typedef void (*print_function_t)(void *);
void btree_print(FILE *output, BTree *tree, print_function_t print_function);*/

int btree_free(BTree *tree);
BTree* btree_reset(BTree *tree);

#endif