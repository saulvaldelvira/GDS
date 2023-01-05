#include "BSTree.h"
#include "../Util/checks.h"

#include <stdlib.h>
#include <stdio.h>

BSTree bst_init(int (*cmp) (void*,void*)){
    return (BSTree) {
        .root = NULL,
        .compare = cmp,
        .n_elements = 0,
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
    }else if (c < 0){
        ret = add_rec(node->left, element, cmp);
        node->left = ret.node;
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
    if (ret.status == 1){
        tree->n_elements++;
    }
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
        if(free_element){
            free(node->info);
        }

        if (node->left == NULL){
            node = node->right;
        } else if(node->right == NULL) {
            node = node->left;
        }else {
            BSNode *aux = get_max(node->left);
            if(aux != NULL){
                aux->father->right = NULL;
                node->info = aux->info;
                free(aux);
            }else if ((aux = get_min(node->right)) != NULL){
                aux->father->left = NULL;
                node->info = aux->info;
                free(aux);
            }else {
                free(node);
                node = NULL;
            }
            free(aux);
            ret.status = 1;
        }
    }
    ret.node = node;
    return ret;
}

int bst_remove(BSTree *tree, void *element){
    CHECK_NULL(tree == NULL || element == NULL, bst_remove)
    struct remove_rec_ret ret = remove_rec(tree->root, element, tree->compare, tree->free_on_delete);
    tree->root = ret.node;
    if (ret.status == 1){
        tree->n_elements--;
    }
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

static void free_rec(BSNode *node, bool free_element){
    if(node == NULL){
        return;
    }
    if(free_element){
        free(node->info);
    }
    free_rec(node->left, free_element);
    free_rec(node->right, free_element);
    free(node);
}

void bst_free(BSTree tree){
    free_rec(tree.root, tree.free_on_delete);
}

void bst_reset(BSTree *tree){
    free_rec(tree->root, tree->free_on_delete);
    tree->root = NULL;
    tree->n_elements = 0;
}

#define INORDER_ERROR (struct in_order_ret) {.data = NULL, .data_size = 0, .status = -1}

static struct in_order_ret {
    void** data;
    size_t data_size;
    int status;
} inorder_rec(BSNode *node){
    if(node == NULL){
        return (struct in_order_ret) {
            .data = NULL,
            .data_size = 0,
            .status = 1
        };
    }

    struct in_order_ret left = inorder_rec(node->left);
    struct in_order_ret right = inorder_rec(node->right);
    
    if(left.status != 1 || right.status != 1){
        return INORDER_ERROR;
    }

    struct in_order_ret result;
    result.data_size = left.data_size + right.data_size + 1;
    result.data = calloc(result.data_size, sizeof(void*));
    CHECK_MEMORY(result.data, bst_inorder, INORDER_ERROR)
    result.status = 1;

    size_t index = 0;

    for (int i = 0; i < left.data_size; i++, index++){
        result.data[index] = left.data[i];
    }

    result.data[index++] = node->info;

    for (int i = 0; i < right.data_size; i++, index++){
        result.data[index] = right.data[i];
    }

    free(left.data);
    free(right.data);

    return result;
}


void** bst_inorder(BSTree *tree){
    struct in_order_ret result = inorder_rec(tree->root);
    if (result.status != 1){
        return NULL;
    }
    return result.data;

}