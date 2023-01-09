/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 08-01-2023
 * Copyright: Saúl Valdelvira Iglesias (2023)
 * Licensed under the GNU GPL V.3. See /LICENSE file for more info
*/
#include "BSTree.h"
#include "../Util/checks.h"

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

struct BSNode {
    struct BSNode *right;
    struct BSNode *left;
    struct BSNode *father;
    unsigned char info[];
};

BSTree bst_init(size_t data_size, int (*cmp) (const void*,const void*)){
    return (BSTree) {
        .root = NULL,
        .compare = cmp,
        .n_elements = 0,
        .data_size = data_size
    };
}

static BSNode* init_node(void *info, size_t size){
    BSNode *node = malloc(offsetof(BSNode, info) + size);
    CHECK_MEMORY(node, init_node, NULL)
    node->left = NULL;
    node->right = NULL;
    node->father = NULL;
    if(!memcpy(node->info, info, size)){
        fprintf(stderr, "ERROR: Could not initialize node\n");
        return NULL;
    }
    return node;
}

// Auxiliar struct for the add_rec function (NOTE: this is part of the function name)
static struct add_rec_ret
{
    BSNode* node;
    int status;
    bool last_op_was_add;
}
    /**
     * This method is a little bit tricky, specially for the weird struct that it returns.
     * It works recursivelly.
     * When it's called with a node, it starts searching for a place to put the element in. Since this is a Binary Search Tree, 
     * when comparing the element with the current node's info we can know if we have to continue searching on the left or the right branch.
     * If the comparison returns < 0, it means that the element we are trying to store is lower than the current element in the node, so we launch 
     * the search in the left son of the node. The oposite with comparison > 0, but in the right side.
     * The struct add_rec_ret this call returns, contains a node, a status and a bool flag.
     * We update the node we just called the search upon (left or right depending on the previous comparison). Most of the time, this will do
     * nothing, since the only time this will change anything is when we find a NULL node in this method.
     * When we reach a NULL node, this means we can stop searching and put there the node, so we return one of this struct add_rec_ret element, containing this 
     * new node, and a sucess status flag.
     * 
     * As said before in this last case, the "node->right = ret.node;" and "node->left = ret.node;" will update this null references to target this newly created node.
     * 
     * Also after that, the method checks if the last operation was a succesfull addition of a node. In this case, the node contained in this struct add_rec_ret called ret, 
     * a node added in the previous call, so we update this node's father reference to the current node.
     * 
     * The use  of struct add_rec_ret is to be able to check the struct add_rec_ret.status when this chain of recursive calls ends and returns to bst_add. This way, we can
     * check if the operation was a sucess and increment the n_elements acordingly, or else we have to return an error status.
    */
add_rec(BSNode *node, void *element, int (*cmp) (const void*,const void*), size_t size){
    if (node == NULL){ // The element does not exist in the tree
        BSNode *aux = init_node(element, size); // Create the node
        if (!aux){ // If memory could not be allocated, return with an error status
            return (struct add_rec_ret) {aux, ALLOCATION_ERROR, false};
        } else { // Return the new node with a sucess status
            return (struct add_rec_ret) {aux, 1, true};
        }
    }
    struct add_rec_ret ret;
    int c = (*cmp) (element, node->info);
    if (c > 0){ // The element is higher than the node's info. Search right
        ret = add_rec(node->right, element, cmp, size);
        node->right = ret.node; // Update the right node
    }else if (c < 0){ // The element is lower than the node's info. Search left
        ret = add_rec(node->left, element, cmp, size);
        node->left = ret.node; // Update the left node
    }else { // Repeated element, return with an error status
        return (struct add_rec_ret) {node, REPEATED_ELEMENT, false};
    }

    if(ret.last_op_was_add){ // If the last call returned a sucessfuly created node, update this node's father reference
        ret.node->father = node;
        ret.last_op_was_add = false; // Set this value to false to avoid doing unecesary updates
    }
    ret.node = node; // Change the node in ret to the actual one before returning
    return ret;
}

int bst_add(BSTree *tree, void *element){
    CHECK_NULL(tree, bst_add, NULL_PARAMETER)
    struct add_rec_ret ret = add_rec(tree->root, element, tree->compare, tree->data_size);
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

/*static BSNode* get_min(BSNode *node){
    if (node == NULL){
        return NULL;
    }
    while (node->left != NULL){
        node = node->left;
    }
    return node;
}*/

// Auxiliar struct for the remove_rec function (NOTE: this is part of the function name)
static struct remove_rec_ret {
    BSNode* node;
    int status;
} 
/**
 * This function behaves similarly to the add_rec. It starts searching through the tree.
 * After every call, we update the current node's references to left or right (depending on the result of the comparison).
 * If we find the node to delete, we have to free this node, and return another one.
 * We have three cases
 * 1) The left son is NULL -> we return the right son
 * 2) The right son is NULL -> we return the left son
 *  NOTE: if both nodes are null the previous conditions will return NULL
 * 3) If there are left and right son, we set the current node's info to the BIGGEST element starting from the left son. 
 *      After that, there are two nodes with the same info, so we delete the node that previosly stored this info, since it will now be stored in this node
*/
remove_rec(BSNode *node, void *element, int (*cmp) (const void*,const void*), size_t size){
    if (node == NULL){
        return (struct remove_rec_ret){NULL, NON_EXISTING_ELEMENT};
    }

    int c = (*cmp) (element, node->info);
    struct remove_rec_ret ret;
    if (c > 0){ // The element is higher than the node's info
        ret = remove_rec(node->right, element, cmp, size);
        node->right = ret.node;
    }else if (c < 0){
        ret = remove_rec(node->left, element, cmp, size);
        node->left = ret.node;
    }else {
        if (node->left == NULL){
            node = node->right;
        } else if(node->right == NULL) {
            node = node->left;
        }else { // Case 3
            BSNode *aux = get_max(node->left); // Get the biggest son starting in the left node
            if(aux != node->left){ // This means the left node have at least one right son
                aux->father->right = NULL; // We simply clear that reference
            }else{ // If aux node is the left node itself (The biggest element from the left is theleft itself.)
                node->left = aux->left; // Now we can safely remove aux without losing the left son of aux (the right son is NULL)
            }
            if(!memcpy(node->info, aux->info, size)){
                fprintf(stderr, "ERROR: could not remove (line 174)\n");
                ret.status = -1;
            }else{
                ret.status = 1;
            }
            free(aux); // Free aux
        }
    }
    ret.node = node;
    return ret;
}

int bst_remove(BSTree *tree, void *element){
    CHECK_NULL(tree, bst_remove, NULL_PARAMETER)
    struct remove_rec_ret ret = remove_rec(tree->root, element, tree->compare, tree->data_size);
    tree->root = ret.node;
    if (ret.status == 1){
        tree->n_elements--;
    }
    return ret.status; 
}

static void* get_rec(BSNode *node, void *element, void *dest, int (*cmp) (const void*,const void*), size_t size){
    if(node == NULL){
        return NULL;
    }
    int c = (*cmp) (element, node->info);
    if(c < 0){
        return get_rec(node->left, element, dest, cmp, size);
    }else if(c > 0){
        return get_rec(node->right, element, dest, cmp, size);
    }else{
        return memcpy(dest, node->info, size);
    }
}

void* bst_get(BSTree tree, void* element, void *dest){
    return get_rec(tree.root, element, dest, tree.compare, tree.data_size);
}

static bool exists_rec(BSNode *node, void *element, int (*cmp) (const void*,const void*)){
    if(node == NULL){
        return false;
    }
    int c = (*cmp) (element, node->info);
    if(c < 0){
        return exists_rec(node->left, element, cmp);
    }else if(c > 0){
        return exists_rec(node->right, element, cmp);
    }else{
        return true;
    }
}

bool bst_exists(BSTree tree, void *element){
    return exists_rec(tree.root, element, tree.compare);
}

bool bst_isempty(BSTree tree){
    return tree.root == NULL;
}

static void free_rec(BSNode *node){
    if(node == NULL){
        return;
    }
    free_rec(node->left);
    free_rec(node->right);
    free(node);
}

void bst_free(BSTree tree){
    free_rec(tree.root);
}

void bst_reset(BSTree *tree){
    free_rec(tree->root);
    tree->root = NULL;
    tree->n_elements = 0;
}

#define ORDERS_ERROR (struct orders_ret) {.elements = NULL, .elements_size = 0, .status = -1}

// Auxiliar structure to use in the traversal methods
struct orders_ret {
    void* elements;
    size_t elements_size;
    int status;
};

// Auxliar enum to specify the type of traversal to "traversal_rec" function
enum Traversal {
    IN_ORDER, PRE_ORDER, POST_ORDER
};

/**
 * This method is used to traverse the tree.
 * It can be done in 3 ways: in order, pre order or post order.
 * It's recursive, wich means it calls recursivelly itself with the left and right branch.
 * When it reaches a point where both left and right sons are NULL, it will return an array of elements with size 1.
 * After that, it just takes those arrays and start building bigger arrays out of them, with the specified order.
 * In the last call, it will return an array with all the elements in the array.
*/
static struct orders_ret traversal_rec(BSNode *node, enum Traversal order, size_t size){
    // If the node is null, return and empty array
    if(node == NULL){
        return (struct orders_ret) {
            .elements = NULL,
            .elements_size = 0,
            .status = 1
        };
    }

    struct orders_ret left = traversal_rec(node->left, order, size);
    struct orders_ret right = traversal_rec(node->right, order, size);
    
    // If the tarversals from the right returned with error statuses, propagate it.
    if(left.status != 1 || right.status != 1){
        return ORDERS_ERROR;
    }

    // Create a new struct orders_ret to agregate the traversal from left and right and this current node all in one
    struct orders_ret result;
    result.elements_size = left.elements_size + right.elements_size + 1; // The +1 is for the element in this node
    result.elements = malloc(result.elements_size * size);
    CHECK_MEMORY(result.elements, bst_inorder, ORDERS_ERROR)
    result.status = 1;

    size_t index = 0;

    // Depending on the order paranmeter, the current node will be added before (pre order) in the middle (in order) or after (post order)

    if(order == PRE_ORDER){
        if(!memcpy(result.elements, node->info, size)){
            return ORDERS_ERROR;
        }
        index++;
    }

    // Add the elements of the left

    if(!memcpy(offset(result.elements, index, size), left.elements, size * left.elements_size)){
        return ORDERS_ERROR;
    }
    index += left.elements_size;

    if(order == IN_ORDER){
        if(!memcpy(offset(result.elements, index, size), node->info, size)){
            return ORDERS_ERROR;
        }
        index++;
    }

    // Add the elements of the right
    if(!memcpy(offset(result.elements, index, size), right.elements, size * right.elements_size)){
        return ORDERS_ERROR;
    }
    index += right.elements_size;

    if(order == POST_ORDER){
        if(!memcpy(offset(result.elements, index, size), node->info, size)){
            return ORDERS_ERROR;
        }
    }

    // Free the left and right arrays because our result.data already stores the elements thus this two arrays are useless now
    free(left.elements);
    free(right.elements);
    return result;
}

void* bst_inorder(BSTree tree){
    struct orders_ret result = traversal_rec(tree.root, IN_ORDER, tree.data_size);
    if (result.status != 1){
        return NULL;
    }
    return result.elements;
}

void** bst_preorder(BSTree tree){
    struct orders_ret result = traversal_rec(tree.root, PRE_ORDER, tree.data_size);
    if (result.status != 1){
        return NULL;
    }
    return result.elements;
}

void** bst_postorder(BSTree tree){
    struct orders_ret result = traversal_rec(tree.root, POST_ORDER, tree.data_size);
    if (result.status != 1){
        return NULL;
    }
    return result.elements;
}