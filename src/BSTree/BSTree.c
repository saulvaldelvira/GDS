#include "BSTree.h"
#include "../Util/checks.h"

#include <stdlib.h>
#include <stdio.h>

BSTree bst_init(int (*cmp) (void*,void*)){
    return (BSTree) {
        .root = NULL,
        .compare = cmp,
        .free_on_delete = 0
    };
}

static BSNode* init_node(void *info){
    BSNode *node = malloc(sizeof(BSNode));
    CHECK_MEMORY(node, init_node, NULL)
    node->info = info;
    node->left = NULL;
    node->right = NULL;
    node->father = NULL;
    return node;
}

void bst_configure(BSTree *tree, int free_on_delete){
    tree->free_on_delete = free_on_delete;
}

static BSNode* add_rec(BSNode *node, void *element, int (*cmp) (void*,void*), int *return_status){
    if (node == NULL){
        BSNode *aux = init_node(element);
        if (!aux){
            *return_status = ALLOCATION_ERROR;
        } else {
            *return_status = 1;
        }
        return aux;
    }

    int c = (*cmp) (element, node->info);
    if (c > 0){ // The element is higher than the node's info
        node->right = add_rec(node->right, element, cmp, return_status);
        node->right->father = node;
    }else if (c < 0){
        node->left = add_rec(node->left, element, cmp, return_status);
        node->left->father = node;
    }else {
        *return_status = REPEATED_ELEMENT;
    }
    return node;
}

int bst_add(BSTree *tree, void *element){
    CHECK_NULL(tree == NULL || element == NULL, bst_add)
    int return_status;
    tree->root = add_rec(tree->root, element, tree->compare, &return_status);
    return return_status;
}

static BSNode* get_max(BSNode *node){
    if (node == NULL){
        return NULL;
    }
    while (node->right != NULL){
        node = node->right;
    }
    return node;
}

static BSNode* get_min(BSNode *node){
    if (node == NULL){
        return NULL;
    }
    while (node->left != NULL){
        node = node->left;
    }
    return node;
}

static inline void free_node(BSNode *node, bool free_element){
    if (free_element){
        free(node->info);
    }
    free(node);
}

static BSNode* remove_rec(BSNode *node, void *element, int (*cmp) (void*,void*), bool free_element, int *return_status){
    if (node == NULL){
        *return_status = NON_EXISTING_ELEMENT;
        return NULL;
    }

    int c = (*cmp) (element, node->info);
    if (c > 0){ // The element is higher than the node's info
        node->right = remove_rec(node->right, element, cmp, free_element, return_status);
    }else if (c < 0){
        node->left = remove_rec(node->left, element, cmp, free_element, return_status);
    }else {
        BSNode *aux = get_max(node->left);
        if(aux != NULL){
            free_node(aux->father->right, free_element);
            aux->father->right = NULL;
            node->info = aux->info;
        }else if ((aux = get_min(node->right)) != NULL){
            free_node(aux->father->left, free_element);
            aux->father->left = NULL;
            node->info = aux->info;
        }else {
            free_node(node, free_element);
            node = NULL;
        }
        *return_status = 1;
    }
    return node;
}

int bst_remove(BSTree *tree, void *element){
    int return_status;
    CHECK_NULL(tree == NULL || element == NULL, bst_remove)
    tree->root = remove_rec(tree->root, element, tree->compare, tree->free_on_delete, &return_status);
    return return_status; 
}

static bool exists_rec(BSNode *node, void *element, int (*cmp) (void*,void*)){
    if (node == NULL){
        return false;
    }
    int c = (*cmp) (element, node->info);
    if (c > 0){
        return exists_rec(node->right, element, cmp);
    }else if(c < 0){
        return exists_rec(node->left, element, cmp);
    }else{
        return true;
    }
}

bool bst_exists(BSTree tree, void *element){
    CHECK_NULL(element == NULL, bst_exists)
    return exists_rec(tree.root, element, tree.compare);
}