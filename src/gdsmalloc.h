#ifndef __XMALLOC_H__
#define __XMALLOC_H__

#include <stddef.h>

void* gdsmalloc(size_t n);
void* gdscalloc(size_t nelem, size_t elemsize);
void* gdsrealloc(void *ptr, size_t size);
void gdsfree(void *ptr);

#endif /* __XMALLOC_H__ */
