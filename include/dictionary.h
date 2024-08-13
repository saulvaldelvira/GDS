/*
 * dictionary.h - dictionary_t definition.
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

typedef struct dictionary dictionary_t;

enum Redispersion{
        LINEAR, QUADRATIC, DOUBLE_HASHING
};

/**
 * Initializes a dictionary
 * @param key_size size in bytes of the keys
 * @param value_size size in bytes of the values
 * @param hash_func hash fucntion for the keys. It takes a const void pointer
 *                     and returns a 64 bit signed integer (int64_t).
*/
dictionary_t* dict_init(size_t key_size, size_t value_size, hash_function_t hash_func);

/**
 * Initializes a dictionary with an initial capacity
 * @param key_size size in bytes of the keys
 * @param value_size size in bytes of the values
 * @param hash_func hash fucntion for the keys. It takes a const void pointer
 *                     and returns a 64 bit signed integer (int64_t).
 * @param capacity initial capacity of the vector
*/
dictionary_t* dict_with_capacity(size_t key_size, size_t value_size, hash_function_t hash_func, size_t capacity);

#define DICT_NO_SHRINKING        -1.0f
#define DICT_DEF_REDISPERSION        DOUBLE_HASHING
#define DICT_DEF_MAX_LF                0.2f
#define DICT_DEF_MIN_LF                0.1f

/**
 * Configures the dictionary_t's behaviour.
 * @param redisperison the kind of redispersion to apply. Can be LINEAR, QUADRATIC or DOUBLE_HASHING (default value).
 * @param min_lf minimun value for the load factor. This means that, when deleting,
 *               if (number of elements / total size) <= min_lf, the dictionary's size is cut in half.
 * @param max_lf maximun value for the load factor. This means that, when adding,
 *               if (number of elements / total size) >= min_lf, the dictionary's size is doubled.
 * @note 1) You can use the macros defined in dictionary.h to pass the default values (for example, DICT_DEF_MAX_LF)
 * @note 2) You can pass 0.0f to the min_lf and max_lf to not change them, and NULL to hash_func to also use the current one.
 * @note 3) You can use DICT_NO_SHRINKING in the min_lf parameter to configure the dictionary to NOT shrink when deleting. This makes it
 *       faster, since we don't need to resize that often, but wastes more memory in some situations.
*/
int dict_configure(dictionary_t *dict, enum Redispersion redispersion, double min_lf, double max_lf, hash_function_t hash_func);

/**
 * Sets the destructor for value type.
 * A NULL parameter means there's no destructor.
*/
void dict_set_destructor(dictionary_t *dict, destructor_function_t value_destructor);

/**
 * Puts the a key-value pair in the dictionary
*/
int dict_put(dictionary_t *dict, void *key, void *value);

/**
 * Returns the value for the given key, or NULL if it
 * doesn't exist in the dictionary
*/
void* dict_get(const dictionary_t *dict, void *key, void *dest);

/**
 * Returns a vector with all the keys to the dictionary_t
 * The vector is of the same type as the keys in the table.
 * This means, if we call this method on a dictionary_t of int to char,
 * the vector will be a vector of ints
 */
vector_t* dict_keys(const dictionary_t *dict);

/**
 * Returns true if the key exists in the dictionary
*/
bool dict_exists(const dictionary_t *dict, void *key);

/**
 * Removes a key from the dictionary
*/
int dict_remove(dictionary_t *dict, void *key);

void dict_free(dictionary_t *d, ...);

/**
 * Frees all the given dicts.
 */
#define dict_free(...) dict_free(__VA_ARGS__, 0L)

/**
 * Removes all elements from the dictionary
*/
void dict_clear(dictionary_t *dict);

#ifdef __cplusplus
}
#endif

#endif
