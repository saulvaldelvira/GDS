/*
 * definitions.h - Util macros and typedefs
 * Author: Saúl Valdelvira (2023)
 */
#pragma once
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

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
