/**
 *  Copyright (C) 2023 - Saúl Valdelvira
 *  License: BSD 3-Clause
 *  Email: saulvaldelvira@gmail.com
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

int64_t hash_char(const void *arg);

int64_t hash_long(const void *arg);

int64_t hash_string(const void *arg);

#ifdef __cplusplus
}
#endif

#endif
