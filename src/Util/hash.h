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
