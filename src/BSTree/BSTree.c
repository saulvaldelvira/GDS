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
#include "BSTree.h"
#include "../Util/error.h"
#include "../Util/definitions.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

typedef struct BSNode {
	struct BSNode *right;
	struct BSNode *left;
	struct BSNode *father;
	byte_t info[];
}BSNode;

struct _BSTree {
	BSNode *root;
	// Comparator function for 2 elements
	comparator_function_t compare;
	size_t n_elements;
	size_t data_size;
};

BSTree* bst_init(size_t data_size, comparator_function_t cmp){
	if (data_size <= 0){
		printerr_data_size(bst_init);
		return NULL;
	}
	if (!cmp){
		printerr_null_param(bst_init);
		return NULL;
	}
	BSTree *tree = malloc(sizeof(*tree));
	if (!tree){
		printerr_allocation(bst_init);
		return NULL;
	}
	tree->root = NULL;
	tree->compare = cmp;
	tree->n_elements = 0;
	tree->data_size = data_size;
	return tree;
}

static BSNode* init_node(void *info, size_t size){
	BSNode *node = malloc(offsetof(BSNode, info) + size);
	if (!node){
		printerr_allocation(init_node);
		return NULL;
	}
	node->left = NULL;
	node->right = NULL;
	node->father = NULL;
	if(!memcpy(node->info, info, size)){
		printerr_memory_op(init_node);
		return NULL;
	}
	return node;
}

// Auxiliar struct for the add_rec function
struct add_rec_ret {
	BSNode* node;
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
	 * The use  of struct add_rec_ret is to be able to check the struct add_rec_ret.status when this chain of recursive calls ends and returns to bst_add. This way, we can
	 * check if the operation was a SUCCESS and increment the n_elements acordingly, or else we have to return an error status.
	*/
static struct add_rec_ret add_rec(BSNode *node, void *element, comparator_function_t cmp, size_t size){
	if (node == NULL){ // The element does not exist in the tree
		BSNode *aux = init_node(element, size); // Create the node
		if (!aux){ // If memory could not be allocated, return with an error status
			return (struct add_rec_ret) {aux, ALLOCATION_ERROR, false};
		} else { // Return the new node with a SUCCESS status
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
		return (struct add_rec_ret) {node, REPEATED_ELEMENT_ERROR, false};
	}

	if(ret.last_op_was_add){ // If the last call returned a SUCCESSfuly created node, update this node's father reference
		ret.node->father = node;
		ret.last_op_was_add = false; // Set this value to false to avoid doing unecesary updates
	}
	ret.node = node; // Change the node in ret to the actual one before returning
	return ret;
}

int bst_add(BSTree *tree, void *element){
	if (!tree || !element){
		printerr_null_param(bst_add);
		return NULL_PARAMETER_ERROR;
	}
	struct add_rec_ret ret = add_rec(tree->root, element, tree->compare, tree->data_size);
	tree->root = ret.node;
	if (ret.status == SUCCESS){
		tree->n_elements++;
	}
	return ret.status;
}

int bst_add_array(BSTree *tree, void *array, size_t array_length){
	if (!tree || !array){
		printerr_null_param(bst_add_array);
		return NULL_PARAMETER_ERROR;
	}
	void *tmp;
	int status;
	for (size_t i = 0; i < array_length; i++){
		tmp = void_offset(array, i * tree->data_size);
		status = bst_add(tree, tmp);
		if (status != SUCCESS){
			return status;
		}
	}
	return SUCCESS;
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

// Auxiliar struct for the remove_rec function
struct remove_rec_ret {
	BSNode* node;
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
 *      After that, there are two nodes with the same info, so we delete the node that previosly stored this info, since it will now be stored in this node
*/
static struct remove_rec_ret remove_rec(BSNode *node, void *element, comparator_function_t cmp, size_t size){
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
		ret.status = 1;
		BSNode *aux = node;
		if (node->left == NULL){
			node = node->right;
		} else if(node->right == NULL) {
			node = node->left;
		}else { // Case 3
			aux = get_max(node->left); // Get the biggest son starting in the left node
			if(aux != node->left){ // This means the left node have at least one right son
				aux->father->right = NULL; // We simply clear that reference
			}else{ // If aux node is the left node itself (The biggest element from the left is theleft itself.)
				node->left = aux->left; // Now we can safely remove aux without losing the left son of aux (the right son is NULL)
			}
			if(!memcpy(node->info, aux->info, size)){
				printerr_memory_op(add_rec);
				ret.status = -1;
			}
		}
		free(aux); // Free aux
	}
	ret.node = node;
	return ret;
}

int bst_remove(BSTree *tree, void *element){
	if (!tree || !element){
		printerr_null_param(bst_remove);
		return NULL_PARAMETER_ERROR;
	}
	struct remove_rec_ret ret = remove_rec(tree->root, element, tree->compare, tree->data_size);
	if (ret.status){
		tree->root = ret.node; // ??? should this be here our outside ???
		tree->n_elements--;
	}
	return ret.status;
}


int bst_remove_array(BSTree *tree, void *array, size_t array_length){
	if (!tree || !array){
		printerr_null_param(bst_remove_array);
		return NULL_PARAMETER_ERROR;
	}
	void *tmp;
	int status;
	for (size_t i = 0; i < array_length; i++){
		tmp = void_offset(array, i * tree->data_size);
		status = bst_remove(tree, tmp);
		if (status != SUCCESS){
			return status;
		}
	}
	return SUCCESS;
}

static void* get_rec(BSNode *node, void *element, void *dest, comparator_function_t cmp, size_t size){
	if(node == NULL){
		return NULL;
	}
	int c = (*cmp) (element, node->info);
	if(c < 0){
		return get_rec(node->left, element, dest, cmp, size);
	}else if(c > 0){
		return get_rec(node->right, element, dest, cmp, size);
	}else{
		if (!memcpy(dest, node->info, size)){
			printerr_memory_op(get_rec);
			return NULL;
		} else{
			return dest;
		}
	}
}

void* bst_get(BSTree *tree, void* element, void *dest){
	if (!tree || !element || !dest){
		printerr_null_param(bst_get);
		return NULL;
	}
	return get_rec(tree->root, element, dest, tree->compare, tree->data_size);
}

static bool exists_rec(BSNode *node, void *element, comparator_function_t cmp){
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

bool bst_exists(BSTree *tree, void *element){
	if (!tree || !element){
		printerr_null_param(bst_exists);
		return NULL;
	}
	return exists_rec(tree->root, element, tree->compare);
}

size_t bst_size(BSTree *tree){
	if (!tree){
		printerr_null_param(bst_size);
		return 0; // Should we use index_t ???
	}
	return tree->n_elements;
}

bool bst_isempty(BSTree *tree){
	if (!tree){
		printerr_null_param(bst_isempty);
		return false;
	}
	return tree->root == NULL;
}

BSTree* bst_join(BSTree *tree_1, BSTree *tree_2){
	if (!tree_1 || !tree_2){
		printerr_null_param(bst_joins);
		return NULL;
	}
	if (tree_1->data_size != tree_2->data_size){
		fprintf(stderr, "ERROR: the trees have different data sizes. In function bst_join\n");
		return NULL;
	}
	BSTree *tree_joint = bst_init(tree_1->data_size, tree_1->compare);
	if (!tree_joint){
		return NULL;
	}

	int status;
	void *tmp = bst_preorder(tree_1);
	if (tmp != NULL){
		status = bst_add_array(tree_joint, tmp, tree_1->n_elements);
		free(tmp);
		if (status != SUCCESS){
			goto exit_err;
		}
	}

	tmp = bst_preorder(tree_2);
	if (tmp != NULL){
		status = bst_add_array(tree_joint, tmp, tree_2->n_elements);
		free(tmp);
		if (status != SUCCESS){
			exit_err:
			free(tree_joint);
			return NULL;
		}
	}

	return tree_joint;
}

static void free_rec(BSNode *node){
	if(node == NULL){
		return;
	}
	free_rec(node->left);
	free_rec(node->right);
	free(node);
}

int bst_free(BSTree *tree){
	if (!tree){
		printerr_null_param(bst_free);
		return NULL_PARAMETER_ERROR;
	}
	free_rec(tree->root);
	free(tree);
	return 1;
}

BSTree* bst_reset(BSTree *tree){
	if (!tree){
		printerr_null_param(bst_reset);
		return NULL;
	}
	free_rec(tree->root);
	tree->root = NULL;
	tree->n_elements = 0;
	return tree;
}

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
static struct traversal_ret traversal_rec(BSNode *node, enum Traversal order, size_t size){
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

void* bst_preorder(BSTree *tree){
	if (!tree){
		printerr_null_param(bst_preorder);
		return NULL;
	}
	struct traversal_ret result = traversal_rec(tree->root, PRE_ORDER, tree->data_size);
	if (result.status != SUCCESS){
		return NULL;
	}
	return result.elements;
}

void* bst_inorder(BSTree *tree){
	if (!tree){
		printerr_null_param(bst_inorder);
		return NULL;
	}
	struct traversal_ret result = traversal_rec(tree->root, IN_ORDER, tree->data_size);
	if (result.status != SUCCESS){
		return NULL;
	}
	return result.elements;
}

void* bst_postorder(BSTree *tree){
	if (!tree){
		printerr_null_param(bst_postorder);
		return NULL;
	}
	struct traversal_ret result = traversal_rec(tree->root, POST_ORDER, tree->data_size);
	if (result.status != SUCCESS){
		return NULL;
	}
	return result.elements;
}