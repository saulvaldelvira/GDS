#pragma once
#ifndef BSTREE_H
#define BSTREE_H

#include <stdbool.h>
#include <stddef.h>

#define REPEATED_ELEMENT -1
#define NON_EXISTING_ELEMENT -1
#define EMPTY_TREE -1

// free_on_delete posible values
    #ifndef FREE_ON_DELETE
        #define FREE_ON_DELETE 1
    #endif

    #ifndef DONT_FREE_ON_DELETE
        #define DONT_FREE_ON_DELETE 0
    #endif

typedef struct BSNode {
    void *info;
    struct BSNode *right;
    struct BSNode *left;
    struct BSNode *father;
} BSNode;

typedef struct BSTree {
    BSNode *root;
    int (*compare) (void*,void*);
    size_t n_elements;
    bool free_on_delete; // Free the element when deleting it
} BSTree;

extern BSTree bst_init(int (*cmp) (void*,void*));

static inline void bst_configure(BSTree *tree, int free_on_delete){
    tree->free_on_delete = free_on_delete;
}

extern int bst_add(BSTree *tree, void *element);
extern int bst_remove(BSTree *tree, void *element);
extern void* bst_get(BSTree tree, void* element);
extern bool bst_exists(BSTree tree, void *element);
extern bool bst_isempty(BSTree tree);

extern void bst_free(BSTree tree);
extern void bst_reset(BSTree *tree);

extern void** bst_inorder(BSTree tree);
extern void** bst_preorder(BSTree tree);
extern void** bst_postorder(BSTree tree);


#endif