#include <stdlib.h>
#include <stdint.h>
#include "gdsmalloc.h"
#include "error_priv.h"

#include "allocator.h"

malloc_t __gds_malloc = malloc;
calloc_t __gds_calloc = calloc;
realloc_t __gds_realloc = realloc;
free_t __gds_free = free;

void gds_set_allocator(malloc_t alloc_func, calloc_t calloc_func, realloc_t realloc_func, free_t free_func) {
        __gds_malloc = alloc_func;
        __gds_calloc = calloc_func;
        __gds_realloc = realloc_func;
        __gds_free = free_func;
}

__inline
void* gdsmalloc(size_t n) {
        void *p = __gds_malloc(n);
        if (!p)
                register_error(GDS_NOMEM_ERROR);
        return p;
}

__inline
void* gdscalloc(size_t nelem, size_t elemsize) {
        void *p = __gds_calloc(nelem, elemsize);
        if (!p)
                register_error(GDS_NOMEM_ERROR);
        return p;
}

__inline
void* gdsrealloc(void *ptr, size_t size) {
        void *p = __gds_realloc(ptr, size);
        if (!p)
                register_error(GDS_NOMEM_ERROR);
        return p;
}

__inline void gdsfree(void *ptr) {
        __gds_free(ptr);
}
