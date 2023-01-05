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
                if(aux != node->left){
                    aux->father->right = NULL;
                }else{
                    node->left = aux->left;
                }
                node->info = aux->info;
            }else {
                aux = get_min(node->right);
                if(aux != node->right){
                    aux->father->left = NULL;
                }else{
                    node->right = aux->right;
                }
                node->info = aux->info;
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

#define ORDERS_ERROR (struct orders_ret) {.data = NULL, .data_size = 0, .status = -1}

struct orders_ret {
    void** data;
    size_t data_size;
    int status;
};

enum Traversal {
    IN_ORDER, PRE_ORDER, POST_ORDER
};

static struct orders_ret order_rec(BSNode *node, enum Traversal order){
    if(node == NULL){
        return (struct orders_ret) {
            .data = NULL,
            .data_size = 0,
            .status = 1
        };
    }

    struct orders_ret left = order_rec(node->left, order);
    struct orders_ret right = order_rec(node->right, order);
    
    if(left.status != 1 || right.status != 1){
        return ORDERS_ERROR;
    }

    struct orders_ret result;
    result.data_size = left.data_size + right.data_size + 1;
    result.data = calloc(result.data_size, sizeof(void*));
    CHECK_MEMORY(result.data, bst_inorder, ORDERS_ERROR)
    result.status = 1;

    size_t index = 0;

    if(order == PRE_ORDER){
        result.data[index++] = node->info;
    }

    for (int i = 0; i < left.data_size; i++, index++){
        result.data[index] = left.data[i];
    }

    if(order == IN_ORDER){
        result.data[index++] = node->info;
    }

    for (int i = 0; i < right.data_size; i++, index++){
        result.data[index] = right.data[i];
    }

    if(order == POST_ORDER){
        result.data[index++] = node->info;
    }

    free(left.data);
    free(right.data);

    return result;
}

void** bst_inorder(BSTree tree){
    struct orders_ret result = order_rec(tree.root, IN_ORDER);
    if (result.status != 1){
        return NULL;
    }
    return result.data;
}

void** bst_preorder(BSTree tree){
    struct orders_ret result = order_rec(tree.root, PRE_ORDER);
    if (result.status != 1){
        return NULL;
    }
    return result.data;
}

void** bst_postorder(BSTree tree){
    struct orders_ret result = order_rec(tree.root, POST_ORDER);
    if (result.status != 1){
        return NULL;
    }
    return result.data;
}