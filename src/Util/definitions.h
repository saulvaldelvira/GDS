/**
 *  Defines macros to be use by the modules.
 *
 *  Copyright (C) 2023 - Saúl Valdelvira
 *  License: BSD 3-Clause
 *  Email: saulvaldelvira@gmail.com
 */
#pragma once
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define GET_ALL_ELEMENTS 0

typedef int8_t byte_t;
#define void_offset(ptr, bytes) (void*)(((byte_t*)ptr) + (bytes))
#define matrix_offset(arr, dim, rows, cols) (arr+((dim)*(rows) + (cols)))

#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define MIN(x,y) ((x) < (y) ? (x) : (y))

#define ABS(x) ((x) < 0 ? (-1 * (x)) : (x))

#ifdef __cplusplus
}
#endif

#endif
