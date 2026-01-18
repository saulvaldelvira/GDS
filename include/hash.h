/*
 * hash.h - Defines functions to hash common data types.
 * Author: Saúl Valdelvira (2023)
 */
#pragma once
#ifndef HASH_H
#define HASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define HASH_COMBINE(h1, h2)

typedef uint64_t hashcode_t;

typedef hashcode_t (*hash_function_t) (const void*);

/**
 * Merges the h1 and h2 into a single hash code
 */
hashcode_t hash_combine(hashcode_t h1, hashcode_t h2);

hashcode_t hash_int(const void *arg);

hashcode_t hash_short(const void *arg);

hashcode_t hash_char(const void *arg);

hashcode_t hash_long(const void *arg);

hashcode_t hash_string(const void *arg);

hashcode_t hash_ptr(const void *arg);

#ifdef __cplusplus
}
#endif

#endif
