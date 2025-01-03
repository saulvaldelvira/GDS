/*
 * deque.h - deque definition.
 * Author: Saúl Valdelvira (2023)
 */
#pragma once
#ifndef __GDS_DEQUE_H__
#define __GDS_DEQUE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include "compare.h"
#include "attrs.h"

typedef struct deque deque_t;

NONNULL()
deque_t* deque_init(size_t data_size, comparator_function_t cmp);

NONNULL()
int deque_push_back(deque_t *deque, void *element);

NONNULL()
int deque_push_back_array(deque_t *deque, void *array, size_t array_len);

NONNULL()
int deque_push_front(deque_t *deque, void *element);

NONNULL()
int deque_push_front_array(deque_t *deque, void *array, size_t array_len);

NONNULL()
void deque_set_comparator(deque_t *self, comparator_function_t cmp);

NONNULL(1)
void deque_set_destructor(deque_t *self, destructor_function_t f);

NONNULL(1)
void* deque_pop_back(deque_t *deque, void *dest);

NONNULL()
int deque_remove_back(deque_t *deque);

NONNULL()
int deque_remove(deque_t *self, const void *element);

NONNULL()
int deque_remove_at(deque_t *self, ptrdiff_t i);

NONNULL(1)
void* deque_pop_front(deque_t *deque, void *dest);

NONNULL()
int deque_remove_front(deque_t *deque);

NONNULL()
void* deque_at(const deque_t *deque, size_t i, void *dest);

NONNULL()
ptrdiff_t deque_indexof(const deque_t *deque, const void *element);

NONNULL()
bool deque_exists(const deque_t *deque, const void *element);

NONNULL()
size_t deque_size(const deque_t *deque);

NONNULL()
size_t deque_isempty(const deque_t *deque);

NONNULL()
size_t deque_capacity(const deque_t *deque);


NONNULL()
void deque_clear(deque_t *deque);

NONNULL()
void deque_free(deque_t *v, ...);

/**
 * Frees all the given deques.
 */
#define deque_free(...) deque_free(__VA_ARGS__, 0L)

#ifdef __cplusplus
}
#endif

#endif /* __GDS_DEQUE_H__ */
