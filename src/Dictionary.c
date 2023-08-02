/**
 *  Copyright (C) 2023 - Saúl Valdelvira
 *  License: BSD 3-Clause
 *  Email: saulvaldelvira@gmail.com
 */
#include "Dictionary.h"
#define GDS_ENABLE_ERROR_MACROS
#include "./util/error.h"
#include "./Vector.h"
#include "./util/definitions.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

typedef struct DictionaryNode {
        void *key;
        void *value;
        enum {
                EMPTY, FULL, DELETED
        } state;
} DictionaryNode;

struct Dictionary{
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
	destructor_function_t destructor;
};

#define LF(x,B) ((x) * 1.0 / (B))
#define DICT_INITIAL_SIZE 11

/// PRIME //////////////////////////////////////////////////////////////////////

static bool is_prime(int n){
        for (int i = n-1; i > 1; --i){
                if (n % i == 0)
                        return false;
	}
        return true;
}

static int get_prev_prime(int n){
        do{
                --n;
        }while(!is_prime(n));
        return n;
}

static int get_next_prime(int n){
        do{
                ++n;
        }while(!is_prime(n));
        return n;
}

/// INITIALIZE /////////////////////////////////////////////////////////////////

/**
 * Initializes a node.
 */
static int init_node(void *node, void *args){
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
static int free_node(void *node, void *args){
        destructor_function_t destructor = * (destructor_function_t*) args;
        DictionaryNode *n = (DictionaryNode*) node;
	if (destructor && n->value)
		destructor(n->value);
        free(n->key);
        free(n->value);
        n->key = NULL;
        n->value = NULL;
        return SUCCESS;
}

Dictionary* dict_init(size_t key_size, size_t value_size, hash_function_t hash_func){
        if (!hash_func){
                printerr_null_param();
                return NULL;
        }
        if (key_size == 0 || value_size == 0){
                printerr_data_size();
                return NULL;
        }
        Dictionary *dict = malloc(sizeof(*dict));
        assert(dict);
        dict->value_size = value_size;
        dict->key_size = key_size;
        dict->min_lf = DICT_DEF_MIN_LF;
        dict->max_lf = DICT_DEF_MAX_LF;
	dict->destructor = NULL;	
        dict->elements = vector_init(sizeof(DictionaryNode), compare_equal);
        assert(dict->elements);
        vector_reserve(dict->elements, DICT_INITIAL_SIZE);
        dict->n_elements = 0;
        int status = vector_process(dict->elements, init_node, NULL);
        if (status != SUCCESS){
                vector_free(dict->elements);
                free(dict);
                return NULL;
        }
        dict->hash = hash_func;
        dict->redispersion = DICT_DEF_REDISPERSION;
        dict->vec_size = DICT_INITIAL_SIZE;
        dict->prev_vec_size = get_prev_prime(DICT_INITIAL_SIZE);
        return dict;
}

int dict_configure(Dictionary *dict, enum Redispersion redispersion, double min_lf, double max_lf, hash_function_t hash_func){
        if (!dict){
                printerr_null_param();
                return NULL_PARAMETER_ERROR;
        }
        float min, max;
        if (min_lf > 0.0f || min_lf == DICT_NO_SHRINKING)
                min = min_lf;
        else
                min = dict->min_lf;

        if (max_lf > 0.0f)
                max = max_lf;
        else
                max = dict->max_lf;

        if (min >= max){
                printerr("Invalid values for min_lf (%f) and max_lf (%f). min_lf must be lower than max_lf",, min, max);
                return INVALID_PARAMETER_ERROR;
        }

        dict->min_lf = min;
        dict->max_lf = max;

        if (redispersion >= 0)
                dict->redispersion = redispersion;
        if (hash_func)
                dict->hash = hash_func;

        return SUCCESS;
}

void dict_set_destructor(Dictionary *dict, destructor_function_t value_destructor){
	if (!dict)
		printerr_null_param();
	else
		dict->destructor = value_destructor;
}
//// REDISPERSE ///////////////////////////////////////////////////////////////

/**
 * Helper function for redisperse method
 */
static int copy_elements(void *element, void *args){
        DictionaryNode *e = (DictionaryNode*) element;
	void **argv = (void**) args;
	DictionaryNode **tail = (DictionaryNode**) argv[0];
        if (e->state == FULL && e->key && e->value){
                (*tail)->key = e->key;
                (*tail)->value = e->value;
                (*tail)++;
        }else{
		destructor_function_t destructor = * (destructor_function_t*) argv[1];
		if (destructor)
			destructor(e->value);
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
        int status = vector_process(dict->elements, copy_elements, (void*[]){&tail, &dict->destructor});
        if (status != SUCCESS)
                return status;

        /// Reset the vector
        if (new_size < dict->vec_size) {
                destructor_function_t destructor = vector_get_destructor(dict->elements);
                status = vector_free(dict->elements);
                dict->elements = vector_init(sizeof(DictionaryNode), compare_equal);
                if (!dict->elements || status != SUCCESS)
                        return ERROR;
                vector_reserve(dict->elements, new_size);
                vector_set_destructor(dict->elements, destructor);
        }
        status = vector_reserve(dict->elements, new_size);
        if (status != SUCCESS)
                return status;

        status = vector_process(dict->elements, init_node, NULL);
        if (status != SUCCESS)
                return status;

        // Change vec_size and n_elements
        size_t n_elements = dict->n_elements;
        dict->n_elements = 0;
        if (dict->vec_size < new_size)
                dict->prev_vec_size = dict->vec_size;
        else
                dict->prev_vec_size = get_prev_prime(new_size);

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
        size_t pos = 0;
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
                printerr_null_param();
                return NULL_PARAMETER_ERROR;
        }
        size_t pos = 0;
        DictionaryNode node = {0};

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

	if (node.state == FULL){
		if (node.value && dict->destructor)
			dict->destructor(node.value);
	}else{
		dict->n_elements++;
	}
	
        // If the node is empty, we need to allocate
        // memory for the key and value.
        if (!node.key)
                node.key = malloc(dict->key_size);	
        if (!node.value)
                node.value = malloc(dict->value_size);
	assert(node.key && node.value);

        memcpy(node.key, key, dict->key_size);
        memcpy(node.value, value, dict->value_size);

        node.state = FULL;

        // Very important. We need to copy back the node to the vector.
        // Remember, the get function returns a COPY of the element.
        vector_set_at(dict->elements, pos, &node);

        // Resize if needed
        if (LF(dict->n_elements, dict->vec_size) >= dict->max_lf){
                size_t new_size = get_next_prime(dict->vec_size * 2);
                int status = dict_redisperse(dict, new_size);
                if (status != SUCCESS){
                        printerr("Could not redisperse", );
                        return status;
                }
        }
        return SUCCESS;
}

//// GET_EXISTS ///////////////////////////////////////////////////////////////

void* dict_get(Dictionary *dict, void *key, void *dest){
        if (!dict || !key){
                printerr_null_param();
                return NULL;
        }
        size_t pos;
        size_t i = 0;
        DictionaryNode node;
        do {
                pos = dict_get_pos(dict, key, i);
                vector_get_at(dict->elements, pos, &node);
                if (node.state == EMPTY)
                        break;
                if (node.state != DELETED){
                        int64_t h1 = dict->hash(key);
                        int64_t h2 = dict->hash(node.key);
                        if (h1 == h2)
                                return memcpy(dest, node.value, dict->value_size);
                }

                i++;
        }while (i < dict->vec_size);
        return NULL;
}

bool dict_exists(Dictionary *dict, void *key){
        if (!dict || !key){
                printerr_null_param();
                return NULL_PARAMETER_ERROR;
        }
        size_t pos;
        size_t i = 0;
        DictionaryNode node;
        do {
                pos = dict_get_pos(dict, key, i);

                vector_get_at(dict->elements, pos, &node);
                if (node.state == EMPTY)
                        break;
                if (node.state != DELETED){
                        int64_t h1 = dict->hash(key);
                        int64_t h2 = dict->hash(node.key);
                        if (h1 == h2)
                                return true;
                }
                i++;
        }while (i < dict->vec_size);
        return false;
}

/// REMOVE ////////////////////////////////////////////////////////////////////

static int dict_delete_node(Dictionary *dict, size_t pos, DictionaryNode node){
        node.state = DELETED;
	if (dict->destructor)
		dict->destructor(node.value);
	vector_set_at(dict->elements, pos, &node);
        dict->n_elements--;
        if (dict->min_lf > 0 && LF(dict->n_elements, dict->vec_size) <= dict->min_lf){
                size_t new_size = get_prev_prime(dict->vec_size / 2);
                int status = dict_redisperse(dict, new_size);
                if (status != SUCCESS){
                        printerr("Could not redisperse",);
                        return status;
                }
        }
        return SUCCESS;
}

int dict_remove(Dictionary *dict, void *key){
        if (!dict || !key){
                printerr_null_param();
                return NULL_PARAMETER_ERROR;
        }
	size_t n_try = 0;
        size_t pos = dict_get_pos(dict, key, 0);
	size_t start_pos = pos;
        DictionaryNode node;
	do{
		vector_get_at(dict->elements, pos, &node);
                if (node.state == FULL){
			int64_t h1 = dict->hash(key);
			int64_t h2 = dict->hash(node.key);
			if (h1 == h2)
				return dict_delete_node(dict, pos, node);
		}
		pos = dict_get_pos(dict, key, ++n_try);
	} while (pos != start_pos);
        return ELEMENT_NOT_FOUND_ERROR;
}

/// FREE //////////////////////////////////////////////////////////////////////

int dict_free(Dictionary *dict){
        if (!dict)
                return NULL_PARAMETER_ERROR;
        vector_process(dict->elements, free_node, &dict->destructor);
        vector_free(dict->elements);
        free(dict);
        return SUCCESS;
}

void dict_free_all(unsigned int n, ...){
	va_list arg;
	va_start(arg, n);
	for (unsigned int i = 0; i < n; i++){
		Dictionary *ptr = va_arg(arg, Dictionary*);
		dict_free(ptr);
	}
	va_end(arg);
}

Dictionary* dict_reset(Dictionary *dict){
        if (!dict)
                return NULL;
        vector_process(dict->elements, free_node, &dict->destructor);
        vector_reserve(dict->elements, DICT_INITIAL_SIZE);
        dict->n_elements = 0;
        dict->vec_size = DICT_INITIAL_SIZE;
        dict->prev_vec_size = get_prev_prime(DICT_INITIAL_SIZE);
        return dict;
}
