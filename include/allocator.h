#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include <stddef.h>

typedef void* (*malloc_t)(size_t);
typedef void* (*calloc_t)(size_t,size_t);
typedef void* (*realloc_t)(void*,size_t);
typedef void (*free_t)(void*);

void gds_set_allocator(malloc_t alloc_func, calloc_t calloc_func, realloc_t realloc_func, free_t free_func);

#endif /* __ALLOCATOR_H__ */
