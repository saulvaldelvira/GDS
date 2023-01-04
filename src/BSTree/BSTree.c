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

static struct add_rec_ret
{
    BSNode* node;
    int status;
    bool last_op_was_add;
} add_rec(BSNode *node, void *element, int (*cmp) (void*,void*)){
    if (node == NULL){
        BSNode *aux = init_node(element);
        if (!aux){
            return (struct add_rec_ret) {aux, ALLOCATION_ERROR, false};
        } else {
            return (struct add_rec_ret) {aux, 1, true};
        }
    }
    struct add_rec_ret ret;
    int c = (*cmp) (element, node->info);
    if (c > 0){ // The element is higher than the node's info
        ret = add_rec(node->right, element, cmp);
        node->right = ret.node;

        if(ret.last_op_was_add){
            printf("ADD %d in node %d's right\n", * (int*) element,* (int*) node->info);
        }

    }else if (c < 0){
        ret = add_rec(node->left, element, cmp);
        node->left = ret.node;
        if(ret.last_op_was_add){
            printf("ADD %d in node %d's left\n", * (int*) element,* (int*) node->info);
        }
    }else {
        return (struct add_rec_ret) {node, REPEATED_ELEMENT, false};
    }

    if(ret.last_op_was_add){
        ret.node->father = node;
        ret.last_op_was_add = false;
    }
    ret.node = node;
    return ret;
}

int bst_add(BSTree *tree, void *element){
    CHECK_NULL(tree == NULL || element == NULL, bst_add)
    int return_status;
    struct add_rec_ret ret = add_rec(tree->root, element, tree->compare);
    tree->root = ret.node;
    return ret.status;
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

static struct remove_rec_ret {
    BSNode* node;
    int status;
} remove_rec(BSNode *node, void *element, int (*cmp) (void*,void*), bool free_element){
    if (node == NULL){
        return (struct remove_rec_ret){NULL, NON_EXISTING_ELEMENT};
    }

    int c = (*cmp) (element, node->info);
    struct remove_rec_ret ret;
    if (c > 0){ // The element is higher than the node's info
        ret = remove_rec(node->right, element, cmp, free_element);
        node->right = ret.node;
    }else if (c < 0){
        ret = remove_rec(node->left, element, cmp, free_element);
        node->left = ret.node;
    }else {
        
        printf("Deleting %d: ", * (int*) element);
        if(free_element){
            free(node->info);
        }

        BSNode *aux = get_max(node->left);
        if(aux != NULL){
            printf("A");
            aux->father->right = NULL;
            node->info = aux->info;
            free(aux);
        }else if ((aux = get_min(node->right)) != NULL){
            printf("B");
            aux->father->left = NULL;
            node->info = aux->info;
            free(aux);
        }else {
            printf("C");
            free(node);
            node = NULL;
        }
        free(aux);
        printf("\n");
        ret.status = 1;
    }
    ret.node = node;
    return ret;
}

int bst_remove(BSTree *tree, void *element){
    CHECK_NULL(tree == NULL || element == NULL, bst_remove)
    struct remove_rec_ret ret = remove_rec(tree->root, element, tree->compare, tree->free_on_delete);
    tree->root = ret.node;
    return ret.status; 
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