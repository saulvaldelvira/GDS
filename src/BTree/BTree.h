/**
 *  Copyright (C) 2023 - Saúl Valdelvira
 *
 *  This program is free software. You can modify
 *  and/or redistribute it under the terms of the GNU
 *  General Public License version 3, or any later version.
 *  See <https://www.gnu.org/licenses/>.
 *
 *  Email: saulvaldelvira@gmail.com
 *  Version: 29-01-2023
 */
#ifndef BTREE_H
#define BTREE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include "../Util/comparator.h"

typedef struct _BTree BTree;

BTree* btree_init(size_t data_size, int K, comparator_function_t cmp);

/**
 * Changes the comparator function of the tree
 * @param cmp the new comparator function
*/
void btree_configure(BTree *tree, comparator_function_t cmp);

int btree_add(BTree *tree, void *element);
int btree_remove(BTree *tree, void *element);
void* btree_get(BTree *tree, void *element, void *dest);
bool btree_exists(BTree *tree, void *element);

/*typedef void (*print_function_t)(void *);
void btree_print(FILE *output, BTree *tree, print_function_t print_function);*/

int btree_free(BTree *tree);
BTree* btree_reset(BTree *tree);

#ifdef __cplusplus
}
#endif

#endif
