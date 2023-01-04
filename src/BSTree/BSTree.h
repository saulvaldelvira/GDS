#pragma once
#ifndef BSTREE_H
#define BSTREE_H

#include <stdbool.h>

typedef struct BSNode {
    void *info;
    BSNode *right;
    BSNode *left;
} BSNode;

typedef struct BSTree {
    BSNode *root;
    int (*compare) (void*,void*);
} BSTree;

extern BSTree bst_init(int (*cmp) (void*,void*));
extern int bst_add(BSTree *tree, void *element);
extern int bst_remove(BSTree *tree, void *element);
extern bool bst_exists(BSTree tree, void *element);


#endif