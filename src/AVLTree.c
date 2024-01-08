/*
 * AVLTree.c - AVLTree implementation.
 * Author: Saúl Valdelvira (2023)
 */
#include "AVLTree.h"
#include "./util/error.h"
#include "./util/definitions.h" // void_offset macro and byte_t typedef
#include <stdlib.h> // malloc
#include <string.h> // memcpy
#include <stdio.h> // fprintf
#include <stdarg.h>
#include <assert.h>

#define MAX_DISBALANCE 2

typedef struct AVLNode {
        struct AVLNode *left;
        struct AVLNode *right;
        int height;
        byte info[];
} AVLNode;

struct AVLTree {
        AVLNode *root;                          ///< Root node of the tree
        size_t data_size;                       ///< Size (in bytes) of the data type being stored
        size_t n_elements;                      ///< Number of elements in the tree
        comparator_function_t compare;          ///< Comparator function pointer
        destructor_function_t destructor;       ///< Destructor function pointer
};

//// INITIALIZE ////////////////////////////////////////////////////////////////

static AVLNode* init_node(void *element, size_t data_size){
        AVLNode *node = malloc(offsetof(AVLNode, info) + data_size);
        if (!node) return NULL;
        node->left = NULL;
        node->right = NULL;
        node->height = 0;
        memcpy(node->info, element, data_size);
        return node;
}

void avl_set_comparator(AVLTree *tree, comparator_function_t cmp){
        if (tree && cmp)
                tree->compare = cmp;
}

void avl_set_destructor(AVLTree *tree, destructor_function_t destructor){
        if (tree)
                tree->destructor = destructor;
}

AVLTree* avl_init(size_t data_size, comparator_function_t cmp){
        assert(cmp && data_size > 0);
        AVLTree *tree = malloc(sizeof(*tree));
        if (!tree) return NULL;
        tree->compare = cmp;
        tree->destructor = NULL;
        tree->data_size = data_size;
        tree->n_elements = 0;
        tree->root = NULL;
        return tree;
}

///////////////////////////////////////////////////////////////////////////////

//// NODE UPDATE FUNCTIONS /////////////////////////////////////////////////////

static inline int max_int(int a, int b) { return a >= b ? a : b; }

/**
 * Updates the node's height.
 * An AVL Node's heigth is the highest of it's son's heights plus one.
 * If a son is NULL, the other son's height is used.
 * If both are NULL this means the current node is a leaf of the three, thus it's height is set to 0.
*/
static void node_update_height(AVLNode *node){
        if (!node)
                return;
        if (node->left == NULL && node->right == NULL)
                node->height = 0;
        else if (node->left == NULL)
                node->height = node->right->height + 1;
        else if (node->right == NULL)
                node->height = node->left->height + 1;
        else
                node->height = max_int(node->left->height, node->right->height) + 1;
}

static int node_bf(AVLNode *node){
        int l_height = node->left  != NULL ? node->left->height  : -1;
        int r_height = node->right != NULL ? node->right->height : -1;
        return r_height - l_height;
}

static AVLNode* single_right_rotation(AVLNode *node){
        AVLNode *aux = node->right;
        node->right = aux->left;
        aux->left = node;
        node_update_height(node);
        return aux;
}

static AVLNode* single_left_rotation(AVLNode *node){
        AVLNode *aux = node->left;
        node->left = aux->right;
        aux->right = node;
        node_update_height(node);
        return aux;
}

static AVLNode* double_right_rotation(AVLNode *node){
        AVLNode *aux_1 = node->right;
        AVLNode *aux_2 = aux_1->left;
        node->right = aux_2->left;
        aux_1->left = aux_2->right;
        aux_2->right = aux_1;
        aux_2->left = node;
        node_update_height(aux_1);
        node_update_height(node);
        return aux_2;
}

static AVLNode* double_left_rotation(AVLNode *node){
        AVLNode *aux_1 = node->left;
        AVLNode *aux_2 = aux_1->right;
        node->left = aux_2->right;
        aux_1->right = aux_2->left;
        aux_2->left = aux_1;
        aux_2->right = node;
        node_update_height(aux_1);
        node_update_height(node);
        return aux_2;

}

static AVLNode* update_bf(AVLNode *node){
        if (!node)
                return NULL;
        node_update_height(node);
        int bf = node_bf(node);
        if (bf <= -MAX_DISBALANCE){
                int left_bf = node_bf(node->left);
                if (left_bf <= 0)
                        node = single_left_rotation(node);
                else
                        node = double_left_rotation(node);
                node_update_height(node);
        }else if (bf >= MAX_DISBALANCE){
                int right_bf = node_bf(node->right);
                if (right_bf >= 0)
                        node = single_right_rotation(node);
                else
                        node = double_right_rotation(node);
                node_update_height(node);
        }
        return node;
}

///////////////////////////////////////////////////////////////////////////////

//// ADD ///////////////////////////////////////////////////////////////////////

struct add_rec_ret{
        AVLNode* node;
        int status;
};

/**
 * Adds the element to the node.
 * 1) Compares the element with the node's info.
 *      If higher: continue with the right son
 *      If lower: continue with the left son
 *      Else: repeated element!
 * 2) If the node is null, we reached a leaf node, create a new node
 *    to store the element and return it.
 * 3) When the recursive call returns, update the son (left or right) to
 *    point to the returned node. After case 2 happens, this means updating
 *    a NULL pointer to now point to the newly created node.
*/
static struct add_rec_ret add_rec(AVLNode *node, void *element, comparator_function_t cmp, size_t size){
        if (node == NULL){
                AVLNode *aux = init_node(element, size);
                return (struct add_rec_ret){aux, aux ? SUCCESS : ERROR};
        }
        struct add_rec_ret ret;
        int c = (*cmp) (element, node->info);
        if (c > 0){
                ret = add_rec(node->right, element, cmp, size);
                node->right = ret.node;
        }else if (c < 0){
                ret = add_rec(node->left, element, cmp, size);
                node->left = ret.node;
        }else{
                return (struct add_rec_ret){node, REPEATED_ELEMENT_ERROR};
        }
        node = update_bf(node);
        ret.node = node;
        return ret;
}

int avl_add(AVLTree *tree, void *element){
        assert(tree && element);
        struct add_rec_ret ret = add_rec(tree->root, element, tree->compare, tree->data_size);
        if (ret.status != SUCCESS)
                return ret.status;
        tree->root = ret.node;
        tree->n_elements++;
        return SUCCESS;
}

int avl_add_array(AVLTree *tree, void *array, size_t array_length){
        assert(tree && array);
        while (array_length-- > 0){
                int status = avl_add(tree, array);
                if (status != SUCCESS){
                        return status;
                }
                array = void_offset(array, tree->data_size);
        }
        return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

/// MAX-MIN ///////////////////////////////////////////////////////////////////

static AVLNode* get_max(AVLNode *node){
        if (node == NULL)
                return NULL;
        while (node->right != NULL)
                node = node->right;
        return node;
}

static AVLNode* get_min(AVLNode *node){
        if (node == NULL)
                return NULL;
        while (node->left != NULL)
                node = node->left;
               return node;
}

///////////////////////////////////////////////////////////////////////////////

//// REMOVE ////////////////////////////////////////////////////////////////////

struct remove_rec_ret {
        AVLNode* node;
        int status;
};

/**
 * This fucntion works like the add.
 * 1) Compares the element with the node's info.
 *    If higher: continue with the right son
 *    If lower: continue with the left son
 *    Else: We found the element to delete.
 *        If left son exists: substitute this node with left son.
 *        If right son exists: substitute this node with right son.
 *        Else: substitute this node with the max element from the
 *              left node AND remove it from there.
 * 2) If the node is null, this means the element does not exist.
*/
static struct remove_rec_ret remove_rec(AVLNode *node, void *element, comparator_function_t cmp, size_t size){
        if (node == NULL)
                return (struct remove_rec_ret){NULL, ELEMENT_NOT_FOUND_ERROR};
        int c = (*cmp) (element, node->info);
        struct remove_rec_ret ret;
        if (c > 0){
                ret = remove_rec(node->right, element, cmp, size);
                node->right = ret.node;
        }else if (c < 0){
                ret = remove_rec(node->left, element, cmp, size);
                node->left = ret.node;
        }else {
                ret.status = SUCCESS;
                AVLNode *aux = node;
                if (node->left == NULL){
                        node = node->right;
                        free(aux);
                } else if(node->right == NULL){
                        node = node->left;
                        free(aux);
                }else {
                        aux = get_max(node->left);
                        memcpy(node->info, aux->info, size);
                        struct remove_rec_ret left = remove_rec(node->left, aux->info, cmp, size);
                        node->left = left.node;
                }
        }
        node = update_bf(node);
        ret.node = node;
        return ret;
}

int avl_remove(AVLTree *tree, void *element){
        assert(tree && element);
        struct remove_rec_ret ret = remove_rec(tree->root, element, tree->compare, tree->data_size);
        if (ret.status == SUCCESS){
                tree->root = ret.node;
                tree->n_elements--;
        }
        return ret.status;
}

int avl_remove_array(AVLTree *tree, void *array, size_t array_length){
        assert(tree && array);
        while (array_length-- > 0){
                avl_remove(tree, array);
                array = void_offset(array, tree->data_size);
        }
        return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

/// OTHER FUNCTIONS ///////////////////////////////////////////////////////////

static bool exists_rec(AVLNode *node, void *element, comparator_function_t compare){
        if (!node)
                return false;
        int c = compare(element, node->info);
        if (c < 0)
                return exists_rec(node->left, element, compare);
        else if (c > 0)
                return exists_rec(node->right, element, compare);
        else
                return true;
}

bool avl_exists(AVLTree *tree, void *element){
        assert(tree && element);
        return exists_rec(tree->root, element, tree->compare);
}

static AVLNode* get_rec(AVLNode *node, void *element, comparator_function_t compare){
        if (!node)
                return NULL;
        int c = compare(element, node->info);
        if (c < 0)
                return get_rec(node->left, element, compare);
        else if (c > 0)
                return get_rec(node->right, element, compare);
        else
                return node;
}

void* avl_get(AVLTree *tree, void *element, void *dest){
        assert(tree && element && dest);
        AVLNode *node = get_rec(tree->root, element, tree->compare);
        if (!node)
                return NULL;
        memcpy(dest, node->info, tree->data_size);
        return dest;
}

size_t avl_size(AVLTree *tree){
        return tree ? tree->n_elements : 0;
}

bool avl_isempty(AVLTree *tree){
        return tree ? tree->root == NULL : true;
}

int avl_height(AVLTree *tree){
        if (!tree || !tree->root)
                return -1;
        return tree->root->height;
}

///////////////////////////////////////////////////////////////////////////////

// Auxiliar structure to use in the traversal methods
struct traversal_ret {
        void* elements;
        size_t elements_size;
        int status;
};

// Auxliar enum to specify the type of traversal for "traversal_rec" function
enum Traversal {
        IN_ORDER, PRE_ORDER, POST_ORDER
};

/// TRAVERSALS ////////////////////////////////////////////////////////////////

/**
 * This method is used to traverse the tree.
 * It can be done in 3 ways: in order, pre order or post order.
 * It's recursive, wich means it calls itself again with the left and right branches.
 * When it reaches a point where both left and right sons are NULL, it will return an
 * array of elements with size 1 (the element in the node).
 * After that, it just takes those arrays and start building bigger arrays out of them.
 * At the end, it returns an array with all the elements in the tree.
*/
static struct traversal_ret traversal_rec(AVLNode *node, enum Traversal order, size_t size){
        // If the node is null, return and empty array
        if(node == NULL){
                return (struct traversal_ret){
                        .elements = NULL,
                        .elements_size = 0,
                        .status = SUCCESS
                };
        }

        struct traversal_ret left = traversal_rec(node->left, order, size);
        struct traversal_ret right = traversal_rec(node->right, order, size);
        struct traversal_ret result = {.status = SUCCESS};

        // If the tarversals from the right returned with error statuses, propagate it.
        if(left.status != SUCCESS || right.status != SUCCESS){
                result.status = left.status != SUCCESS ? left.status : right.status;
                goto cleanup;
        }

        /* Create a new struct traversal_ret to aggregate the traversal
           from left and right and this current node all in one */
        result.elements_size = left.elements_size + right.elements_size + 1;
        result.elements = malloc(result.elements_size * size);
        if (!result.elements){
                result.status = ERROR;
                goto cleanup;
        }

        void *tmp = result.elements;

        if(order == PRE_ORDER){
                memcpy(tmp, node->info, size);
                tmp = void_offset(tmp, size);
        }

        if (left.elements){
                memcpy(tmp, left.elements, size * left.elements_size);
                tmp = void_offset(tmp, size * left.elements_size);
        }

        if(order == IN_ORDER){
                memcpy(tmp, node->info, size);
                tmp = void_offset(tmp, size);
        }

        if (right.elements){
                memcpy(tmp, right.elements, size * right.elements_size);
                tmp = void_offset(tmp, size * right.elements_size);
        }

        if(order == POST_ORDER){
                memcpy(tmp, node->info, size);
        }
cleanup:
        free(left.elements);
        free(right.elements);
        return result;
}

void* avl_preorder(AVLTree *tree){
        assert(tree);
        struct traversal_ret result = traversal_rec(tree->root, PRE_ORDER, tree->data_size);
        return result.elements;
}

void* avl_inorder(AVLTree *tree){
        assert(tree);
        struct traversal_ret result = traversal_rec(tree->root, IN_ORDER, tree->data_size);
        return result.elements;
}

void* avl_postorder(AVLTree *tree){
        assert(tree);
        struct traversal_ret result = traversal_rec(tree->root, POST_ORDER, tree->data_size);

        return result.elements;
}

///////////////////////////////////////////////////////////////////////////////

AVLTree* avl_join(AVLTree *tree_1, AVLTree *tree_2){
        assert(tree_1 && tree_2 && tree_1->data_size == tree_2->data_size);
        AVLTree *tree_joint = avl_init(tree_1->data_size, tree_1->compare);
        int status;
        void *tmp = avl_preorder(tree_1);
        if (tmp != NULL){
                status = avl_add_array(tree_joint, tmp, tree_1->n_elements);
                free(tmp);
                if (status != SUCCESS){
                        free(tree_joint);
                        return NULL;
                }
        }

        tmp = avl_preorder(tree_2);
        if (tmp != NULL){
                status = avl_add_array(tree_joint, tmp, tree_2->n_elements);
                free(tmp);
                if (status != SUCCESS){
                        free(tree_joint);
                        return NULL;
                }
        }

        return tree_joint;
}

///// MAX-MIN /////////////////////////////////////////////////////////////////

void* avl_max(AVLTree *tree, void *dest){
        assert(tree && dest);
        return avl_max_from(tree, tree->root->info, dest);
}

void* avl_min(AVLTree *tree, void *dest){
        assert(tree && dest);
        return avl_min_from(tree, tree->root->info, dest);
}

void* avl_max_from(AVLTree *tree, void *element, void *dest){
        assert(tree && element && dest);
        AVLNode *tmp = get_rec(tree->root, element, tree->compare);
        if (!tmp)
                return NULL;
        tmp = get_max(tmp);
        memcpy(dest, tmp->info, tree->data_size);
        return dest;
}

void* avl_min_from(AVLTree *tree, void *element, void *dest){
        assert(tree && element && dest);
        AVLNode *tmp = get_rec(tree->root, element, tree->compare);
        if (!tmp)
                return NULL;
        tmp = get_min(tmp);
        memcpy(dest, tmp->info, tree->data_size);
        return dest;
}

///////////////////////////////////////////////////////////////////////////////

//// FREE /////////////////////////////////////////////////////////////////////

static void free_node(AVLNode *node, destructor_function_t destructor){
        if (!node)
                return;
        if (destructor)
                destructor(node->info);
        free_node(node->left, destructor);
        free_node(node->right, destructor);
        free(node);
}

void avl_free(AVLTree *tree){
        if (tree){
                free_node(tree->root, tree->destructor);
                free(tree);
        }
}

void avl_free_all(unsigned int n, ...){
        va_list arg;
        va_start(arg, n);
        for (unsigned int i = 0; i < n; i++){
                AVLTree *ptr = va_arg(arg, AVLTree*);
                avl_free(ptr);
        }
        va_end(arg);
}

void avl_clear(AVLTree *tree){
        if (tree){
                free_node(tree->root, tree->destructor);
                tree->root = NULL;
                tree->n_elements = 0;
        }
}
