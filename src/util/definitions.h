/**
 * @file definitions.h
 * Common macros and typedefs
 *
 *  Copyright (C) 2023 - Saúl Valdelvira \n
 *  License: BSD 3-Clause \n
 *  Email: saul@saulv.es
 */
/// @cond
#pragma once
#ifndef DEFINITIONS_H
#define DEFINITIONS_H
/// @endcond

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef int8_t byte;
#define void_offset(ptr, bytes) ((void*)((byte*)ptr + (bytes)))

#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define ABS(x) ((x) < 0 ? (-1 * (x)) : (x))

#ifdef __cplusplus
}
#endif

#endif
