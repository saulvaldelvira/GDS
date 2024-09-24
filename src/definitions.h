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
typedef uint8_t u8;
typedef int16_t i16;

#define void_offset(ptr, bytes) ((void*)((byte*)ptr + (bytes)))

#include <string.h>
#define copy_into(buf, i, e, size) { \
        void *ptr = void_offset(buf, i * size); \
        memcpy(ptr, e, size); \
}

#define copy_from(buf, i, e, size) { \
        void *ptr = void_offset(buf, i * size); \
        memcpy(e, ptr, size); \
}

#ifdef __GNUC__
#       define __inline inline __attribute__((always_inline))
#else
#       define __inline inline
#endif

#ifdef __cplusplus
}
#endif

#endif
