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

#define MIN_N 2

#define MAX_ELEMENTS(k) ((k) - 1)
#define MIN_ELEMENTS(k) (((k) - 1) / 2)

#define CRITIC(m,n) ((m) == (n))


BTree* btree_init(size_t data_size, int K, comparator_function_t cmp){
        if (!cmp){
                printerr_null_param(btree_init);
                return NULL;
        }
        if (data_size == 0){
                printerr_data_size(btree_init);
                return NULL;
        }
        if (N <= MIN_N){
                fprintf(stderr, "ERROR: Can't build a BTree with order <= %d\n", MIN_N);
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
                return NULL;
        }
        node->n_childs = 0;
        node->n_elements = 0;
        return node;
}

struct add_rec {
        int status;
        BTreeNode *node;
        bool propagate;
        void *element;
        BTreeNode *left;
        BTreeNode *right;
};

static void right_shift_node(BTreeNode *node, int index){

}

static int find_position(BTreeNode *node, void *element, comparator_function_t compare, size_t data_size){
        for (int i = 0; i < node->n_elements; i++){
                void *tmp = void_offset(node->elements, i * data_size);
                int c = compare(element, tmp);
                if (c < 0){
                        return i;
                } else if (c > 0){
                        return i + 1;
                } else {
                        return -1;
                }
        }
}

static BTreeNode* split_node(BTreeNode *node, void *element, BTree *tree){
        int middle = (node->n_elements + 1) / 2;
        int pos = find_position(node, element, tree->compare, tree->data_size);
        BTreeNode *split = btree_init_node(tree->K, tree->data_size);
        if (!split){
                printerr_allocation(split_node);
                return NULL;
        }

        if (pos == middle){

        }else {

        }

        void *dst = void_offset(node->elements, middle)

}



static struct add_rec btree_add_rec(BTreeNode *node, BTree *tree, void *element){
        /*if (node->n_childs > 0){
                void *tmp;
                struct add_rec aux;
                for (int i=0; i < node->n_elements-1; i++){
                        tmp = void_offset(node->elements, i * tree->data_size);
                        int c_1 = (*tree->compare) (element, tmp);
                        tmp = void_offset(node->elements, (i + 1) * tree->data_size);
                        int c_2 = (*tree->compare) (element, tmp);

                        if (c_1 == 0 || c_2 == 0){
                                aux.status = REPEATED_ELEMENT_ERROR;
                                return aux;
                        }


                        if (i == 0 && c_1 < 0){
                                aux = btree_add_rec(node->childs[0], tree, element);
                        }

                        if (i == (node->n_elements - 1) && c_2 > 0){
                                aux = btree_add_rec(node->childs[MAX_CHILDS(tree->N) - 1], tree, element);
                        }

                        if (c_1 > 0 && c_2 < 0){
                                aux = btree_add_rec(node->childs[i + 1], tree, element);

                        }
                }
                if (aux.propagate){
			int i;
			for (i=0; i < node->n_elements; i++){
				tmp = void_offset(node->elements, i * tree->data_size);
				if ((*tree->compare) (element, tmp) == 0){
					break;
				}
			}
                        if (node->n_elements == M(tree->N)){

			}else {
				
			}
                }
                
        }*/

        if (node->n_childs > 0){
                // Non leaf
                BTreeNode *ret = NULL;
                int child_pos = find_position(node, element, tree->compare, tree->data_size);
                if (ret != NULL){
                        // Add the overflow to this page

                }


        }else {
                // Leaf
        }

}

int btree_add(BTree *tree, void *element){
        if (!tree || !element){
                printerr_null_param(btree_add);
                return NULL_PARAMETER_ERROR;
        }

}

int btree_remove(BTree *tree, void *element);

static BTreeNode* btree_get_node(BTreeNode *node, size_t data_size, void *element, comparator_function_t cmp, int *index){
        for ((*index) = 0; (*index) < node->n_elements; (*index)++){
                void *tmp = void_offset(node->elements, (*index) * data_size);
                int c = cmp(element, tmp);
                if (c == 0){
                        return node;
                }else if(c < 0){
                        if (node->n_childs > (*index)){
                                return btree_get_node(node->childs[(*index)], data_size, element, cmp, index);
                        }else {
                                return NULL;
                        }
                }
        }
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

}