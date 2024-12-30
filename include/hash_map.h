/*
 * hash_map.h - hash_map_t definition.
 * Author: Saúl Valdelvira (2023)
 */
#pragma once
#ifndef DICTIONARY_H
#define DICTIONARY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include "hash.h"
#include "compare.h"
#include "vector.h"

typedef struct hash_map hash_map_t;

enum Redispersion{
        LINEAR, QUADRATIC, DOUBLE_HASHING
};

/**
 * Initializes a hash_map
 * @param key_size size in bytes of the keys
 * @param value_size size in bytes of the values
 * @param hash_func hash fucntion for the keys. it takes a const void pointer
 *                     and returns a 64 bit signed integer (int64_t).
 * @param cmp Comparator function
*/
NONNULL()
hash_map_t* hashmap_init(size_t key_size, size_t value_size, hash_function_t hash_func, comparator_function_t cmp);

/**
 * Initializes a hash_map with an initial capacity
 * @param key_size size in bytes of the keys
 * @param value_size size in bytes of the values
 * @param hash_func hash fucntion for the keys. It takes a const void pointer
 *                     and returns a 64 bit signed integer (int64_t).
 * @param cmp Comparator function
 * @param capacity initial capacity of the vector
*/
NONNULL()
hash_map_t* hashmap_with_capacity(size_t key_size, size_t value_size, hash_function_t hash_func, comparator_function_t cmp, size_t capacity);

#define DICT_NO_SHRINKING        -1.0f
#define DICT_DEF_REDISPERSION        DOUBLE_HASHING
#define DICT_DEF_MAX_LF                0.2f
#define DICT_DEF_MIN_LF                0.1f

/**
 * Configures the hash_map_t's behaviour.
 * @param redisperison the kind of redispersion to apply. Can be LINEAR, QUADRATIC or DOUBLE_HASHING (default value).
 * @param min_lf minimun value for the load factor. This means that, when deleting,
 *               if (number of elements / total size) <= min_lf, the hash_map's size is cut in half.
 * @param max_lf maximun value for the load factor. This means that, when adding,
 *               if (number of elements / total size) >= min_lf, the hash_map's size is doubled.
 * @note 1) You can use the macros defined in hash_map.h to pass the default values (for example, DICT_DEF_MAX_LF)
 * @note 2) You can pass 0.0f to the min_lf and max_lf to not change them, and NULL to hash_func to also use the current one.
 * @note 3) You can use DICT_NO_SHRINKING in the min_lf parameter to configure the hash_map to NOT shrink when deleting. This makes it
 *       faster, since we don't need to resize that often, but wastes more memory in some situations.
*/
NONNULL()
int hashmap_configure(hash_map_t *map, enum Redispersion redispersion, double min_lf, double max_lf);

/**
 * Sets the destructor for value type.
 * A NULL parameter means there's no destructor.
*/
NONNULL(1)
void hashmap_set_destructor(hash_map_t *map, destructor_function_t value_destructor);

/**
 * Puts the a key-value pair in the hash_map
*/
NONNULL(1,2)
int hashmap_put(hash_map_t *map, void *key, void *value);

/**
 * Returns the value for the given key, or NULL if it
 * doesn't exist in the hash_map
*/
NONNULL()
void* hashmap_get(const hash_map_t *map, void *key, void *dest);

/**
 * Returns a vector with all the keys to the hash_map_t
 * The vector is of the same type as the keys in the table.
 * This means, if we call this method on a hash_map_t of int to char,
 * the vector will be a vector of ints
 */
NONNULL()
vector_t* hashmap_keys(const hash_map_t *map);

/**
 * Returns true if the key exists in the hash_map
*/
NONNULL()
bool hashmap_exists(const hash_map_t *map, void *key);

/**
 * Removes a key from the hash_map
*/
NONNULL()
int hashmap_remove(hash_map_t *map, void *key);

/**
 * Removes all elements from the hash_map
 */
NONNULL()
void hashmap_clear(hash_map_t *map);

NONNULL()
void hashmap_free(hash_map_t *d, ...);

/**
 * Frees all the given maps.
 */
#define hashmap_free(...) hashmap_free(__VA_ARGS__, 0L)

#ifdef __cplusplus
}
#endif

#endif
