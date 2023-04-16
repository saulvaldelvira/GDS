/**
 *  Defines macros to be use by the modules.
 *
 *  Copyright (C) 2023 - Saúl Valdelvira
 *
 *  This program is free software. You can modify
 *  and/or redistribute it under the terms of the
 *  GNU General Public License version 2, or newer.
 *  See <https://www.gnu.org/licenses/>.
 *
 *  Email: saulvaldelvira@gmail.com
 */
#pragma once
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#define GET_ALL_ELEMENTS 0

typedef unsigned char byte_t;
#define void_offset(ptr, bytes) (void*)(((byte_t*)ptr) + (bytes))
#define matrix_offset(arr, dim, rows, cols) (arr+((dim)*(rows) + (cols)))

#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define MIN(x,y) ((x) < (y) ? (x) : (y))

#define ABS(x) ((x) < 0 ? (-1 * (x)) : (x))

#ifdef __cplusplus
}
#endif

#endif
