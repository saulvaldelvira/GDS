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
typedef uint32_t u32;
typedef uint16_t u16;
typedef int16_t i16;

#define void_offset(ptr, bytes) ((void*)((byte*)ptr + (bytes)))

#ifdef __cplusplus
}
#endif

#endif
