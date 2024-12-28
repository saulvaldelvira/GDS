#include "deque.h"
#include "error.h"
#include "gdsmalloc.h"
#include "definitions.h"
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>

struct deque {
        void *ringbuf;
        size_t head, tail;
        size_t n_elements, capacity;
        comparator_function_t cmp;
        destructor_function_t destructor;
        size_t data_size;
};

#define DEFAULT_RINGBUF_SIZE 8

#define WRAP_INDEX(i, max) ( (i) % max )

deque_t* deque_init(size_t data_size, comparator_function_t cmp) {
        deque_t *deque = gdsmalloc(sizeof(*deque));
        if (!deque)
                return NULL;
        *deque = (deque_t) {
                .cmp = cmp,
                .ringbuf = gdsmalloc(DEFAULT_RINGBUF_SIZE * data_size),
                .data_size = data_size,
                .capacity = DEFAULT_RINGBUF_SIZE,
        };
        if (!deque->ringbuf) {
                gdsfree(deque);
                return NULL;
        }
        return deque;
}

void deque_set_comparator(deque_t *self, comparator_function_t cmp) {
        if (self && cmp)
                self->cmp = cmp;
}

void deque_set_destructor(deque_t *self, destructor_function_t f) {
        assert(self);
        self->destructor = f;
}

static int __deque_expand(deque_t *deque, size_t n) {
        assert(deque);
        deque->ringbuf = gdsrealloc(deque->ringbuf, n * deque->data_size);
        if (!deque->ringbuf)
                return GDS_ERROR;
        deque->capacity = n;
        if (deque->head > deque->tail) {
                size_t move_num = deque->capacity - deque->head;
                size_t move_start = deque->capacity - move_num;
                void *src = void_offset(deque->ringbuf, deque->head * deque->data_size);
                void *dst = void_offset(deque->ringbuf, move_start * deque->data_size);
                memmove(dst, src, move_num * deque->data_size);
                deque->head = move_start;
        }
        return GDS_SUCCESS;
}

__inline
size_t deque_size(const deque_t *deque) {
        assert(deque);
        return deque->n_elements;
}

__inline
size_t deque_isempty(const deque_t *deque) {
        assert(deque);
        return deque->n_elements == 0;
}

__inline
size_t deque_capacity(const deque_t *deque) {
        assert(deque);
        return deque->capacity;
}

#define __advance_tail(deque) { \
                if (deque->tail >= deque->capacity - 1) \
                        deque->tail = 0; \
                else \
                        deque->tail++; }

int deque_push_back(deque_t *deque, void *element) {
        assert(deque && element);
        if (deque->n_elements >= deque->capacity) {
                int ret = __deque_expand(deque, deque->capacity * 2);
                if (unlikely(ret == GDS_ERROR))
                        return GDS_ERROR;
        }
        if (deque->n_elements > 0) {
                __advance_tail(deque);
        }
        copy_into(deque->ringbuf, deque->tail, element, deque->data_size);
        deque->n_elements++;
        return GDS_SUCCESS;
}

int deque_push_back_array(deque_t *deque, void *array, size_t array_len) {
        while (array_len-- > 0) {
                int status = deque_push_back(deque, array);
                if (unlikely(status == GDS_ERROR))
                        return status;
                array = void_offset(array, deque->data_size);
        }
        return GDS_SUCCESS;
}

#define __recede_head(deque) { \
                if (deque->head == 0) \
                        deque->head = deque->capacity - 1; \
                else \
                        deque->head--; }

int deque_push_front(deque_t *deque, void *element) {
        assert(deque && element);
        if (deque->n_elements >= deque->capacity) {
                int ret = __deque_expand(deque, deque->capacity * 2);
                if (unlikely(ret == GDS_ERROR))
                        return GDS_ERROR;
        }
        if (deque->n_elements > 0) {
                __recede_head(deque);
        }
        copy_into(deque->ringbuf, deque->head, element, deque->data_size);
        deque->n_elements++;
        return GDS_SUCCESS;
}

int deque_push_front_array(deque_t *deque, void *array, size_t array_len) {
        while (array_len-- > 0) {
                int status = deque_push_front(deque, array);
                if (unlikely(status == GDS_ERROR))
                        return status;
                array = void_offset(array, deque->data_size);
        }
        return GDS_SUCCESS;
}

#define __recede_tail(deque) {\
        if (deque->tail == 0) \
                deque->tail = deque->capacity - 1; \
        else \
                deque->tail-- ; }

static __inline void* __remove_back(deque_t *deque, void *dest, bool destroy) {
        if (deque->n_elements == 0)
                return NULL;
        void *src = void_offset(deque->ringbuf, deque->tail * deque->data_size);
        if (destroy) {
                if (deque->destructor)
                        deque->destructor(src);
        } else if (dest) {
                memcpy(dest, src, deque->data_size);
        }
        deque->n_elements--;
        if (deque->n_elements > 0)
                __recede_tail(deque);
        return dest;
}

void* deque_pop_back(deque_t *deque, void *dest) {
        assert(deque);
        return __remove_back(deque, dest, false);
}

int deque_remove_back(deque_t *deque) {
        assert(deque);
        __remove_back(deque, NULL, true);
        return GDS_SUCCESS;
}

#define __advance_head(deque) { \
        if (deque->head >= deque->capacity - 1) \
                deque->head = 0; \
        else \
                deque->head++; }

static __inline void* __remove_front(deque_t *deque, void *dest, bool destroy) {
        void *src = void_offset(deque->ringbuf, deque->head * deque->data_size);
        if (deque->n_elements == 0)
                return NULL;
        if (destroy) {
                if (deque->destructor)
                        deque->destructor(src);
        } else if (dest) {
                memcpy(dest, src, deque->data_size);
        }
        deque->n_elements--;
        if (deque->n_elements > 0)
                __advance_head(deque);
        return dest;
}

void* deque_pop_front(deque_t *deque, void *dest) {
        assert(deque);
        return __remove_front(deque, dest, false);
}

int deque_remove_front(deque_t *deque) {
        assert(deque);
        __remove_front(deque, NULL, true);
        return GDS_SUCCESS;
}


static __inline
void* get_at_index(const deque_t *self, size_t i) {
        size_t index = WRAP_INDEX(self->head + i, self->capacity);
        return void_offset(self->ringbuf, index * self->data_size);
}

int deque_remove(deque_t *self, const void *element) {
        assert(self && element);
        ptrdiff_t i = deque_indexof(self, element);
        return deque_remove_at(self, i);
}

int deque_remove_at(deque_t *self, ptrdiff_t i) {
        assert(self);
        if (i < 0 || i >= (ptrdiff_t)self->n_elements)
                return GDS_INDEX_BOUNDS_ERROR;
        while (i < (ptrdiff_t)self->n_elements - 1) {
                void *curr = get_at_index(self, i);
                const void *next = get_at_index(self, i + 1);
                memcpy(curr, next, self->data_size);
        }
        return GDS_SUCCESS;
}

void* deque_at(const deque_t *deque, size_t i, void *dest) {
        assert(deque && dest);
        if (i >= deque->n_elements)
                return NULL;
        return memcpy(
                dest,
                get_at_index(deque, i),
                deque->data_size
        );
}

ptrdiff_t deque_indexof(const deque_t *deque, const void *element) {
        assert(deque && element);
        for (size_t i = 0; i < deque->n_elements; i++) {
                void *e = get_at_index(deque, i);
                if (deque->cmp(e,element) == 0)
                        return i;
        }
        return GDS_ELEMENT_NOT_FOUND_ERROR;
}

bool deque_exists(const deque_t *deque, const void *element) {
        assert(deque && element);
        return deque_indexof(deque, element) != GDS_ELEMENT_NOT_FOUND_ERROR;
}

static __inline
void destroy_content(deque_t *deque){
        if (deque->destructor){
                for (size_t i = 0; i < deque->n_elements; i++){
                        void *ptr = void_offset(deque->ringbuf, deque->head * deque->data_size);
                        deque->destructor(ptr);
                        __advance_head(deque);
                }
        }
}

void deque_clear(deque_t *deque){
        if (deque){
                destroy_content(deque);
                deque->n_elements = 0;
        }
}

static void _deque_free(deque_t *deque){
        if (!deque)
                return;
        destroy_content(deque);
        gdsfree(deque->ringbuf);
        gdsfree(deque);
}

void (deque_free)(deque_t *v, ...){
        if (!v)
                return;
        va_list arg;
        va_start(arg, v);
        do {
                _deque_free(v);
                v = va_arg(arg, deque_t*);
        } while (v);
        va_end(arg);
}

