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

typedef struct deque deque_t;

deque_t* deque_init(size_t data_size, comparator_function_t cmp);
int deque_push_back(deque_t *deque, void *element);
int deque_push_front(deque_t *deque, void *element);

void* deque_pop_back(deque_t *deque, void *dest);
int deque_remove_back(deque_t *deque);
void* deque_pop_front(deque_t *deque, void *dest);
int deque_remove_front(deque_t *deque);

void* deque_at(deque_t *deque, size_t i, void *dest);

size_t deque_size(deque_t *deque);
size_t deque_capacity(deque_t *deque);

void deque_clear(deque_t *deque);
void deque_free(deque_t *v, ...);

/**
 * Frees all the given deques.
 */
#define deque_free(...) deque_free(__VA_ARGS__, 0L)

#ifdef __cplusplus
}
#endif

#endif /* __GDS_DEQUE_H__ */
