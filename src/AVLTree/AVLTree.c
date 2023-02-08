/**
 *  Copyright (C) 2022-2023  Saúl Valdelvira Iglesias
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *  Email: saulvaldelvira@gmail.com
 *  Version: 17-01-2023
 */
#include "AVLTree.h"
#include "../Util/error.h"
#include "../Util/definitions.h" // void_offset macro and byte_t typedef
#include <stdlib.h> // malloc
#include <memory.h> // memcpy
#include <stdio.h> // fprintf

#define MAX_DISBALANCE 2

typedef struct AVLNode {
        struct AVLNode *left;
        struct AVLNode *right;
        struct AVLNode *father;
        int height;
        byte_t info[];
} AVLNode;

struct _AVLTree {
        AVLNode *root;
        size_t data_size;
        size_t n_elements;
        comparator_function_t compare;
};

static AVLNode* init_node(void *element, size_t data_size){
        AVLNode *node = malloc(sizeof(*node));
        if (!node){
                printerr_allocation(init_node);
                return NULL;
        }
        node->left = NULL;
        node->right = NULL;
        node->father = NULL;
        node->height = 0;
        if (!memcpy(node->info, element, data_size)){
                printerr_memory_op(init_node);
                free(node);
                return NULL;
        }
        return node;
}

void avl_configure(AVLTree *tree, comparator_function_t cmp) {
	if (!tree || !cmp){
		printerr_null_param(avl_configure);
		return;
	}
	tree->compare = cmp;
}

/**
 * Updates the node's height.
 * An AVL Node's height is the highest of it's son's heights plus one.
 * If a son is NULL, the other son's height is used.
 * If both are NULL this means the current node is a leaf of the three, thus it's height is set to 0.
*/
static void node_update_height(AVLNode *node){
	if (!node){
		return;
	}
        if (node->left == NULL && node->right == NULL){
                node->height = 0;
        }else if (node->left == NULL){
                node->height = node->right->height + 1;
        }else if (node->right == NULL){
                node->height = node->left->height + 1;
        }else {
                node->height = MAX(node->left->height, node->right->height) + 1;
        }
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

AVLTree* avl_init(size_t data_size, comparator_function_t cmp){
        if (!cmp){
                printerr_null_param(avl_init);
                return NULL;
        }
        AVLTree *tree = malloc(sizeof(*tree));
        if (!tree){
                printerr_allocation(avl_init);
                return NULL;
        }
        tree->compare = cmp;
        tree->data_size = data_size;
        tree->n_elements = 0;
        tree->root = NULL;
	return tree;
}

static AVLNode* update_bf(AVLNode *node){
	if (!node){
		return NULL;
	}
        node_update_height(node);
        int bf = node_bf(node);
        if (bf <= -MAX_DISBALANCE){
                int left_bf = node_bf(node->left);
                if (left_bf <= 0){
                        node = single_left_rotation(node);
                }else {
                        node = double_left_rotation(node);
                }
		node_update_height(node);

        }else if (bf  >= MAX_DISBALANCE){
                int right_bf = node_bf(node->right);
                if (right_bf >= 0){
                	node = single_right_rotation(node);
                }else {
                        node = double_right_rotation(node);
                }
		node_update_height(node);
        }
        return node;
}

// Auxiliar struct for the add_rec function
struct add_rec_ret
{
	AVLNode* node;
	int status;
	bool last_op_was_add;
};

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
 * new node, and a SUCCESS status flag.
 *
 * As said before in this last case, the "node->right = ret.node;" and "node->left = ret.node;" will update this null references to target this newly created node.
 *
 * Also after that, the method checks if the last operation was a succesfull addition of a node. In this case, the node contained in this struct add_rec_ret called ret,
 * a node added in the previous call, so we update this node's father reference to the current node.
 *
 * The use  of struct add_rec_ret is to be able to check the struct add_rec_ret.status when this chain of recursive calls ends and returns to avl_add. This way, we can
 * check if the operation was a SUCCESS and increment the n_elements acordingly, or else we have to return an error status.
*/
static struct add_rec_ret add_rec(AVLNode *node, void *element, comparator_function_t cmp, size_t size){
	if (node == NULL){ // The element does not exist in the tree
		AVLNode *aux = init_node(element, size); // Create the node
		if (!aux){ // If memory could not be allocated, return with an error status
			return (struct add_rec_ret) {aux, ALLOCATION_ERROR, false};
		} else { // Return the new node with a SUCCESS status
			return (struct add_rec_ret) {aux, SUCCESS, true};
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
		return (struct add_rec_ret) {node, REPEATED_ELEMENT_ERROR, false};
	}

	if(ret.last_op_was_add){ // If the last call returned a SUCCESSfuly created node, update this node's father reference
		ret.node->father = node;
		ret.last_op_was_add = false; // Set this value to false to avoid doing unecesary updates
	}
	node = update_bf(node);
	ret.node = node; // Change the node in ret to the actual one before returning
	return ret;
}

int avl_add(AVLTree *tree, void *element){
        if (!tree || !element){
                printerr_null_param(avl_add);
                return NULL_PARAMETER_ERROR;
        }
        struct add_rec_ret ret = add_rec(tree->root, element, tree->compare, tree->data_size);
        if (ret.status != SUCCESS){
                return ret.status;
        }
        tree->root = ret.node;
	tree->n_elements++;
        return SUCCESS;
}

int avl_add_array(AVLTree *tree, void *array, size_t array_length){
	if (!tree || !array){
                printerr_null_param(avl_add_array);
                return NULL_PARAMETER_ERROR;
        }
	void *tmp;
	int status;
	for (size_t i = 0; i < array_length; i++){
		tmp = void_offset(array, i * tree->data_size);
		status = avl_add(tree, tmp);
		if (status != SUCCESS){
			return status;
		}
	}
	return SUCCESS;
}

static AVLNode* get_max(AVLNode *node){
	if (node == NULL){
		return NULL;
	}
	while (node->right != NULL){
		node = node->right;
	}
	return node;
}

static AVLNode* get_min(AVLNode *node){
	if (node == NULL){
		return NULL;
	}
	while (node->left != NULL){
		node = node->left;
	}
	return node;
}


// Auxiliar struct for the remove_rec function
struct remove_rec_ret {
	AVLNode* node;
	int status;
};
/**
 * This function behaves similarly to the add_rec. It starts searching through the tree->
 * After every call, we update the current node's references to left or right (depending on the result of the comparison).
 * If we find the node to delete, we have to free this node, and return another one.
 * We have three cases
 * 1) The left son is NULL -> we return the right son
 * 2) The right son is NULL -> we return the left son
 *  NOTE: if both nodes are null the previous conditions will return NULL
 * 3) If there are left and right son, we set the current node's info to the BIGGEST element starting from the left son.
 *      
*/
static struct remove_rec_ret remove_rec(AVLNode *node, void *element, comparator_function_t cmp, size_t size){
	if (node == NULL){
		return (struct remove_rec_ret){NULL, ELEMENT_NOT_FOUND_ERROR};
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
		ret.status = SUCCESS;
		AVLNode *aux = node;
		if (node->left == NULL){
			node = node->right;
			free(aux);
		} else if(node->right == NULL) {
			node = node->left;
			free(aux);
		}else { // Case 3
			aux = get_max(node->left); // Get the biggest son starting in the left node
			if(!memcpy(node->info, aux->info, size)){
				printerr_memory_op(remove_rec);
				ret.status = MEMORY_OP_ERROR;
			}
			struct remove_rec_ret left = remove_rec(node->left, aux->info, cmp, size);
			node->left = left.node;
		}
	}
	node = update_bf(node);
	ret.node = node;
	return ret;
}

int avl_remove(AVLTree *tree, void *element){
	if (!tree || !element){
		printerr_null_param(avl_remove);
		return NULL_PARAMETER_ERROR;
	}
	struct remove_rec_ret ret = remove_rec(tree->root, element, tree->compare, tree->data_size);
	if (ret.status != SUCCESS){
		return ret.status;
	}
	tree->root = ret.node;
	tree->n_elements--;
	return SUCCESS;
}

int avl_remove_array(AVLTree *tree, void *array, size_t array_length){
	if (!tree || !array){
                printerr_null_param(avl_remove_array);
                return NULL_PARAMETER_ERROR;
        }
	void *tmp;
	int status;
	for (size_t i = 0; i < array_length; i++){
		tmp = void_offset(array, i * tree->data_size);
		status = avl_remove(tree, tmp);
		if (status != SUCCESS){
			return status;
		}
	}
	return SUCCESS;
}

bool exists_rec(AVLNode *node, void *element, comparator_function_t compare){
	if (!node){
		return false;
	}
	int c = compare(element, node->info);
	if (c < 0){
		return exists_rec(node->left, element, compare);
	} else if (c > 0){
		return exists_rec(node->right, element, compare);
	} else{
		return true;
	}
}

bool avl_exists(AVLTree *tree, void *element){
	if (!tree || !element){
		printerr_null_param(avl_exists);
		return false;
	}
	return exists_rec(tree->root, element, tree->compare);
}

static AVLNode* get_rec(AVLNode *node, void *element, comparator_function_t compare){
	if (!node) {
		return NULL;
	}
	int c = compare(element, node->info);
	if (c < 0){
		return get_rec(node->left, element, compare);
	} else if (c > 0){
		return get_rec(node->right, element, compare);
	} else{
		return node;
	}
}

void* avl_get(AVLTree *tree, void *element, void *dest){
	if (!tree || !element || !dest){
		printerr_null_param(avl_get);
		return NULL;
	}
	AVLNode *node = get_rec(tree->root, element, tree->compare);
	if (!node){
		return NULL;
	}
	if(!memcpy(dest, node->info, tree->data_size)){
		printerr_memory_op(avl_get);
		return NULL;
	}
	return dest;
}

size_t avl_size(AVLTree *tree){
	if (!tree){
		printerr_null_param(avl_size);
		return 0;
	}
	return tree->n_elements;
}

bool avl_isempty(AVLTree *tree){
	if (!tree){
		printerr_null_param(avl_isempty);
		return 0;
	}
	return tree->root == NULL;
}

int avl_height(AVLTree *tree){
	if (!tree){
		printerr_null_param(avl_height);
		return NULL_PARAMETER_ERROR;
	}
	if (tree->root == NULL){
		return -1;
	}
	return tree->root->height;
}

/////////////////////////
// Auxiliar structure to use in the traversal methods
struct traversal_ret {
	void* elements;
	size_t elements_size;
	int status;
};

// Auxliar enum to specify the type of traversal to "traversal_rec" function
enum Traversal {
	IN_ORDER, PRE_ORDER, POST_ORDER
};

/**
 * This method is used to traverse the tree->
 * It can be done in 3 ways: in order, pre order or post order.
 * It's recursive, wich means it calls recursivelly itself with the left and right branch.
 * When it reaches a point where both left and right sons are NULL, it will return an array of elements with size 1.
 * After that, it just takes those arrays and start building bigger arrays out of them, with the specified order.
 * In the last call, it will return an array with all the elements in the array.
*/
static struct traversal_ret traversal_rec(AVLNode *node, enum Traversal order, size_t size){
	// If the node is null, return and empty array
	if(node == NULL){
		return (struct traversal_ret) {
			.elements = NULL,
			.elements_size = 0,
			.status = SUCCESS
		};
	}

	struct traversal_ret left = traversal_rec(node->left, order, size);
	struct traversal_ret right = traversal_rec(node->right, order, size);
	struct traversal_ret result;

	// If the tarversals from the right returned with error statuses, propagate it.
	if(left.status != SUCCESS || right.status != SUCCESS){
		result.status = MEMORY_OP_ERROR;
		goto free_garbage;
	}

	// Create a new struct traversal_ret to agregate the traversal from left and right and this current node all in one
	result.elements_size = left.elements_size + right.elements_size + 1; // The +1 is for the element in this node
	result.elements = malloc(result.elements_size * size);
	if(!result.elements){
		printerr_allocation(traversal_rec);
		free(result.elements);
		result.status = ALLOCATION_ERROR;
		goto free_garbage;
	}
	result.status = SUCCESS;

	size_t index = 0;

	// Depending on the order paranmeter, the current node will be added before (pre order) in the middle (in order) or after (post order)

	void *tmp;

	if(order == PRE_ORDER){
		tmp = memcpy(result.elements, node->info, size);
		if(!tmp){
			printerr_memory_op(traversal_rec);
			free(result.elements);
			result.status = MEMORY_OP_ERROR;
			goto free_garbage;
		}
		index++;
	}

	// Add the elements of the left
	tmp = void_offset(result.elements, index * size);
	tmp = memcpy(tmp, left.elements, size * left.elements_size);
	if(!tmp){
		printerr_memory_op(traversal_rec);
		free(result.elements);
		result.status = MEMORY_OP_ERROR;
		goto free_garbage;
	}
	index += left.elements_size;

	if(order == IN_ORDER){
		tmp = void_offset(result.elements, index * size);
		tmp = memcpy(tmp, node->info, size);
		if(!tmp){
			printerr_memory_op(traversal_rec);
			free(result.elements);
			result.status = MEMORY_OP_ERROR;
			goto free_garbage;
		}
		index++;
	}

	// Add the elements of the right
	tmp = void_offset(result.elements, index * size);
	tmp = memcpy(tmp, right.elements, size * right.elements_size);
	if(!tmp){
		printerr_memory_op(traversal_rec);
		free(result.elements);
		result.status = MEMORY_OP_ERROR;
		goto free_garbage;
	}
	index += right.elements_size;

	if(order == POST_ORDER){
		tmp = void_offset(result.elements, index * size);
		tmp = memcpy(tmp, node->info, size);
		if(!tmp){
			printerr_memory_op(traversal_rec);
			free(result.elements);
			result.status = MEMORY_OP_ERROR;
			goto free_garbage;
		}
	}

	// Free the left and right arrays because our result.data already stores the elements thus this two arrays are useless now
	free_garbage:
		free(left.elements);
		free(right.elements);
		return result;
}

void* avl_preorder(AVLTree *tree){
	if (!tree){
		printerr_null_param(avl_preorder);
		return NULL;
	}
	struct traversal_ret result = traversal_rec(tree->root, PRE_ORDER, tree->data_size);
	if (result.status != SUCCESS){
		return NULL;
	}
	return result.elements;
}

void* avl_inorder(AVLTree *tree){
	if (!tree){
		printerr_null_param(avl_inorder);
		return NULL;
	}
	struct traversal_ret result = traversal_rec(tree->root, IN_ORDER, tree->data_size);
	if (result.status != SUCCESS){
		return NULL;
	}
	return result.elements;
}

void* avl_postorder(AVLTree *tree){
	if (!tree){
		printerr_null_param(avl_postorder);
		return NULL;
	}
	struct traversal_ret result = traversal_rec(tree->root, POST_ORDER, tree->data_size);
	if (result.status != SUCCESS){
		return NULL;
	}
	return result.elements;
}
////////////////

AVLTree* avl_join(AVLTree *tree_1, AVLTree *tree_2){
	if (!tree_1 || !tree_2){
		printerr_null_param(avl_join);
		return NULL;
	}
	if (tree_1->data_size != tree_2->data_size){
		fprintf(stderr, "ERROR: the trees have different data sizes. In function avl_join\n");
		return NULL;
	}
	AVLTree *tree_joint = avl_init(tree_1->data_size, tree_1->compare);
	if (!tree_joint){
		return NULL;
	}

	int status;
	void *tmp = avl_preorder(tree_1);
	if (tmp != NULL){
		status = avl_add_array(tree_joint, tmp, tree_1->n_elements);
		free(tmp);
		if (status != SUCCESS){
			goto exit_err;
		}
	}

	tmp = avl_preorder(tree_2);
	if (tmp != NULL){
		status = avl_add_array(tree_joint, tmp, tree_2->n_elements);
		free(tmp);
		if (status != SUCCESS){
			exit_err:
			free(tree_joint);
			return NULL;
		}
	}

	return tree_joint;
}


void* avl_max(AVLTree *tree, void *dest){
	if (!tree || !dest){
		printerr_null_param(avl_max);
		return NULL;
	}
	return avl_max_from(tree, tree->root->info, dest);
}


void* avl_min(AVLTree *tree, void *dest){
	if (!tree || !dest){
		printerr_null_param(avl_min);
		return NULL;
	}
	return avl_min_from(tree, tree->root->info, dest);
}


void* avl_max_from(AVLTree *tree, void *element, void *dest){
	if (!tree || !element || !dest){
		printerr_null_param(avl_max_from);
		return NULL;
	}
	AVLNode *tmp = get_rec(tree->root, element, tree->compare);
	if (!tmp){
		return NULL;
	}
	tmp = get_max(tmp);
	
	if (!memcpy(dest, tmp->info, tree->data_size)){
		printerr_memory_op(avl_max_from);
		return NULL;
	}
	return dest;
}


void* avl_min_from(AVLTree *tree, void *element, void *dest){
	if (!tree || !element || !dest){
		printerr_null_param(avl_min_from);
		return NULL;
	}
	AVLNode *tmp = get_rec(tree->root, element, tree->compare);
	if (!tmp){
		return NULL;
	}
	tmp = get_min(tmp);
	
	if (!memcpy(dest, tmp->info, tree->data_size)){
		printerr_memory_op(avl_min_from);
		return NULL;
	}
	return dest;
}

////////

static void free_node(AVLNode *node){
	if (!node){
		return;
	}
	free_node(node->left);
	free_node(node->right);
	free(node);
}

int avl_free(AVLTree *tree){
	if (!tree){
		printerr_null_param(avl_free);
		return NULL_PARAMETER_ERROR;
	}
	free_node(tree->root);
	free(tree);
	return SUCCESS;
}

AVLTree* avl_reset(AVLTree *tree){
	if (!tree){
		printerr_null_param(avl_reset);
		return NULL;
	}
	free_node(tree->root);
	tree->root = NULL;
	tree->n_elements = 0;
	return tree;
}