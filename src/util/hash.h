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

typedef int64_t (*hash_function_t) (const void*);

int64_t hash_int(const void *arg);

int64_t hash_short(const void *arg);

int64_t hash_char(const void *arg);

int64_t hash_long(const void *arg);

int64_t hash_string(const void *arg);

#ifdef __cplusplus
}
#endif

#endif
