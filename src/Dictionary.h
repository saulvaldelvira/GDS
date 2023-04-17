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
#pragma once
#ifndef DICTIONARY_H
#define DICTIONARY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include "./Util/hash.h"

typedef struct _Dictionary Dictionary;

enum Redispersion{
	LINEAR, QUADRATIC, DOUBLE_HASHING
};

/**
 * Initializes a dictionary
 * @param key_size size in bytes of the keys
 * @param value_size size in bytes of the values
 * @param hash_func hash fucntion for the keys. It takes a const void pointer
 * 		    and returns a 64 bit signed integer (int64_t).
*/
Dictionary* dict_init(size_t key_size, size_t value_size, hash_function_t hash_func);

#define DICT_NO_SHRINKING	-1.0f
#define DICT_DEF_REDISPERSION	DOUBLE_HASHING
#define DICT_DEF_MAX_LF		0.2f
#define DICT_DEF_MIN_LF		0.1f

/**
 * Configures the Dictionary's behaviour.
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
int dict_configure(Dictionary *dict, enum Redispersion redispersion, double min_lf, double max_lf, hash_function_t hash_func);

/**
 * Puts the a key-value pair in the dictionary
*/
int dict_put(Dictionary *dict, void *key, void *value);

/**
 * Returns the value for the given key, or NULL if it
 * doesn't exist in the dictionary
*/
void* dict_get(Dictionary *dict, void *key, void *dest);

/**
 * Returns true if the key exists in the dictionary
*/
bool dict_exists(Dictionary *dict, void *key);

/**
 * Removes a key from the dictionary
*/
int dict_remove(Dictionary *dict, void *key);

/**
 * Frees the dictionary
*/
int dict_free(Dictionary *dict);

Dictionary* dict_reset(Dictionary *dict);

#ifdef __cplusplus
}
#endif

#endif
