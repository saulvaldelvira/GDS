/*
 * queue.h - queue_t definition.
 * Author: Saúl Valdelvira (2023)
 */
#pragma once
#ifndef QUEUE_H
#define QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include "compare.h"
#include "attrs.h"

/**
 * The queue_t works in a similar way to a linked_list. But in this case, the elements are removed from the head.
 * This is because a queue_t is a Data Structure that follows a FIFO (First In First Out) dynamic. The first element added (the head)
 * is the first one that goes out.
*/
typedef struct queue queue_t;

/**
 * @param data_size the size of the data being stored.
 * @return a new queue_t structure
*/
NONNULL()
queue_t* queue_init(size_t data_size, comparator_function_t cmp);

/**
 * Changes the comparator function of the queue
 * @param cmp the new comparator function
*/
NONNULL()
void queue_set_comparator(queue_t *queue, comparator_function_t cmp);

/**
 * Changes the destructor function of the queue
 * @param destructor the new destructor function. NULL means no destructor.
*/
NONNULL(1)
void queue_set_destructor(queue_t *queue, destructor_function_t destructor);

/**
 * Adds the element to the queue
 * @return 1 if the operation is successful
*/
NONNULL()
int queue_enqueue(queue_t *queue, void *element);

/**
 * Enqueues the first [array_length] elements in array
*/
NONNULL()
int queue_enqueue_array(queue_t *queue, void *array, size_t array_length);

/**
 * @return the corresponding element of the queue (the one added first) and removes it from the queue
*/
NONNULL()
void* queue_dequeue(queue_t *queue, void *dest);

/**
 * Dequeues [array_length] elements into dest_array
*/
NONNULL()
int queue_dequeue_array(queue_t *queue, void *array, size_t array_length);

/**
 * @return the corresponding element of the queue (the one added first), without removing it
*/
NONNULL()
void* queue_peek(const queue_t *queue, void *dest);

/**
 * @return true if the element is in the queue
*/
NONNULL()
bool queue_exists(const queue_t *queue, void *element);

/**
 * Removes the element from the queue.
 * @return 1 if the element is successfuly deleted
*/
NONNULL()
int queue_remove(queue_t *queue, void *element);

/**
 * Removes all the elements from the queue
*/
NONNULL()
void queue_clear(queue_t *queue);


/**
 * @return the number of elements in the queue_t
*/
NONNULL()
size_t queue_size(const queue_t *queue);

/**
 * @return true if the queue is empty
*/
NONNULL()
bool queue_isempty(const queue_t *queue);

NONNULL()
void queue_free(queue_t *v, ...);

/**
 * Frees all the given queues.
 */
#define queue_free(...) queue_free(__VA_ARGS__, 0L)

/**
 * Frees multiple queues at once.
 * @param n number of pointers to free.
*/
void queue_free_all(unsigned int n, ...);

#ifdef __cplusplus
}
#endif

#endif
