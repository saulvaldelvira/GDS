/**
 *  Copyright (C) 2023 - Saúl Valdelvira
 *
 *  This library is free software. You can modify
 *  and/or redistribute it under the terms of the
 *  GNU Lesser General Public License version 2.1
 *  See <https://www.gnu.org/licenses/>
 *
 *  Email: saulvaldelvira@gmail.com
 */
#include "BSTree.h"
#include "./Util/error.h"
#include "./Util/definitions.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

typedef struct BSNode {
	struct BSNode *right;
	struct BSNode *left;
	struct BSNode *father;
	byte_t info[];
}BSNode;

struct BSTree {
	BSNode *root;
	// Comparator function for 2 elements
	comparator_function_t compare;
	size_t n_elements;
	size_t data_size;
};

///// INITIALIZE //////////////////////////////////////////////////////////////

BSTree* bst_init(size_t data_size, comparator_function_t cmp){
	if (!cmp){
		printerr_null_param();
		return NULL;
	}
	if (data_size == 0){
		printerr_data_size();
		return NULL;
	}
	BSTree *tree = malloc(sizeof(*tree));
	if (!tree){
		printerr_allocation();
		return NULL;
	}
	tree->root = NULL;
	tree->compare = cmp;
	tree->n_elements = 0;
	tree->data_size = data_size;
	return tree;
}

void bst_configure(BSTree *tree, comparator_function_t cmp){
	if (!tree || !cmp){
		printerr_null_param();
		return;
	}
	tree->compare = cmp;
}

static BSNode* init_node(void *info, size_t size){
	BSNode *node = malloc(offsetof(BSNode, info) + size);
	if (!node){
		printerr_allocation();
		return NULL;
	}
	node->left = NULL;
	node->right = NULL;
	node->father = NULL;
	memcpy(node->info, info, size);
	return node;
}

///////////////////////////////////////////////////////////////////////////////

//////// ADD-REMOVE //////////////////////////////////////////////////////////////////

// Auxiliar struct for the add function
struct add_rec_ret {
	BSNode* node;
	int status;
};


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


/**
 * Adds the element to the node.
 * 1) Compares the element with the node's info.
 * 	If higher: continue with the right son
 * 	If lower: continue with the left son
 * 	Else: repeated element!
 * 2) If the node is null, we reached a leaf node, create a new node
 *    to store the element and return it.
 * 3) When the recursive call returns, update the son (left or right) to
 *    point to the returned node. After case 2 happens, this means updating
 *    a NULL pointer to now point to the newly created node.
*/
static struct add_rec_ret add_rec(BSNode *node, void *element, comparator_function_t cmp, size_t size){
	if (node == NULL){
		BSNode *aux = init_node(element, size);
		if (!aux){
			return (struct add_rec_ret) {aux, ALLOCATION_ERROR};
		} else {
			return (struct add_rec_ret) {aux, 1};
		}
	}
	struct add_rec_ret ret;
	int c = (*cmp) (element, node->info);
	if (c > 0){
		ret = add_rec(node->right, element, cmp, size);
		node->right = ret.node;
	}else if (c < 0){
		ret = add_rec(node->left, element, cmp, size);
		node->left = ret.node;
	}else {
		return (struct add_rec_ret) {node, REPEATED_ELEMENT_ERROR};
	}

	ret.node->father = node;
	ret.node = node;
	return ret;
}

int bst_add(BSTree *tree, void *element){
	if (!tree || !element){
		printerr_null_param();
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
		printerr_null_param();
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

// Auxiliar struct for the remove_rec function
struct remove_rec_ret {
	BSNode* node;
	int status;
};

/**
 * This fucntion works like the add.
 * 1) Compares the element with the node's info.
 * 	If higher: continue with the right son
 * 	If lower: continue with the left son
 * 	Else: We found the element to delete.
 * 	   If left son exists: substitute this node with left son.
 *         If right son exists: substitute this node with right son.
 *         Else: substitute this node with the max element from the
 *               left node AND remove it from there.
 * 2) If the node is null, this means the element does not exist.
*/
static struct remove_rec_ret remove_rec(BSNode *node, void *element, comparator_function_t cmp, size_t size){
	if (node == NULL){
		return (struct remove_rec_ret){NULL, ELEMENT_NOT_FOUND_ERROR};
	}

	int c = (*cmp) (element, node->info);
	struct remove_rec_ret ret;
	if (c > 0){
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
		}else {
			aux = get_max(node->left);
			if(aux != node->left){
				aux->father->right = NULL;
			}else{
				node->left = aux->left;
			}
			memcpy(node->info, aux->info, size);
		}
		free(aux);
	}
	ret.node = node;
	return ret;
}

int bst_remove(BSTree *tree, void *element){
	if (!tree || !element){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	struct remove_rec_ret ret = remove_rec(tree->root, element, tree->compare, tree->data_size);
	if (ret.status == SUCCESS){
		tree->root = ret.node;
		tree->n_elements--;
	}
	return ret.status;
}

int bst_remove_array(BSTree *tree, void *array, size_t array_length){
	if (!tree || !array){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	while (array_length-- > 0){
		bst_remove(tree, array);
		array = void_offset(array, tree->data_size);
	}
	return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

/// GET ///////////////////////////////////////////////////////////////////

static BSNode* get_rec(BSNode *node, void *element, comparator_function_t cmp){
	if(node == NULL){
		return NULL;
	}
	int c = (*cmp) (element, node->info);
	if(c < 0){
		return get_rec(node->left, element, cmp);
	}else if(c > 0){
		return get_rec(node->right, element, cmp);
	}else{
		return node;
	}
}

void* bst_get(BSTree *tree, void* element, void *dest){
	if (!tree || !element || !dest){
		printerr_null_param();
		return NULL;
	}
	BSNode *node = get_rec(tree->root, element, tree->compare);
	if (!node){
		return NULL;
	}
	memcpy(dest, node->info, tree->data_size);
	return dest;
}

void* bst_max(BSTree *tree, void *dest){
	if (!tree || !dest){
		printerr_null_param();
		return NULL;
	}
	return bst_max_from(tree, tree->root->info, dest);
}

void* bst_min(BSTree *tree, void *dest){
	if (!tree || !dest){
		printerr_null_param();
		return NULL;
	}
	return bst_min_from(tree, tree->root->info, dest);
}

void* bst_max_from(BSTree *tree, void *element, void *dest){
	if (!tree || !element || !dest){
		printerr_null_param();
		return NULL;
	}
	BSNode *tmp = get_rec(tree->root, element, tree->compare);
	if (!tmp){
		return NULL;
	}
	tmp = get_max(tmp);
	memcpy(dest, tmp->info, tree->data_size);
	return dest;
}

void* bst_min_from(BSTree *tree, void *element, void *dest){
	if (!tree || !element || !dest){
		printerr_null_param();
		return NULL;
	}
	BSNode *tmp = get_rec(tree->root, element, tree->compare);
	if (!tmp){
		return NULL;
	}
	tmp = get_min(tmp);
	memcpy(dest, tmp->info, tree->data_size);
	return dest;
}

///////////////////////////////////////////////////////////////////////////////

/// OTHER FUNCTIONS ///////////////////////////////////////////////////////////

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
		printerr_null_param();
		return NULL;
	}
	return exists_rec(tree->root, element, tree->compare);
}

size_t bst_size(BSTree *tree){
	if (!tree){
		printerr_null_param();
		return 0; // Should we use index_t ???
	}
	return tree->n_elements;
}

bool bst_isempty(BSTree *tree){
	if (!tree){
		printerr_null_param();
		return false;
	}
	return tree->root == NULL;
}

BSTree* bst_join(BSTree *tree_1, BSTree *tree_2){
	if (!tree_1 || !tree_2){
		printerr_null_param();
		return NULL;
	}
	if (tree_1->data_size != tree_2->data_size){
		printerr("The trees have different data sizes (%zu,%zu)",, tree_1->data_size, tree_2->data_size);
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

///////////////////////////////////////////////////////////////////////////////

/// TRAVERSE //////////////////////////////////////////////////////////////////

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
 * This method is used to traverse the tree.
 * It can be done in 3 ways: in order, pre order or post order.
 * It's recursive, wich means it calls itself again with the left and right branches.
 * When it reaches a point where both left and right sons are NULL, it will return an
 * array of elements with size 1 (the element in the node).
 * After that, it just takes those arrays and start building bigger arrays out of them.
 * At the end, it returns an array with all the elements in the tree.
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
		result.status = left.status != SUCCESS ? left.status : right.status;
		free(left.elements);
		free(right.elements);
		return result;
	}

	// Create a new struct traversal_ret to agregate the traversal from left and right and this current node all in one
	result.elements_size = left.elements_size + right.elements_size + 1; // The +1 is for the element in this node
	result.elements = malloc(result.elements_size * size);
	if(!result.elements){
		printerr_allocation();
		free(result.elements);
		result.status = ALLOCATION_ERROR;
		free(left.elements);
		free(right.elements);
		return result;
	}
	result.status = SUCCESS;

	// Depending on the order paranmeter, the current node will be added before (pre order) in the middle (in order) or after (post order)
	void *tmp = result.elements;

	if(order == PRE_ORDER){
		memcpy(tmp, node->info, size);
		tmp = void_offset(tmp, size);
	}

	// Add the elements of the left
	memcpy(tmp, left.elements, size * left.elements_size);
	tmp = void_offset(tmp, size * left.elements_size);

	if(order == IN_ORDER){
		memcpy(tmp, node->info, size);
		tmp = void_offset(tmp, size);
	}

	// Add the elements of the right
	memcpy(tmp, right.elements, size * right.elements_size);
	tmp = void_offset(tmp, size * right.elements_size);

	if(order == POST_ORDER){
		memcpy(tmp, node->info, size);
	}

	// Free the left and right arrays because our result.data already stores the elements thus this two arrays are useless now
	free(left.elements);
	free(right.elements);
	return result;
}

void* bst_preorder(BSTree *tree){
	if (!tree){
		printerr_null_param();
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
		printerr_null_param();
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
		printerr_null_param();
		return NULL;
	}
	struct traversal_ret result = traversal_rec(tree->root, POST_ORDER, tree->data_size);
	if (result.status != SUCCESS){
		return NULL;
	}
	return result.elements;
}

///////////////////////////////////////////////////////////////////////////////

/// FREE //////////////////////////////////////////////////////////////////////

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
		return NULL_PARAMETER_ERROR;
	}
	free_rec(tree->root);
	free(tree);
	return 1;
}

void bst_free_all(unsigned int n, ...){
	va_list arg;
	va_start(arg, n);
	for (unsigned int i = 0; i < n; i++){
		BSTree *ptr = va_arg(arg, BSTree*);
		bst_free(ptr);
	}
	va_end(arg);
}

BSTree* bst_reset(BSTree *tree){
	if (!tree){
		return NULL;
	}
	free_rec(tree->root);
	tree->root = NULL;
	tree->n_elements = 0;
	return tree;
}