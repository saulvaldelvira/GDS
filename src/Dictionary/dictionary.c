/**
 *  Copyright (C) 2023 - Saúl Valdelvira
 *
 *  This program is free software. You can modify
 *  and/or redistribute it under the terms of the
 *  GNU General Public License version 2, or newer.
 *  See <https://www.gnu.org/licenses/>
 *
 *  Email: saulvaldelvira@gmail.com
 *  Version: 18-03-2023
 */
#include "dictionary.h"
#include "../Util/error.h"
#include "../Vector/vector.h"
#include "../Util/definitions.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct DictionaryNode {
        void *key;
        void *value;
        enum {
                EMPTY, FULL, DELETED
        } state;
} DictionaryNode;

struct _Dictionary{
        size_t value_size;
        size_t key_size;
        Vector *elements;
        size_t n_elements;
        size_t vec_size;
        size_t prev_vec_size;
        float min_lf;
        float max_lf;
        enum Redispersion redispersion;
        hash_function_t hash;
};

#define LF(x,B) ((x) * 1.0 / (B))

/// PRIME //////////////////////////////////////////////////////////////////////

bool is_prime(int n){
        for (int i = n-1; i > 1; --i){
                if (n % i == 0){
                        return false;
                }
        }
        return true;
}

int get_prev_prime(int n){
        do{
                --n;
        }while(!is_prime(n));
        return n;
}

int get_next_prime(int n){
        do{
                ++n;
        }while(!is_prime(n));
        return n;
}

/// INITIALIZE /////////////////////////////////////////////////////////////////

#define INITIAL_SIZE 11

/**
 * Initializes a node.
*/
int init_node(void *node, void *args){
        (void) args;
        DictionaryNode *n = (DictionaryNode*) node;
        n->key = NULL;
        n->value = NULL;
        n->state = EMPTY;
        return SUCCESS;
}

/**
 * Frees the memory allocated for a node.
*/
int free_node(void *node, void *args){
        (void) args;
        DictionaryNode *n = (DictionaryNode*) node;
        free(n->key);
        free(n->value);
        n->key = NULL;
        n->value = NULL;
        return SUCCESS;
}

/**
 * An useless comparator function.
 * This dictionary uses a Vector to store the elements, so
 * we need this function to construct one, even though we are
 * never going to compare elements inside the vector.
*/
int compare_allways_equal(const void *e_1, const void *e_2) {
        (void) e_1;
        (void) e_2;
        return 0;
}

Dictionary* dict_init(size_t key_size, size_t value_size, hash_function_t hash_func){
        if (!hash_func){
                printerr_null_param(dict_init);
                return NULL;
        }
        if (key_size == 0 || value_size == 0){
                printerr_data_size(dict_init);
                return NULL;
        }
        Dictionary *dict = malloc(sizeof(*dict));
        if (!dict){
                printerr_allocation(dict_init);
                return NULL;
        }
        dict->value_size = value_size;
        dict->key_size = key_size;
        dict->min_lf = DICT_DEF_MIN_LF;
        dict->max_lf = DICT_DEF_MAX_LF;
        dict->elements = vector_init(sizeof(DictionaryNode), INITIAL_SIZE, compare_allways_equal);
        if (!dict->elements){
                free(dict);
                return NULL;
        }
        vector_reserve(dict->elements, INITIAL_SIZE);

        dict->n_elements = 0;

        int status = vector_process(dict->elements, init_node, NULL);
        if (status != SUCCESS){
                vector_free(dict->elements);
                free(dict);
                return NULL;
        }
        dict->hash = hash_func;
        dict->redispersion = DICT_DEF_REDISPERSION;
        dict->vec_size = INITIAL_SIZE;
        dict->prev_vec_size = get_prev_prime(INITIAL_SIZE);
        return dict;
}

int dict_configure(Dictionary *dict, enum Redispersion redispersion, double min_lf, double max_lf, hash_function_t hash_func){
        if (!dict){
                printerr_null_param(dict_configure);
                return NULL_PARAMETER_ERROR;
        }
        float min, max;
        if (min_lf == 0.0f){
                min = dict->min_lf;
        }else if (min_lf == DICT_NO_SHRINKING){
                min = DICT_NO_SHRINKING;
        }else{
                min = min_lf;
        }

        if (max_lf == 0.0f){
                max = dict->max_lf;
        }else{
                max = max_lf;
        }

        if (min >= max){
                printerr(dict_configure, "Invalid values for min_lf (%f) and max_lf (%f). min_lf must be lower than max_lf",, min, max);
                return INVALID_PARAMETER_ERROR;
        }

        dict->min_lf = min;
        dict->max_lf = max;

        if (redispersion >= 0){
                dict->redispersion = redispersion;
        }
        if (hash_func){
                dict->hash = hash_func;
        }
        return SUCCESS;
}

//// REDISPERSE ///////////////////////////////////////////////////////////////

/**
 * Helper function for redisperse method
*/
static int copy_elements(void *element, void *args){
        DictionaryNode *e = (DictionaryNode*) element;
        DictionaryNode **tail = (DictionaryNode**) args;
        if (e->state == FULL && e->key && e->value){
                (*tail)->key = e->key;
                (*tail)->value = e->value;
                (*tail)++;
        }else{
                free(e->key);
                free(e->value);
        }
        return SUCCESS;
}

/**
 * Redisperses the dictionary.
 * Can be used to expand or shrink the table.
*/
static int dict_redisperse(Dictionary *dict, size_t new_size){
        // Save the current elements (only the nodes with info, FULL)
        DictionaryNode *elements = (DictionaryNode*) calloc(dict->n_elements, sizeof(*elements));
        DictionaryNode *tail = elements;
        if (!elements){
                return ERROR;
        }
        int status = vector_process(dict->elements, copy_elements, &tail);
        if (status != SUCCESS){
                return status;
        }

        /// Reset the vector
        if (new_size < dict->vec_size) {
                status = vector_free(dict->elements);
                dict->elements = vector_init(sizeof(DictionaryNode), new_size, compare_allways_equal);
                if (!dict->elements || status != SUCCESS){
                        return ERROR;
                }
        }
        status = vector_reserve(dict->elements, new_size);
        if (status != SUCCESS){
                return status;
        }
        size_t sizes[] = {dict->key_size, dict->value_size};
        status = vector_process(dict->elements, init_node, sizes);
        if (status != SUCCESS){
                return status;
        }

        // Change vec_size and n_elements
        size_t n_elements = dict->n_elements;
        dict->n_elements = 0;
        if (dict->vec_size < new_size){
                dict->prev_vec_size = dict->vec_size;
        }else{
                dict->prev_vec_size = get_prev_prime(new_size);
        }
        dict->vec_size = new_size;

        // Add the elements in the new dictionary
        for (size_t i = 0; i < n_elements; ++i){
                DictionaryNode *n = elements + i;
                status = dict_put(dict, n->key, n->value);
                // If one fails we need to free the elements array before returning
                if (status != SUCCESS){
                        while (i < n_elements){
                                n = elements + i;
                                free(n->key);
                                free(n->value);
                                ++i;
                        }
                        free(elements);
                        return status;
                }
                // Allways free the key and value pointers of the
                // nodes, since their values are COPIED to the new table
                free(n->key);
                free(n->value);
        }
        free(elements);
        return SUCCESS;
}

//// PUT //////////////////////////////////////////////////////////////////////

/**
 * Retuns an index to store the key into
 * @param n_it number of tries, to handle collisions.
*/
static size_t dict_get_pos(Dictionary *dict, void *key, size_t n_it){
        size_t pos;
        switch (dict->redispersion){
        case LINEAR:
                pos = ABS(dict->hash(key)) + n_it;
                break;
        case QUADRATIC:
                pos =  ABS(dict->hash(key)) + n_it * n_it;
                break;
        case DOUBLE_HASHING: ;
                int h1 = ABS(dict->hash(key));
                int h2 = dict->prev_vec_size - h1 % dict->prev_vec_size;
                pos = h1 + n_it * h2;
                break;
        }
        return pos % dict->vec_size;
}

int dict_put(Dictionary *dict, void *key, void *value){
        if (!dict || !key || !value){
                printerr_null_param(dict_put);
                return NULL_PARAMETER_ERROR;
        }
        size_t pos;
        DictionaryNode node;

        for (size_t i = 0; i < dict->vec_size; ++i){
                pos = dict_get_pos(dict, key, i);
                vector_get_at(dict->elements, pos, &node);

                if (node.state == FULL){
                        int64_t h1 = dict->hash(key);
                        int64_t h2 = dict->hash(node.key);
                        if (h1 == h2){
                                break;
                        }
                }else {
                        break;
                }
        }

        // If the node is empty, we need to allocate
        // memory for the key and value
        if (!node.key){
                node.key = malloc(dict->key_size);
        }
        if (!node.value){
                node.value = malloc(dict->value_size);
        }
        if (!node.key || !node.value){
                printerr_allocation(dict_put);
                return ALLOCATION_ERROR;
        }

        node.key = memcpy(node.key, key, dict->key_size);
        node.value = memcpy(node.value, value, dict->value_size);
        if (!node.key || !node.value){
                printerr_memory_op(dict_put);
                free(node.key);
                free(node.value);
                return MEMORY_OP_ERROR;
        }

        node.state = FULL;
        // Very important. We need to copy back the node to the vector.
        // Remember, the get function returns a COPY of the element.
        vector_set_at(dict->elements, pos, &node);

        // Update n_elements and resize if needed
        dict->n_elements++;
        if (LF(dict->n_elements, dict->vec_size) >= dict->max_lf){
                size_t new_size = get_next_prime(dict->vec_size * 2);
                int status = dict_redisperse(dict, new_size);
                if (status != SUCCESS){
                        printerr(dict_put, "Could not redisperse");
                        return status;
                }
        }
        return SUCCESS;
}

//// GET_EXISTS ///////////////////////////////////////////////////////////////

void* dict_get(Dictionary *dict, void *key, void *dest){
        if (!dict || !key){
                printerr_null_param(dict_get);
                return NULL;
        }
        size_t pos;
        size_t i = 0;
        DictionaryNode node;
        do {
                pos = dict_get_pos(dict, key, i);
                vector_get_at(dict->elements, pos, &node);
                if (node.state == EMPTY){
                        break;
                }
                if (node.state != DELETED){
                        int h1 = dict->hash(key);
                        int h2 = dict->hash(node.key);
                        if (h1 == h2){
                                dest = memcpy(dest, node.value, dict->value_size);
                                if(!dest){
                                        printerr_memory_op(dict_get);
                                }
                                return dest;
                        }
                }

                i++;
        }while (i < dict->vec_size);
        return NULL;
}

bool dict_exists(Dictionary *dict, void *key){
        if (!dict || !key){
                printerr_null_param(dict_exists);
                return NULL_PARAMETER_ERROR;
        }
        size_t pos;
        size_t i = 0;
        DictionaryNode node;
        do {
                pos = dict_get_pos(dict, key, i);

                vector_get_at(dict->elements, pos, &node);
                if (node.state == EMPTY){
                        break;
                }
                if (node.state != DELETED){
                        int h1 = dict->hash(key);
                        int h2 = dict->hash(node.key);
                        if (h1 == h2){
                                return true;
                        }
                }
                i++;
        }while (i < dict->vec_size);
        return false;
}

/// REMOVE ////////////////////////////////////////////////////////////////////

static int dict_delete_node(Dictionary *dict, size_t pos, DictionaryNode node){
        node.state = DELETED;
        vector_set_at(dict->elements, pos, &node);
        dict->n_elements--;
        if (dict->min_lf > 0 && LF(dict->n_elements, dict->vec_size) <= dict->min_lf){
                size_t new_size = get_prev_prime(dict->vec_size / 2);
                int status = dict_redisperse(dict, new_size);
                if (status != SUCCESS){
                        printerr(dict_delete_node, "Could not redisperse");
                        return status;
                }
        }
        return SUCCESS;
}

int dict_remove(Dictionary *dict, void *key){
        if (!dict || !key){
                printerr_null_param(dict_contains);
                return NULL_PARAMETER_ERROR;
        }
        size_t pos;
        DictionaryNode node;
        for (size_t i = 0; i < dict->vec_size; ++i){
                pos = dict_get_pos(dict, key, i);
                vector_get_at(dict->elements, pos, &node);
                if (node.state == DELETED){
                        continue;
                }
                if (node.state == EMPTY){
                        break;
                }
                int64_t h1 = dict->hash(key);
                int64_t h2 = dict->hash(node.key);
                if (h1 == h2){
                        return dict_delete_node(dict, pos, node);
                }
        }
        return ELEMENT_NOT_FOUND_ERROR;
}

/// FREE //////////////////////////////////////////////////////////////////////

int dict_free(Dictionary *dict){
        if (!dict){
                printerr_null_param(dict_free);
                return NULL_PARAMETER_ERROR;
        }
        vector_process(dict->elements, free_node, NULL);
        vector_free(dict->elements);
        free(dict);
        return SUCCESS;
}

Dictionary* dict_reset(Dictionary *dict){
        if (!dict){
                printerr_null_param(dict_reset);
                return NULL;
        }
        vector_process(dict->elements, free_node, NULL);
        vector_reserve(dict->elements, INITIAL_SIZE);
        dict->n_elements = 0;
        dict->vec_size = INITIAL_SIZE;
        dict->prev_vec_size = get_prev_prime(INITIAL_SIZE);
        return dict;
}
