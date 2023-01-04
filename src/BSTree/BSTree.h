#pragma once
#ifndef BSTREE_H
#define BSTREE_H

#include <stdbool.h>

#define REPEATED_ELEMENT -1
#define NON_EXISTING_ELEMENT -1

typedef struct BSNode {
    void *info;
    struct BSNode *right;
    struct BSNode *left;
    struct BSNode *father;
} BSNode;

typedef struct BSTree {
    BSNode *root;
    int (*compare) (void*,void*);
    bool free_on_delete; // Free the element when deleting it
} BSTree;

extern BSTree bst_init(int (*cmp) (void*,void*));
extern void bst_configure(BSTree *tree, int free_on_delete);
extern int bst_add(BSTree *tree, void *element);
extern int bst_remove(BSTree *tree, void *element);
extern bool bst_exists(BSTree tree, void *element);


#endif