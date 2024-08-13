#include <stdlib.h>
#include "gdsmalloc.h"
#include "error_priv.h"

__inline
void* gdsmalloc(size_t n) {
        void *p = malloc(n);
        if (!p)
                register_error(GDS_NOMEM_ERROR);
        return p;
}

__inline
void* gdscalloc(size_t nelem, size_t elemsize) {
        void *p = calloc(nelem, elemsize);
        if (!p)
                register_error(GDS_NOMEM_ERROR);
        return p;
}

__inline
void* gdsrealloc(void *ptr, size_t size) {
        void *p = realloc(ptr, size);
        if (!p)
                register_error(GDS_NOMEM_ERROR);
        return p;
}
