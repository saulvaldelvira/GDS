/**
 *  Copyright (C) 2023 - Saúl Valdelvira
 *
 *  This library is free software. You can modify
 *  and/or redistribute it under the terms of the
 *  GNU Lesser General Public License version 2.1
 *  See <https://www.gnu.org/licenses/>
 *
 *  Email: saulvaldelvira@gmail.com
 */
#pragma once
#ifndef QUEUE_H
#define QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include "./Util/comparator.h"

/**
 * The Queue works in a similar way to a LinkedList. But in this case, the elements are removed from the head.
 * This is because a Queue is a Data Structure that follows a FIFO (First In First Out) dynamic. The first element added (the head)
 * is the first one that goes out.
*/
typedef struct _Queue Queue;

/**
 * @param data_size the size of the data being stored.
 * @return a new Queue structure
*/
Queue* queue_init(size_t data_size, comparator_function_t cmp);

/**
 * Changes the comparator function of the queue
 * @param cmp the new comparator function
*/
void queue_configure(Queue *queue, comparator_function_t cmp);

/**
 * Adds the element to the queue
 * @return 1 if the operation is successful
*/
int queue_enqueue(Queue *queue, void *element);

/**
 * Enqueues the first [array_length] elements in array
*/
int queue_enqueue_array(Queue *queue, void *array, size_t array_length);

/**
 * @return the corresponding element of the queue (the one added first) and removes it from the queue
*/
void* queue_dequeue(Queue *queue, void *dest);

/**
 * Dequeues [array_length] elements into dest_array
*/
int queue_dequeue_array(Queue *queue, void *dest_array, size_t dest_length);

/**
 * @return the corresponding element of the queue (the one added first), without removing it
*/
void* queue_peek(Queue *queue, void *dest);

/**
 * @return true if the element is in the queue
*/
bool queue_exists(Queue *queue, void *element);

/**
 * @return the number of elements in the Queue
*/
size_t queue_size(Queue *queue);

/**
 * @return true if the queue is empty
*/
bool queue_isempty(Queue *queue);

/**
 * Frees the memory allocated for the queue.
 * @return 1 if the operation is successful
*/
int queue_free(Queue *queue);

/**
 * Frees multiple queues at once.
 * @n number of pointers to free.
*/
void queue_free_all(unsigned int n, ...);

/**
 * Frees the memory allocated for the queue AND resets it to the initial state of a Queue
 * @return the same pointer if sucess, NULL if the operation fails
*/
Queue* queue_reset(Queue *queue);

#ifdef __cplusplus
}
#endif

#endif
