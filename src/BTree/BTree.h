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

int btree_free(BTree *tree);
BTree* btree_reset(BTree *tree);

#endif