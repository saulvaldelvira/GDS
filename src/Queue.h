/*
 * Queue.h - Queue definition.
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
#include "./util/compare.h"

/**
 * The Queue works in a similar way to a LinkedList. But in this case, the elements are removed from the head.
 * This is because a Queue is a Data Structure that follows a FIFO (First In First Out) dynamic. The first element added (the head)
 * is the first one that goes out.
*/
typedef struct Queue Queue;

/**
 * @param data_size the size of the data being stored.
 * @return a new Queue structure
*/
Queue* queue_init(size_t data_size, comparator_function_t cmp);

/**
 * Changes the comparator function of the queue
 * @param cmp the new comparator function
*/
void queue_set_comparator(Queue *queue, comparator_function_t cmp);

/**
 * Changes the destructor function of the queue
 * @param destructor the new destructor function. NULL means no destructor.
*/
void queue_set_destructor(Queue *queue, destructor_function_t destructor);

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
int queue_dequeue_array(Queue *queue, void *array, size_t array_length);

/**
 * @return the corresponding element of the queue (the one added first), without removing it
*/
void* queue_peek(Queue *queue, void *dest);

/**
 * @return true if the element is in the queue
*/
bool queue_exists(Queue *queue, void *element);

/**
 * Removes the element from the queue.
 * @return 1 if the element is successfuly deleted
*/
int queue_remove(Queue *queue, void *element);

/**
 * Removes all the elements from the queue
*/
void queue_clear(Queue *queue);


/**
 * @return the number of elements in the Queue
*/
size_t queue_size(Queue *queue);

/**
 * @return true if the queue is empty
*/
bool queue_isempty(Queue *queue);

void queue_free(Queue *v, ...);

/**
 * Frees all the given queues.
 */
#define queue_free(...) queue_free(__VA_ARGS__, NULL)

/**
 * Frees multiple queues at once.
 * @param n number of pointers to free.
*/
void queue_free_all(unsigned int n, ...);

#ifdef __cplusplus
}
#endif

#endif
