#include "BTree.h"
#include "../Util/error.h"
#include "../Util/definitions.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

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

static int right_shift_node(BTreeNode *node, int index, BTree *tree, void *element){
        if (node->n_elements == MAX_ELEMENTS(tree->K)){
                return INDEX_BOUNDS_ERROR;
        }
        void *src = void_offset(node->elements, index * tree->data_size);
        void *dst = void_offset(node->elements, (index+1) * tree->data_size);
        int n_elements_to_move = node->n_elements - index;
        dst = memmove(dst, src, n_elements_to_move * tree->data_size);

        src = memcpy(src, element, tree->data_size);
        node->n_elements++;
        
        if (!dst || !src){
                printerr_memory_op(right_shift_node);
                return NULL_PARAMETER_ERROR;
        }
        
        for (int i=n_elements_to_move; i > 0; i--){
                node->childs[index + i] = node->childs[index + i - 1];
        }
        // node->childs[index] = NULL ??? 
        return SUCCESS;
}

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
        if (c > 0){
                return node->n_elements;
        }
        return 0;
}

static BTreeNode* split_node(BTreeNode *node, void *element, BTree *tree, BTreeNode *left_overflow, BTreeNode *right_overflow){
        int middle = node->n_elements / 2;
        bool has_overflow = left_overflow && right_overflow;
        int index = find_position(node, element, tree->compare, tree->data_size);
        BTreeNode *right = btree_init_node(tree->K, tree->data_size);
        BTreeNode *split = btree_init_node(tree->K, tree->data_size);
        if (!right || !split){
                printerr_allocation(split_node);
                return NULL;
        }

        right->n_elements = node->n_elements - middle; // Number of elements to copy into right
        node->n_elements = middle;
        split->n_elements = 1;
        split->n_childs = 2;

        // If there is "overflow" thus we are adding in a NON LEAF node,
        // we set the n_childs to n_elements + 1. Else we are in a leaf node, 
        // wich means it has 0 childs.
        if (has_overflow){
                node->n_childs = node->n_elements + 1;
                right->n_childs = right->n_elements + 1;
        } else {
                node->n_childs = 0;
                right->n_childs = 0;
        }

        if (index == middle){
                // Add the element into the father node
                if (!memcpy(split->elements, element, tree->data_size)){
                        printerr_memory_op(split_node);
                        return NULL;
                }
                
                // Add the elements of the right node
                void *tmp = void_offset(node->elements, middle * tree->data_size);
                if (!memcpy(right->elements, tmp, right->n_elements * tree->data_size)){
                        printerr_memory_op(split_node);
                        return NULL;
                }

                // Add the child-nodes to the sub-nodes
                for (int i=0, m = middle+1; i < right->n_childs ; i++){
                        right->childs[i] = node->childs[m + i];
                        node->childs[m + i] = NULL;
                }

                if (has_overflow){
                        node->childs[node->n_elements] = left_overflow;
                        right->childs[0] = right_overflow;
                }

        }else if (index < middle){
                // Add the element into the father node.
                void *tmp = void_offset(node->elements, (middle-1) * tree->data_size);
                tmp = memcpy(split->elements, tmp, tree->data_size);
                if (!tmp){
                        printerr_memory_op(split_node);
                        return NULL;
                }

                // Add second half of element into right node
                tmp = void_offset(node->elements, middle * tree->data_size);
                tmp = memcpy(right->elements, tmp, right->n_elements * tree->data_size);
                if (!tmp){
                        printerr_memory_op(split_node);
                        return NULL;
                }

                // Make space for the extra element
                void *src = void_offset(node->elements, index * tree->data_size);
                void *dst = void_offset(node->elements, (index+1) * tree->data_size);
                dst = memmove(dst, src, (middle - index) * tree->data_size); // Move elements to the right
                
                // Insert the element into index
                src = memcpy(src, element, tree->data_size); // Add in the element in the spot
                if (!dst || !src){
                        printerr_memory_op(split_node);
                        return NULL;
                }
                
                // Add the child-nodes to the sub-nodes
                for (int i=0, m = middle; i < right->n_childs ; i++){
                        right->childs[i] = node->childs[m + i];
                        node->childs[m + i] = NULL;
                }


                for (int i=node->n_childs-1; i > index; i--){
                        node->childs[i] = node->childs[i-1];
                }

                if (has_overflow){
                        node->childs[index] = left_overflow;
                        node->childs[index+1] = right_overflow;
                }

        } else {
                // Add element to father
                void *tmp = void_offset(node->elements, middle * tree->data_size);
                if (!memcpy(split->elements, tmp, tree->data_size)){
                        printerr_memory_op(split_node);
                        return NULL;
                }

                int n; // To keep track of how many element we already added to right

                // Add everithing from [middle + 1 ] to [inserting point]
                void *src = void_offset(node->elements, (middle+1) * tree->data_size);
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
                src = void_offset(node->elements, index * tree->data_size);
                dst = void_offset(right->elements, n * tree->data_size);
                dst = memcpy(dst, src, (right->n_elements - n) * tree->data_size);
                if (!dst){
                        printerr_memory_op(split_node);
                        return NULL;
                }

                // Add the child-nodes to the sub-nodes
                for (int i=0, m = middle+1; i < right->n_childs ; i++){
                        right->childs[i] = node->childs[m + i];
                        node->childs[m + i] = NULL;
                }

                for (int i=right->n_childs-1; i > index; i--){
                        node->childs[i] = node->childs[i-1];
                }

                if (has_overflow){
                        index = index - middle - 1;
                        right->childs[index] = left_overflow;
                        right->childs[index+1] = right_overflow;
                }
        }

        

        // Append left (node) and right to the father (split)
        split->childs[0] = node;
        split->childs[1] = right;
        return split;
}

struct add_rec {
        int status;
        BTreeNode *node;
};

static struct add_rec btree_add_rec(BTreeNode *node, BTree *tree, void *element){
        struct add_rec ret;

        // Find position to insert element
        int pos = find_position(node, element, tree->compare, tree->data_size);
        if (pos < 0){
                ret.status = REPEATED_ELEMENT_ERROR;
                return ret;
        }
        
        // If we are NOT in a leaf node
        if (node->n_childs > 0){
                // Add recursively
                ret = btree_add_rec(node->childs[pos], tree, element);
                // If there was "overflow" and we need to add it
                if (ret.node != NULL){
                        pos = find_position(node, ret.node->elements, tree->compare, tree->data_size);
                        ret.status = right_shift_node(node, pos, tree, ret.node->elements);

                        // If there is not size in this node (n_elements == max)
                        if (ret.status == INDEX_BOUNDS_ERROR){
                                // Split the node
                                BTreeNode *aux = split_node(node, ret.node->elements, tree, ret.node->childs[0], ret.node->childs[1]);
                                free(ret.node->childs);
                                free(ret.node);
                                ret.node = aux;

                        }else{
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
                }else{
                        ret.node = NULL;
                }
        }
        return ret;
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
        struct add_rec ret = btree_add_rec(tree->root, tree, element);
        if (ret.node != NULL){
                tree->root = ret.node;
        }
        return ret.status;
}

int btree_remove(BTree *tree, void *element);

static BTreeNode* btree_get_node(BTreeNode *node, size_t data_size, void *element, comparator_function_t cmp, int *index){
        if (node == NULL){
                return NULL;
        }
        for ((*index) = 0; (*index) < node->n_elements; (*index)++){
                void *tmp = void_offset(node->elements, (*index) * data_size);
                int c = cmp(element, tmp);
                if (c == 0){
                        return node;
                }else if(c < 0){
                        return btree_get_node(node->childs[(*index)], data_size, element, cmp, index);
                }else if (c > 0 && (*index) == node->n_elements-1) {
                        return btree_get_node(node->childs[node->n_elements], data_size, element, cmp, index);
                }
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

static void btree_free_node(BTreeNode *node, int K){
        if (!node){
                return;
        }
        for (int i = 0; i < K; i++){
                btree_free_node(node->childs[i], K);
        }
        free(node->childs);
        free(node);
}

int btree_free(BTree *tree){
        if (!tree){
                printerr_null_param(btree_free);
                return NULL_PARAMETER_ERROR;
        }
        btree_free_node(tree->root, tree->K);
        free(tree);
        return SUCCESS;
}

BTree* btree_reset(BTree *tree){
        if (!tree){
                printerr_null_param(btree_reset);
                return NULL;
        }
        btree_free_node(tree->root, tree->K);
        tree->root = NULL;
        return tree;
}