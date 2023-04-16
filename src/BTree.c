/**
 *  Copyright (C) 2023 - Saúl Valdelvira
 *
 *  This program is free software. You can modify
 *  and/or redistribute it under the terms of the
 *  GNU General Public License version 2, or newer.
 *  See <https://www.gnu.org/licenses/>
 *
 *  Email: saulvaldelvira@gmail.com
 *  Version: 29-01-2023
 */
#include "BTree.h"
#include "./Util/error.h"
#include "./Util/definitions.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct BTreeNode {
	int n_elements;
	int n_childs; // 0 if it's a leaf
	struct BTreeNode **childs;
	byte_t elements[];
} BTreeNode;

struct _BTree {
	int K; // Max number of childs
	BTreeNode *root;
	size_t data_size;
	comparator_function_t compare;
};

#define MIN_K 2
#define MAX_ELEMENTS(k) ((k) - 1)
#define MIN_ELEMENTS(k) (((k) - 1) / 2)
#define LEAF(node) ((node)->n_childs == 0)

/// INITIALIZE ////////////////////////////////////////////////////////////////

BTree* btree_init(size_t data_size, int K, comparator_function_t cmp){
	if (!cmp){
		printerr_null_param(btree_init);
		return NULL;
	}
	if (data_size == 0){
		printerr_data_size(btree_init);
		return NULL;
	}
	if (K < MIN_K){
		fprintf(stderr, "ERROR: Can't build a BTree with order <= %d\n", MIN_K);
		return NULL;
	}

	BTree *tree = malloc(sizeof(*tree));
	if (!tree){
		printerr_allocation(btree_init);
		return NULL;
	}
	tree->compare = cmp;
	tree->data_size = data_size;
	tree->K = K;
	tree->root = NULL;
	return tree;
}

void btree_configure(BTree *tree, comparator_function_t cmp){
	if (!tree || !cmp){
		printerr_null_param(btree_configure);
		return;
	}
	tree->compare = cmp;
}

static BTreeNode* btree_init_node(int K, size_t data_size){
	BTreeNode *node = malloc(offsetof(BTreeNode, elements) + (data_size * MAX_ELEMENTS(K)));
	if (!node){
		printerr_allocation(btree_init_node);
		return NULL;
	}
	node->childs = calloc(K, sizeof(BTreeNode*));
	if (!node->childs){
		printerr_allocation(btree_init_node);
		free(node);
		return NULL;
	}
	node->n_childs = 0;
	node->n_elements = 0;
	return node;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * Auxiliar struct for the add and remove functions.
*/
struct add_remove_ret {
	int status;
	BTreeNode *node;
};

/// ADD ///////////////////////////////////////////////////////////////////////

/**
 * Tries to insert the element into the given index.
 * If the node is full returns INDEX_BOUNDS_ERROR.
*/
static int right_shift_node(BTreeNode *node, int index, BTree *tree, void *element){
	if (node->n_elements == MAX_ELEMENTS(tree->K)){
		return INDEX_BOUNDS_ERROR;
	}
	// Shift elements to the right
	void *src = void_offset(node->elements, index * tree->data_size);
	void *dst = void_offset(node->elements, (index+1) * tree->data_size);
	int n_elements_to_move = node->n_elements - index;
	dst = memmove(dst, src, n_elements_to_move * tree->data_size);

	// Insert the element
	src = memcpy(src, element, tree->data_size);
	node->n_elements++;

	if (!dst || !src){
		printerr_memory_op(right_shift_node);
		return NULL_PARAMETER_ERROR;
	}

	// Also shift the child nodes to the right
	for (int i=node->n_childs; i > index; i--){
		node->childs[i] = node->childs[i - 1];
	}
	node->childs[index] = NULL;
	return SUCCESS;
}

/**
 * Finds the position in wich the element should be inserted.
 * For non leaf nodes, this means the position of the child node to continue recursively searching in.
*/
static int find_position(BTreeNode *node, void *element, comparator_function_t compare, size_t data_size){
	void *tmp;
	int c = 0;
	for (int i = 0; i < node->n_elements; i++){
		tmp = void_offset(node->elements, i * data_size);
		c = compare(element, tmp);
		if (c < 0){
			return i;
		} else if (c == 0){
			return -1;
		}
	}
	// If it's greater than the last node, return the last index
	if (c > 0){
		return node->n_elements;
	}
	return 0;
}

/**
 * Adds an element to a full node and splits it.
 * 1) Adds the element into its position.
 * 2) Splits the node into a left and right node, and appends them to an extra node, that also contains the middle element we must propagate upwards.
 * 3) If the params left_overflow and right_overflow are NOT NULL, this means we are splitting a non leaf node, that tried to add a split node, but
 *      was full and propagated the add process a second time. In this case, we have to add left_overflow and right_overflow to the split nodes.
*/
static BTreeNode* split_node(BTreeNode *node, void *element, BTree *tree, BTreeNode *left_overflow, BTreeNode *right_overflow){
	int middle = node->n_elements / 2; // Index of the element to propagate
	bool has_overflow = left_overflow && right_overflow;
	int index = find_position(node, element, tree->compare, tree->data_size);
	BTreeNode *right = btree_init_node(tree->K, tree->data_size);
	BTreeNode *split = btree_init_node(tree->K, tree->data_size);
	BTreeNode *left  = node;
	if (!right || !split){
		printerr_allocation(split_node);
		return NULL;
	}

	right->n_elements = left->n_elements - middle; // Number of elements to copy into right
	left->n_elements = middle;
	split->n_elements = 1;
	split->n_childs = 2;

	// If there is "overflow" thus we are adding in a NON LEAF node,
	// we set the n_childs to n_elements + 1. Else we are in a leaf node,
	// wich means it has 0 childs.
	if (has_overflow){
		left->n_childs = left->n_elements + 1;
		right->n_childs = right->n_elements + 1;
	} else {
		left->n_childs = 0;
		right->n_childs = 0;
	}

	// The element to propagate is the element to add itself
	if (index == middle){
		// Add the element into the father node
		if (!memcpy(split->elements, element, tree->data_size)){
			printerr_memory_op(split_node);
			return NULL;
		}

		// Add the elements of the right node
		void *tmp = void_offset(left->elements, middle * tree->data_size);
		if (!memcpy(right->elements, tmp, right->n_elements * tree->data_size)){
			printerr_memory_op(split_node);
			return NULL;
		}

		// Split the childs arrays
		for (int i=1, m = middle; i < right->n_childs ; i++){
			right->childs[i] = left->childs[m + i];
			left->childs[m + i] = NULL;
		}

		if (has_overflow){
			left->childs[left->n_elements] = left_overflow;
			right->childs[0] = right_overflow;
		}

	}else if (index < middle){ // Add the element into the left node
		// Add the element into the father node.
		void *tmp = void_offset(node->elements, (middle-1) * tree->data_size);
		tmp = memcpy(split->elements, tmp, tree->data_size);
		if (!tmp){
			printerr_memory_op(split_node);
			return NULL;
		}

		// Add second half of element into right node
		tmp = void_offset(left->elements, middle * tree->data_size);
		tmp = memcpy(right->elements, tmp, right->n_elements * tree->data_size);
		if (!tmp){
			printerr_memory_op(split_node);
			return NULL;
		}

		// Make space for the extra element
		void *src = void_offset(left->elements, index * tree->data_size);
		void *dst = void_offset(left->elements, (index+1) * tree->data_size);
		dst = memmove(dst, src, (middle - index) * tree->data_size); // Move elements to the right

		// Insert the element into index
		src = memcpy(src, element, tree->data_size); // Add in the element in the spot
		if (!dst || !src){
			printerr_memory_op(split_node);
			return NULL;
		}

		// Split the childs arrays
		for (int i=0, m = middle; i < right->n_childs ; i++){
			right->childs[i] = left->childs[m + i];
			left->childs[m + i] = NULL;
		}

		for (int i=left->n_childs-1; i > index; i--){
			left->childs[i] = left->childs[i-1];
		}

		if (has_overflow){
			left->childs[index] = left_overflow;
			left->childs[index+1] = right_overflow;
		}

	} else {// Add the element into the right node
		// Add the middle to father
		void *tmp = void_offset(left->elements, middle * tree->data_size);
		if (!memcpy(split->elements, tmp, tree->data_size)){
			printerr_memory_op(split_node);
			return NULL;
		}

		int n; // To keep track of how many element we already added to right

		// Add everithing from [middle + 1 ] to [inserting point]
		void *src = void_offset(left->elements, (middle+1) * tree->data_size);
		int first_chunk = index - (middle+1);
		src = memcpy(right->elements, src, first_chunk * tree->data_size);
		n = first_chunk;

		// Insert the element in the index
		void *dst = void_offset(right->elements, first_chunk * tree->data_size);
		dst = memcpy(dst, element, tree->data_size);
		n++;

		if (!dst || !src){
			printerr_memory_op(split_node);
			return NULL;
		}

		// Add the rest of the elements
		src = void_offset(left->elements, index * tree->data_size);
		dst = void_offset(right->elements, n * tree->data_size);
		dst = memcpy(dst, src, (right->n_elements - n) * tree->data_size);
		if (!dst){
			printerr_memory_op(split_node);
			return NULL;
		}

		// Split the childs arrays
		for (int i=1, m = middle; i < right->n_childs ; i++){
			right->childs[i] = left->childs[m + i];
			left->childs[m + i] = NULL;
		}

		// Shift right childs to the right
		for (int i=right->n_childs-1; i > index; i--){
			right->childs[i] = right->childs[i-1];
		}

		if (has_overflow){
			index = index - middle - 1;
			right->childs[index] = left_overflow;
			right->childs[index+1] = right_overflow;
		}
	}

	// Append left and right nodes to the father (split)
	split->childs[0] = left;
	split->childs[1] = right;
	return split;
}

static struct add_remove_ret btree_add_rec(BTreeNode *node, BTree *tree, void *element){
	struct add_remove_ret ret = {.status = SUCCESS};

	// Find position to insert element
	int pos = find_position(node, element, tree->compare, tree->data_size);
	if (pos < 0){
		printerr(btree_add, "The element %p already exists\n",, element);
		ret.status = REPEATED_ELEMENT_ERROR;
		return ret;
	}
	// If we are NOT in a leaf node
	if (node->n_childs > 0){
		// Add recursively
		ret = btree_add_rec(node->childs[pos], tree, element);
		// If there was "overflow" and we need to add it
		if (ret.status == SUCCESS && ret.node != NULL){
			pos = find_position(node, ret.node->elements, tree->compare, tree->data_size);
			ret.status = right_shift_node(node, pos, tree, ret.node->elements);

			// If there is not size in this node (n_elements == max)
			if (ret.status == INDEX_BOUNDS_ERROR){
				// Split the node
				BTreeNode *aux = split_node(node, ret.node->elements, tree, ret.node->childs[0], ret.node->childs[1]);
				free(ret.node->childs);
				free(ret.node);
				ret.node = aux;
				ret.status = SUCCESS;
			}else if (ret.status == SUCCESS){
				node->n_childs++;
				node->childs[pos] = ret.node->childs[0];
				node->childs[pos+1] = ret.node->childs[1];
				free(ret.node->childs);
				free(ret.node);
				ret.node = NULL;
			}

		}
	}else { // Leaf node
		ret.status = right_shift_node(node, pos, tree, element);
		// If no size, split the node
		if (ret.status == INDEX_BOUNDS_ERROR){
			ret.node = split_node(node, element, tree, NULL, NULL);
			ret.status = SUCCESS;
		}else{
			ret.node = NULL;
		}
	}
	return ret;
}

static int add_to_node(BTreeNode *node, BTree *tree, void *element){
	int pos = find_position(node, element, tree->compare, tree->data_size);
	return right_shift_node(node, pos, tree, element);
}

int btree_add(BTree *tree, void *element){
	if (!tree || !element){
		printerr_null_param(btree_add);
		return NULL_PARAMETER_ERROR;
	}
	if (tree->root == NULL){
		tree->root = btree_init_node(tree->K, tree->data_size);
		if (!tree->root){
			printerr_allocation(btree_add);
			return ALLOCATION_ERROR;
		}
	}
	struct add_remove_ret ret = btree_add_rec(tree->root, tree, element);
	if (ret.node != NULL){
		tree->root = ret.node;
	}
	return ret.status;
}

int btree_add_array(BTree *tree, void *array, size_t array_size){
	if (!tree || !array){
		printerr_null_param(btree_add_array);
		return NULL_PARAMETER_ERROR;
	}
	for (size_t i = 0; i < array_size; ++i){
		void *tmp = void_offset(array, i * tree->data_size);
		int ret = btree_add(tree, tmp);
		if (ret != SUCCESS){
			return ret;
		}
	}
	return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

/// REMOVE ////////////////////////////////////////////////////////////////////

/**
 * Returns the position of the element, or -1 if it's not in this node.
*/
static int find_element(BTreeNode *node, void *element, comparator_function_t compare, size_t data_size){
	void *tmp;
	for (int i = 0; i < node->n_elements; i++){
		tmp = void_offset(node->elements, i * data_size);
		int c = compare(element, tmp);
		if (c == 0){
			return i;
		}
	}
	return -1;
}

/**
 * Copies into dest the smallest element in
 * the leftmos leaf node and deletes it from there.
*/
static int get_min_and_delete(BTreeNode *node, void *dest, size_t data_size){
	if (node == NULL){
		return NULL_PARAMETER_ERROR;
	}
	if (LEAF(node)){
		void *tmp = memcpy(dest, node->elements, data_size);
		if (!tmp){
			printerr_memory_op(get_min_and_delete);
			return MEMORY_OP_ERROR;
		}
		node->n_elements--;

		tmp = void_offset(node->elements, 1 * data_size);
		tmp = memmove(node->elements, tmp, node->n_elements * data_size);
		if (!tmp){
			printerr_memory_op(get_min_and_delete);
			return MEMORY_OP_ERROR;
		}

		return SUCCESS;
	}else {
		return get_min_and_delete(node->childs[0], dest, data_size);
	}
}

/**
 * Copies into dest the biggest element in
 * the rightmos leaf node and deletes it from there.
*/
static int get_max_and_delete(BTreeNode *node, void *dest, size_t data_size){
	if (node == NULL){
		return NULL_PARAMETER_ERROR;
	}
	if (node->n_childs == 0){
		void *tmp = void_offset(node->elements, (node->n_elements - 1) * data_size);
		tmp = memcpy(dest, tmp, data_size);
		if (!tmp){
			printerr_memory_op(get_max_and_delete);
			return MEMORY_OP_ERROR;
		}
		node->n_elements--;
		return SUCCESS;
	}else {
		return get_max_and_delete(node->childs[node->n_childs - 1], dest, data_size);
	}
}

/**
 * Removes the element at index pos from the node.
*/
static int remove_element(BTreeNode *node, size_t data_size, int pos){
	if (pos < node->n_elements - 1){
		void *src = void_offset(node->elements, (pos + 1) * data_size);
		void *dst = void_offset(node->elements, pos * data_size);
		int n = node->n_elements - pos - 1;
		dst = memmove(dst, src, n * data_size);
		if (!dst){
			printerr_memory_op(remove_element);
			return MEMORY_OP_ERROR;
		}
	}
	if (node->n_childs > 0){

		for (int i = pos+1; i < node->n_childs - 1; i++){
			node->childs[i] = node->childs[i+1];
		}
		node->n_childs--;
	}
	node->n_elements--;
	return SUCCESS;
}

/**
 * Merges the left and right nodes.
 * mid_element in the father's element which is going down, and
 * it's inserted in between left's own elements and right's.
 * The merge is done INTO the left node.
*/
static void merge_nodes(BTreeNode **left, void *mid_element, BTreeNode **right, size_t data_size){
	// Copy the mid element into left
	void *dst = void_offset((*left)->elements, (*left)->n_elements * data_size);
	dst = memcpy(dst, mid_element, data_size);
	if (!dst){
		printerr_memory_op(merge_nodes);
		return;
	}
	(*left)->n_elements++;

	// Move dst one position
	dst = void_offset(dst, data_size);

	// Copy right's elements to the node.
	dst = memcpy(dst, (*right)->elements, (*right)->n_elements * data_size);
	if (!dst){
		printerr_memory_op(merge_nodes);
		return;
	}
	(*left)->n_elements += (*right)->n_elements;

	// Copy right's childs to the node
	int p = (*left)->n_childs;
	for (int i = 0; i < (*right)->n_childs; i++){
		(*left)->childs[p + i] = (*right)->childs[i];
	}
	(*left)->n_childs += (*right)->n_childs;

	free((*right)->childs);
	free(*right);
	*right = NULL;
}

/**
 * Returns a pointer to the rightmos leaf node,
 * only IF that node is NOT in a critical situation.
 * This means if the node can transfer elements
 * to a brother.
*/
static BTreeNode* borrow_max_node(BTreeNode *node, int K){
	if (LEAF(node)){
		if (node->n_elements <= MIN_ELEMENTS(K)){
			return NULL;
		}else {
			return node;
		}
	}else {
		return borrow_max_node(node->childs[node->n_childs-1], K);
	}
}

/**
 * Returns a pointer to the leftmos leaf node,
 * only IF that node is NOT in a critical situation.
 * This means if the node can transfer elements
 * to a brother.
*/
static BTreeNode* borrow_min_node(BTreeNode *node, int K){
	if (LEAF(node)){
		if (node->n_elements <= MIN_ELEMENTS(K)){
			return NULL;
		}else {
			return node;
		}
	}else {
		return borrow_min_node(node->childs[0], K);
	}
}

/**
 * Copies the maximun element from the rightmos leaf node into dest.
*/
static void* copy_max_element(BTreeNode *node, int K, size_t data_size, void *dest){
	if (LEAF(node)){
		void *max = void_offset(node->elements, (node->n_elements - 1) * data_size);
		dest = memcpy(dest, max, data_size);
		if (!dest){
			printerr_memory_op(copy_max_element);
			return NULL;
		}
		return dest;
	}else {
		return copy_max_element(node->childs[node->n_childs-1], K, data_size, dest);
	}
}

/**
 * Request to the father node to borrow an element to node from it's brothers.
 * If no brother node is in a situation to do so, it merges the node with one
 * of it's brothers.
*/
static BTreeNode* request_or_merge(BTreeNode *node, BTreeNode *father, BTree *tree){
	// Find the position of node in the father's childs array
	int father_pos = -1;
	for (int i = 0; i < father->n_childs; ++i){
		if (father->childs[i] == node){
			father_pos = i;
		}
	}

	BTreeNode **left_sibling = father_pos > 0 ? &father->childs[father_pos - 1] : NULL;
	BTreeNode **right_sibling = father_pos < (father->n_childs - 1) ? &father->childs[father_pos + 1] : NULL;

	BTreeNode *max_left = NULL;
	BTreeNode *min_right = NULL;
	// Element to move down to the merged node if necessary.
	void *element_down = father_pos > 0
					    ? void_offset(father->elements, (father_pos - 1) * tree->data_size)
					    : father->elements;
	// Ask brothers for an element
	if (left_sibling && (max_left = borrow_max_node(*left_sibling, tree->K))){
		add_to_node(node, tree, element_down);
		get_max_and_delete(max_left, element_down, tree->data_size);

	} else if (right_sibling && (min_right = borrow_min_node(*right_sibling, tree->K))){
		element_down = void_offset(element_down, tree->data_size); // Fetch the next element (the father of the right child)
		add_to_node(node, tree, element_down);
		get_min_and_delete(min_right, element_down, tree->data_size);

	} else { // Merge
		if (left_sibling){
			merge_nodes(left_sibling, element_down, &node, tree->data_size);
			remove_element(father, tree->data_size, father_pos-1);
			return *left_sibling;
		}else if (right_sibling){
			if (father_pos > 0){
				element_down = void_offset(element_down, tree->data_size); // Fetch the next element (the father of the right child)
			}
			merge_nodes(&node, element_down, right_sibling, tree->data_size);
			remove_element(father, tree->data_size, father_pos);
		}
	}
	return node;
}

static struct add_remove_ret btree_remove_rec(BTreeNode *node, BTreeNode *father, BTree *tree, void *element){
	int pos = find_element(node, element, tree->compare, tree->data_size);
	struct add_remove_ret ret = {.status = SUCCESS};
	// If the element is NOT on this node, recurse into the corresponding child.
	if (pos < 0){
		pos = find_position(node, element, tree->compare, tree->data_size);
		if (pos < 0){
			ret.status = pos;
			return ret;
		}
		if (node->n_childs == 0){
			return (struct add_remove_ret){
				.node = node,
				.status = ELEMENT_NOT_FOUND_ERROR
			};
		}
		ret = btree_remove_rec(node->childs[pos], node, tree, element);
		if (node == tree->root && node->n_elements == 0){
			free(node->childs);
			free(node);
			node = ret.node;
		} else if (node != tree->root && node->n_elements < MIN_ELEMENTS(tree->K)){
			node = request_or_merge(node, father, tree);
		}
	}else {
		if (LEAF(node)){ // If we're in a leaf node
			// And is NOT in a critical situation, simply delete the element
			if (node->n_elements > MIN_ELEMENTS(tree->K)){
				ret.status = remove_element(node, tree->data_size, pos);
			}else if (node == tree->root){
				// The same if it's the root, regardless of the number of elements
				if (node->n_elements == 1){
					// If removing the only element in the tree
					free(node->childs);
					free(node);
					node = NULL;
				} else {
					ret.status = remove_element(node, tree->data_size, pos);
				}
			} else {
				ret.status = remove_element(node, tree->data_size, pos);
				node = request_or_merge(node, father, tree);
			}
		}else { // Get an element from it's childs into the position of the element to delete
			BTreeNode *left_child = borrow_max_node(node->childs[pos], tree->K);
			BTreeNode *right_child;
			void *element_to_delete = void_offset(node->elements, pos * tree->data_size);

			if (left_child){
				get_max_and_delete(left_child, element_to_delete, tree->data_size);
			}else if ((right_child = borrow_min_node(node->childs[pos + 1], tree->K))){
				get_min_and_delete(right_child, element_to_delete, tree->data_size);
			}else {
				// If both childs are in a critic situation, get an element from the left
				// child and start a recursive remove call to reorganize the branch.
				void *tmp = copy_max_element(node->childs[pos], tree->K, tree->data_size, element_to_delete);
				if (!tmp){
					ret.status = MEMORY_OP_ERROR;
				} else {
					ret.status = btree_remove_rec(node->childs[pos], node, tree, tmp).status;
				}
			}
		}
	}
	ret.node = node;
	return ret;
}

int btree_remove(BTree *tree, void *element){
	if (!tree || !element){
		printerr_null_param(btree_remove);
		return NULL_PARAMETER_ERROR;
	}
	struct add_remove_ret ret = btree_remove_rec(tree->root, NULL, tree, element);
	tree->root = ret.node;
	return ret.status;
}

int btree_remove_array(BTree *tree, void *array, size_t array_size){
	if (!tree || !array){
		printerr_null_param(btree_remove_array);
		return NULL_PARAMETER_ERROR;
	}
	for (size_t i = 0; i < array_size; ++i){
		void *tmp = void_offset(array, i * tree->data_size);
		int ret = btree_remove(tree, tmp);
		if (ret != SUCCESS){
			return ret;
		}
	}
	return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

/// GET-EXISTS ////////////////////////////////////////////////////////////////

static BTreeNode* btree_get_node(BTreeNode *node, size_t data_size, void *element, comparator_function_t cmp, int *index){
	if (node == NULL){
		return NULL;
	}
	void *tmp = node->elements;
	for ((*index) = 0; (*index) < node->n_elements; (*index)++){
		int c = cmp(element, tmp);
		if (c == 0){
			return node;
		}else if(c < 0){
			return btree_get_node(node->childs[(*index)], data_size, element, cmp, index);
		}else if (c > 0 && (*index) == node->n_elements-1){
			return btree_get_node(node->childs[node->n_elements], data_size, element, cmp, index);
		}
		tmp = void_offset(tmp, data_size);
	}
	return NULL;
}

void* btree_get(BTree *tree, void *element, void *dest){
	if (!tree || !element || !dest){
		printerr_null_param(btree_get);
		return false;
	}
	int index;
	BTreeNode *node = btree_get_node(tree->root, tree->data_size, element, tree->compare, &index);
	if (!node){
		return NULL;
	}
	void *tmp = void_offset(node->elements, index * tree->data_size);
	dest = memcpy(dest, tmp, tree->data_size);
	if(!dest){
		printerr_memory_op(btree_get);
	}
	return dest;
}

bool btree_exists(BTree *tree, void *element){
	if (!tree || !element){
		printerr_null_param(btree_exists);
		return false;
	}
	int tmp;
	return btree_get_node(tree->root, tree->data_size, element, tree->compare, &tmp) != NULL;
}

/// FREE //////////////////////////////////////////////////////////////////////

static void btree_free_node(BTreeNode *node){
	if (!node){
		return;
	}
	for (int i = 0; i < node->n_childs; i++){
		btree_free_node(node->childs[i]);
	}
	free(node->childs);
	free(node);
}

int btree_free(BTree *tree){
	if (!tree){
		printerr_null_param(btree_free);
		return NULL_PARAMETER_ERROR;
	}
	btree_free_node(tree->root);
	free(tree);
	return SUCCESS;
}

BTree* btree_reset(BTree *tree){
	if (!tree){
		printerr_null_param(btree_reset);
		return NULL;
	}
	btree_free_node(tree->root);
	tree->root = NULL;
	return tree;
}
