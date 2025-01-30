/*
 * hash_map.c - Hash Map implementation.
 * Author: Saúl Valdelvira (2023)
 */
#include "hash_map.h"
#include "attrs.h"
#include "error.h"
#include "definitions.h"
#include "vector.h"
#include "compare.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <time.h>
#include "gdsmalloc.h"

typedef struct hash_node_t {
        void *key;
        void *value;
        enum NodeState {
                EMPTY, FULL, DELETED
        } state;
} hash_node_t;

struct hash_map {
        vector_t *vec_elements;   ///< vector_t to store the elements
        hash_function_t hash;                   ///< Hashing function pointer
        destructor_function_t destructor;       ///< Destructor function pointer
        comparator_function_t cmp;       ///< Comparator function pointer
        u32 n_elements;      ///< Number of elements in the hash_map_t
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

#define VEC_SIZE(map) vector_size(map->vec_elements)

_const_fn
static inline int64_t abs_i64(int64_t n) { return n < 0 ? -n : n; }

/// PRIME //////////////////////////////////////////////////////////////////////

_const_fn
static bool is_prime(int n){
        for (int i = n-1; i > 1; --i){
                if (n % i == 0)
                        return false;
        }
        return true;
}

_const_fn
static int get_prev_prime(int n){
        do{
                --n;
        }while(!is_prime(n));
        return n;
}

_const_fn
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
        hash_node_t *n = (hash_node_t*) node;
        n->key = NULL;
        n->value = NULL;
        n->state = EMPTY;
}

/**
 * Frees the memory allocated for a node.
 */
static void free_node(void *node, void *args){
        destructor_function_t destructor = * (destructor_function_t*) args;
        hash_node_t *n = (hash_node_t*) node;
        if (destructor && n->state == FULL)
                destructor(n->value);
        free(n->key);
        free(n->value);
        n->key = NULL;
        n->value = NULL;
}

__inline
static int __init_map(hash_map_t *map, size_t key_size, size_t value_size, hash_function_t hash_func, comparator_function_t cmp, size_t capacity) {
        map->value_size = value_size;
        map->key_size = key_size;
        map->min_lf = DICT_DEF_MIN_LF;
        map->max_lf = DICT_DEF_MAX_LF;
        map->cmp = cmp;
        map->destructor = NULL;
        map->vec_elements = vector_init(sizeof(hash_node_t), compare_equal);
        if (!map->vec_elements){
                return GDS_ERROR;
        }
        vector_resize(map->vec_elements, capacity, init_node);
        map->n_elements = 0;
        map->hash = hash_func;
        map->redispersion = DICT_DEF_REDISPERSION;
        map->prev_vec_size = get_prev_prime(capacity);
        return GDS_SUCCESS;
}

hash_map_t* hashmap_with_capacity(size_t key_size, size_t value_size, hash_function_t hash_func, comparator_function_t cmp, size_t capacity) {
        assert(hash_func && key_size > 0);
        hash_map_t *map = gdsmalloc(sizeof(*map));
        if (!map) return NULL;
        if ( __init_map(map, key_size, value_size, hash_func, cmp, capacity) != GDS_SUCCESS) {
                free(map);
                return NULL;
        }
        return map;
}

inline hash_map_t* hashmap_init(size_t key_size, size_t value_size, hash_function_t hash_func, comparator_function_t cmp){
        return hashmap_with_capacity(key_size, value_size, hash_func, cmp, DICT_INITIAL_SIZE);
}

int hashmap_configure(hash_map_t *map, enum Redispersion redispersion, double min_lf, double max_lf){
        assert(map);
        float min, max;
        if (min_lf > 0.0f || min_lf == DICT_NO_SHRINKING)
                min = min_lf;
        else
                min = map->min_lf;

        if (max_lf > 0.0f)
                max = max_lf;
        else
                max = map->max_lf;

        if (min >= max)
                return GDS_INVALID_PARAMETER_ERROR;

        map->min_lf = min;
        map->max_lf = max;

        if (redispersion >= 0)
                map->redispersion = redispersion;

        return GDS_SUCCESS;
}

__inline
void hashmap_set_destructor(hash_map_t *map, destructor_function_t value_destructor){
        if (map)
                map->destructor = value_destructor;
}

static inline void hashmap_free_contents(hash_map_t *map) {
        assert(map);
        vector_map(map->vec_elements, free_node, &map->destructor);
        vector_free(map->vec_elements);
}


static bool __are_equal(const hash_map_t *map, const void *e1, const void *e2) {
        int64_t h1 = map->hash(e1);
        int64_t h2 = map->hash(e2);
        if (h1 == h2) {
                int c = map->cmp(e1, e2);
                if (c == 0)
                        return true;

        }
        return false;
}

//// REDISPERSE ///////////////////////////////////////////////////////////////

static int __delete_node(hash_map_t *map, size_t pos, bool redispersion, bool destroy);

/**
 * Redisperses the hash_map.
 * Can be used to expand or shrink the table.
 */
static int hashmap_redisperse(hash_map_t *map, size_t new_size){
        assert(map->n_elements < new_size);

        hash_map_t d;
        __init_map(&d, map->key_size, map->value_size, map->hash, map->cmp, new_size);
        d.destructor = map->destructor;

        for (size_t i = 0; i < VEC_SIZE(map); i++) {
                hash_node_t node;
                vector_at(map->vec_elements, i, &node);
                if (node.state == FULL) {
                        int status = hashmap_put(&d, node.key, node.value);
                        if (status != GDS_SUCCESS) {
                                hashmap_free(&d);
                                return status;
                        }
                        __delete_node(map, i, false, false);
                }
        }

        hashmap_free_contents(map);
        memcpy(map, &d, sizeof(hash_map_t));
        return GDS_SUCCESS;
}

//// PUT //////////////////////////////////////////////////////////////////////

/**
 * Retuns an index to store the key into
 * @param n_it number of tries, to handle collisions.
 */
static size_t hashmap_get_pos(const hash_map_t *map, void *key, size_t n_it){
        size_t pos = 0;
        switch (map->redispersion){
        case LINEAR:
                pos = abs_i64(map->hash(key)) + n_it;
                break;
        case QUADRATIC:
                pos =  abs_i64(map->hash(key)) + n_it * n_it;
                break;
        case DOUBLE_HASHING: ;
                int h1 = abs_i64(map->hash(key));
                int h2 = map->prev_vec_size - h1 % map->prev_vec_size;
                pos = h1 + n_it * h2;
                break;
        }
        return pos % VEC_SIZE(map);
}

int hashmap_put(hash_map_t *map, void *key, void *value){
        assert(map && key);
        if (map->value_size != 0)
                assert(value);
        size_t pos = 0;
        hash_node_t node = {0};

        for (size_t i = 0; i < VEC_SIZE(map); ++i){
                pos = hashmap_get_pos(map, key, i);
                vector_at(map->vec_elements, pos, &node);

                if (node.state == FULL){
                        if (__are_equal(map, key, node.key))
                                break;
                }else {
                        break;
                }
        }

        /* if the node is empty, we need to allocate
           memory for the key and value. */
        if (!node.key){
                node.key = gdsmalloc(map->key_size);
                if (!node.key)
                        return GDS_ERROR;
        }
        if (!node.value){
                node.value = gdsmalloc(map->value_size);
                if (!node.value){
                        free(node.key);
                        return GDS_ERROR;
                }
        }

        if (node.state == FULL && map->destructor)
                        map->destructor(node.value);

        if (node.state != FULL)
                map->n_elements++;

        memcpy(node.key, key, map->key_size);
        memcpy(node.value, value, map->value_size);

        node.state = FULL;

        /* Very important!. We need to copy back the node into the vector.
           Remember that the get function returns a COPY of the element. */
        vector_set_at(map->vec_elements, pos, &node);

        // Resize if needed
        if (LF(map->n_elements, VEC_SIZE(map)) >= map->max_lf){
                size_t new_size = get_next_prime(VEC_SIZE(map) * 2);
                int status = hashmap_redisperse(map, new_size);
                if (status != GDS_SUCCESS)
                        return status;
        }
        return GDS_SUCCESS;
}

//// GET_EXISTS ///////////////////////////////////////////////////////////////

void* hashmap_get(const hash_map_t *map, void *key, void *dest){
        assert(map && key);
        for (size_t i = 0; i < VEC_SIZE(map); i++) {
                size_t pos = hashmap_get_pos(map, key, i);
                hash_node_t node;
                vector_at(map->vec_elements, pos, &node);
                if (node.state == FULL){
                        if (__are_equal(map, key, node.key))
                                return memcpy(dest, node.value, map->value_size);
                }
                if (node.state == EMPTY) break;
        }
        return NULL;
}

bool hashmap_exists(const hash_map_t *map, void *key){
        assert(map && key);
        for (size_t i = 0; i < VEC_SIZE(map); i++) {
                size_t pos = hashmap_get_pos(map, key, i);
                hash_node_t node;
                vector_at(map->vec_elements, pos, &node);
                if (node.state == FULL){
                        if (__are_equal(map, key, node.key))
                                return true;
                }
                if (node.state == EMPTY) break;
        }
        return false;
}

vector_t* hashmap_keys(const hash_map_t *map) {
        assert(map);
        vector_t *v = vector_with_capacity(map->key_size, compare_equal, map->n_elements);
        if (!v) return NULL;

        for (size_t i = 0; i < VEC_SIZE(map); i++) {
                hash_node_t node;
                vector_at(map->vec_elements, i, &node);
                if (node.state == FULL) {
                        if (vector_append(v, node.key) != GDS_SUCCESS) {
                                vector_free(v);
                                return NULL;
                        }
                }
        }

        return v;
}

/// REMOVE ////////////////////////////////////////////////////////////////////

static int __delete_node(hash_map_t *map, size_t pos, bool redispersion, bool destroy){
        hash_node_t node;
        vector_at(map->vec_elements, pos, &node);
        if (destroy && map->destructor)
                map->destructor(node.value);
        free(node.value);
        free(node.key);
        node.key = node.value = NULL;
        node.state = DELETED;
        vector_set_at(map->vec_elements, pos, &node);
        map->n_elements--;
        if (!redispersion)
                return GDS_SUCCESS;
        if (map->min_lf > 0 && LF(map->n_elements, VEC_SIZE(map)) <= map->min_lf){
                size_t new_size = get_prev_prime(VEC_SIZE(map) / 2);
                int status = hashmap_redisperse(map, new_size);
                if (status != GDS_SUCCESS)
                        return status;
        }
        return GDS_SUCCESS;
}

int hashmap_remove(hash_map_t *map, void *key){
        assert(map && key);
        for (size_t i = 0; i < VEC_SIZE(map); i++){
                size_t pos = hashmap_get_pos(map, key, i);
                hash_node_t node;
                vector_at(map->vec_elements, pos, &node);
                if (node.state == FULL){
                        if (__are_equal(map, key, node.key))
                                return __delete_node(map, pos, true, true);
                }
                else if (node.state == EMPTY) break;
        }
        return GDS_ELEMENT_NOT_FOUND_ERROR;
}

//// FREE //////////////////////////////////////////////////////////////////////

static void __hashmap_free(hash_map_t *map){
        assert(map);
        hashmap_free_contents(map);
        free(map);
}

void (hashmap_free)(hash_map_t *d, ...){
        if (!d)
                return;
        va_list arg;
        va_start(arg, d);
        do {
                if (d)
                     __hashmap_free(d);
                d = va_arg(arg, hash_map_t*);
        } while (d);
        va_end(arg);
}

void hashmap_clear(hash_map_t *map){
        if (!map)
                return;
        vector_map(map->vec_elements, free_node, &map->destructor);
        vector_reset(map->vec_elements);
        vector_resize(map->vec_elements, DICT_INITIAL_SIZE, init_node);
        map->n_elements = 0;
        map->prev_vec_size = get_prev_prime(DICT_INITIAL_SIZE);
}
