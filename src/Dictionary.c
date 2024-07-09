/*
 * Dictionary.c - Dictionary implementation.
 * Author: Saúl Valdelvira (2023)
 */
#include "Dictionary.h"
#include "./util/error.h"
#include "./util/definitions.h"
#include "Vector.h"
#include "util/compare.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <time.h>


typedef struct DictionaryNode {
        void *key;
        void *value;
        enum NodeState {
                EMPTY, FULL, DELETED
        } state;
} DictionaryNode;

struct Dictionary {
        Vector *vec_elements;   ///< Vector to store the elements
        hash_function_t hash;                   ///< Hashing function pointer
        destructor_function_t destructor;       ///< Destructor function pointer
        u32 n_elements;      ///< Number of elements in the Dictionary
        u32 prev_vec_size;   ///< Previous capacity of the vector
        u16 value_size;      ///< Size (in bytes) of the value data type
        u16 key_size;        ///< Size (in bytes) of the key data type
        float max_lf;   ///< Maximun load factor before redispersing
        float min_lf;    ///< Minimun load factor before shrinking
        enum Redispersion redispersion;   ///< Type of redispersion to apply
};

/**
 * Calculates the load factor.
 * @param x number of elements
 * @param B capacity of the table
*/
#define LF(x,B) ((x) * 1.0 / (B))
#define DICT_INITIAL_SIZE 11

#define VEC_SIZE(dict) vector_size(dict->vec_elements)

static inline int64_t abs_i64(int64_t n) { return n < 0 ? -n : n; }

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
static void init_node(void *node){
        DictionaryNode *n = (DictionaryNode*) node;
        n->key = NULL;
        n->value = NULL;
        n->state = EMPTY;
}

/**
 * Frees the memory allocated for a node.
 */
static void free_node(void *node, void *args){
        destructor_function_t destructor = * (destructor_function_t*) args;
        DictionaryNode *n = (DictionaryNode*) node;
        if (destructor && n->state == FULL)
                destructor(n->value);
        free(n->key);
        free(n->value);
        n->key = NULL;
        n->value = NULL;
}

static int __init_dict(Dictionary *dict, size_t key_size, size_t value_size, hash_function_t hash_func, size_t capacity) {
        dict->value_size = value_size;
        dict->key_size = key_size;
        dict->min_lf = DICT_DEF_MIN_LF;
        dict->max_lf = DICT_DEF_MAX_LF;
        dict->destructor = NULL;
        dict->vec_elements = vector_init(sizeof(DictionaryNode), compare_equal);
        if (!dict->vec_elements){
                return ERROR;
        }
        vector_resize(dict->vec_elements, capacity, init_node);
        dict->n_elements = 0;
        dict->hash = hash_func;
        dict->redispersion = DICT_DEF_REDISPERSION;
        dict->prev_vec_size = get_prev_prime(capacity);
        return SUCCESS;
}

Dictionary* dict_with_capacity(size_t key_size, size_t value_size, hash_function_t hash_func, size_t capacity) {
        assert(hash_func && key_size > 0 && value_size > 0);
        Dictionary *dict = malloc(sizeof(*dict));
        if (!dict) return NULL;
        if ( __init_dict(dict, key_size, value_size, hash_func, capacity) != SUCCESS) {
                free(dict);
                return NULL;
        }
        return dict;
}

inline Dictionary* dict_init(size_t key_size, size_t value_size, hash_function_t hash_func){
        return dict_with_capacity(key_size, value_size, hash_func, DICT_INITIAL_SIZE);
}

int dict_configure(Dictionary *dict, enum Redispersion redispersion, double min_lf, double max_lf, hash_function_t hash_func){
        assert(dict);
        float min, max;
        if (min_lf > 0.0f || min_lf == DICT_NO_SHRINKING)
                min = min_lf;
        else
                min = dict->min_lf;

        if (max_lf > 0.0f)
                max = max_lf;
        else
                max = dict->max_lf;

        if (min >= max)
                return INVALID_PARAMETER_ERROR;

        dict->min_lf = min;
        dict->max_lf = max;

        if (redispersion >= 0)
                dict->redispersion = redispersion;
        if (hash_func)
                dict->hash = hash_func;

        return SUCCESS;
}

void dict_set_destructor(Dictionary *dict, destructor_function_t value_destructor){
        if (dict)
                dict->destructor = value_destructor;
}

static inline void dict_free_contents(Dictionary *dict) {
        assert(dict);
        vector_map(dict->vec_elements, free_node, &dict->destructor);
        vector_free(dict->vec_elements);
}

//// REDISPERSE ///////////////////////////////////////////////////////////////

static int __delete_node(Dictionary *dict, size_t pos, bool redispersion, bool destroy);

/**
 * Redisperses the dictionary.
 * Can be used to expand or shrink the table.
 */
static int dict_redisperse(Dictionary *dict, size_t new_size){
        assert(dict->n_elements < new_size);

        Dictionary d;
        __init_dict(&d, dict->key_size, dict->value_size, dict->hash, new_size);
        d.destructor = dict->destructor;

        for (size_t i = 0; i < VEC_SIZE(dict); i++) {
                DictionaryNode node;
                vector_at(dict->vec_elements, i, &node);
                if (node.state == FULL) {
                        int status = dict_put(&d, node.key, node.value);
                        if (status != SUCCESS) {
                                dict_free(&d);
                                return status;
                        }
                        __delete_node(dict, i, false, false);
                }
        }

        dict_free_contents(dict);
        memcpy(dict, &d, sizeof(Dictionary));
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
                pos = abs_i64(dict->hash(key)) + n_it;
                break;
        case QUADRATIC:
                pos =  abs_i64(dict->hash(key)) + n_it * n_it;
                break;
        case DOUBLE_HASHING: ;
                int h1 = abs_i64(dict->hash(key));
                int h2 = dict->prev_vec_size - h1 % dict->prev_vec_size;
                pos = h1 + n_it * h2;
                break;
        }
        return pos % VEC_SIZE(dict);
}

int dict_put(Dictionary *dict, void *key, void *value){
        assert(dict && key && value);
        size_t pos = 0;
        DictionaryNode node = {0};

        for (size_t i = 0; i < VEC_SIZE(dict); ++i){
                pos = dict_get_pos(dict, key, i);
                vector_at(dict->vec_elements, pos, &node);

                if (node.state == FULL){
                        int64_t h1 = dict->hash(key);
                        int64_t h2 = dict->hash(node.key);
                        if (h1 == h2)
                                break;
                }else {
                        break;
                }
        }

        /* if the node is empty, we need to allocate
           memory for the key and value. */
        if (!node.key){
                node.key = malloc(dict->key_size);
                if (!node.key)
                        return ERROR;
        }
        if (!node.value){
                node.value = malloc(dict->value_size);
                if (!node.value){
                        free(node.key);
                        return ERROR;
                }
        }

        if (node.state == FULL && dict->destructor)
                        dict->destructor(node.value);

        if (node.state != FULL)
                dict->n_elements++;

        memcpy(node.key, key, dict->key_size);
        memcpy(node.value, value, dict->value_size);

        node.state = FULL;

        /* Very important!. We need to copy back the node into the vector.
           Remember that the get function returns a COPY of the element. */
        vector_set_at(dict->vec_elements, pos, &node);

        // Resize if needed
        if (LF(dict->n_elements, VEC_SIZE(dict)) >= dict->max_lf){
                size_t new_size = get_next_prime(VEC_SIZE(dict) * 2);
                int status = dict_redisperse(dict, new_size);
                if (status != SUCCESS)
                        return status;
        }
        return SUCCESS;
}

//// GET_EXISTS ///////////////////////////////////////////////////////////////

void* dict_get(Dictionary *dict, void *key, void *dest){
        assert(dict && key);
        for (size_t i = 0; i < VEC_SIZE(dict); i++) {
                size_t pos = dict_get_pos(dict, key, i);
                DictionaryNode node;
                vector_at(dict->vec_elements, pos, &node);
                if (node.state == FULL){
                        int64_t h1 = dict->hash(key);
                        int64_t h2 = dict->hash(node.key);
                        if (h1 == h2)
                                return memcpy(dest, node.value, dict->value_size);
                }
                if (node.state == EMPTY) break;
        }
        return NULL;
}

bool dict_exists(Dictionary *dict, void *key){
        assert(dict && key);
        for (size_t i = 0; i < VEC_SIZE(dict); i++) {
                size_t pos = dict_get_pos(dict, key, i);
                DictionaryNode node;
                vector_at(dict->vec_elements, pos, &node);
                if (node.state == FULL){
                        int64_t h1 = dict->hash(key);
                        int64_t h2 = dict->hash(node.key);
                        if (h1 == h2)
                                return true;
                }
                if (node.state == EMPTY) break;
        }
        return false;
}

Vector* dict_keys(Dictionary *dict) {
        assert(dict);
        Vector *v = vector_with_capacity(dict->key_size, compare_equal, dict->n_elements);
        if (!v) return NULL;

        for (size_t i = 0; i < VEC_SIZE(dict); i++) {
                DictionaryNode node;
                vector_at(dict->vec_elements, i, &node);
                if (node.state == FULL) {
                        if (vector_append(v, node.key) != SUCCESS) {
                                vector_free(v);
                                return NULL;
                        }
                }
        }

        return v;
}

/// REMOVE ////////////////////////////////////////////////////////////////////

static int __delete_node(Dictionary *dict, size_t pos, bool redispersion, bool destroy){
        DictionaryNode node;
        vector_at(dict->vec_elements, pos, &node);
        if (destroy && dict->destructor)
                dict->destructor(node.value);
        free(node.value);
        free(node.key);
        node.key = node.value = NULL;
        node.state = DELETED;
        vector_set_at(dict->vec_elements, pos, &node);
        dict->n_elements--;
        if (!redispersion)
                return SUCCESS;
        if (dict->min_lf > 0 && LF(dict->n_elements, VEC_SIZE(dict)) <= dict->min_lf){
                size_t new_size = get_prev_prime(VEC_SIZE(dict) / 2);
                int status = dict_redisperse(dict, new_size);
                if (status != SUCCESS)
                        return status;
        }
        return SUCCESS;
}

int dict_remove(Dictionary *dict, void *key){
        assert(dict && key);
        for (size_t i = 0; i < VEC_SIZE(dict); i++){
                size_t pos = dict_get_pos(dict, key, i);
                DictionaryNode node;
                vector_at(dict->vec_elements, pos, &node);
                if (node.state == FULL){
                        int64_t h1 = dict->hash(key);
                        int64_t h2 = dict->hash(node.key);
                        if (h1 == h2)
                                return __delete_node(dict, pos, true, true);
                }
                else if (node.state == EMPTY) break;
        }
        return ELEMENT_NOT_FOUND_ERROR;
}

//// FREE //////////////////////////////////////////////////////////////////////

static void __dict_free(Dictionary *dict){
        assert(dict);
        dict_free_contents(dict);
        free(dict);
}

void (dict_free)(Dictionary *d, ...){
        if (!d)
                return;
        va_list arg;
        va_start(arg, d);
        do {
                if (d)
                     __dict_free(d);
                d = va_arg(arg, Dictionary*);
        } while (d);
        va_end(arg);
}

void dict_clear(Dictionary *dict){
        if (!dict)
                return;
        vector_map(dict->vec_elements, free_node, &dict->destructor);
        vector_reset(dict->vec_elements);
        vector_resize(dict->vec_elements, DICT_INITIAL_SIZE, init_node);
        dict->n_elements = 0;
        dict->prev_vec_size = get_prev_prime(DICT_INITIAL_SIZE);
}
