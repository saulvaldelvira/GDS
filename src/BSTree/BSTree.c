#include "BSTree.h"
#include "../Util/checks.h"

#include <stdlib.h>
#include <stdio.h>

BSTree bst_init(bool (*cmp) (void*,void*)){
    return (BSTree) {
        .root = NULL,
        .compare = cmp
    };
}

static BSNode* bst_add_rec(BSNode *node, void *element, bool (*cmp) (void*,void*)){

    
}

int bst_add(BSTree *tree, void *element){
    CHECK_NULL(tree == NULL || element == NULL, bst_add)

}

int bst_remove(BSTree *tree, void *element);

bool bst_exists(BSTree tree, void *element);