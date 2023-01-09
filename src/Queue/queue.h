/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 08-01-2023
 * Copyright: Saúl Valdelvira Iglesias (2023)
 * Licensed under the GNU GPL V.3. See /LICENSE file for more info
*/
#pragma once
#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct QueueNode QueueNode;

/**
 * The Queue works in a similar way to a LinkedList. But in this case, the elements are removed from the head.
 * This is because a Queue is a Data Structure that follows a FIFO (First In First Out) dynamic. The first element added (the head)
 * is the first one that goes out.
*/
typedef struct Queue {
    QueueNode *head;
    QueueNode *tail;
    size_t data_size;
    // Comparator function
    int (*compare) (const void*, const void*);
} Queue;

/**
 * @param data_size the size of the data being stored.
 * @return a new Queue structure
*/
Queue queue_init(size_t data_size, int (*cmp) (const void*, const void*));

/**
 * Adds the element to the queue
 * @return 1 if the operation is successful
*/
int queue_enqueue(Queue *queue, void *element);

/**
 * @return the corresponding element of the queue (the one added first) and removes it from the queue
*/
void* queue_dequeue(Queue *queue, void *dest);

/**
 * @return the corresponding element of the queue (the one added first), without removing it
*/
void* queue_peek(Queue queue, void *dest);

/**
 * @return true if the element is in the queue
*/
bool queue_search(Queue queue, void *element);

/**
 * @return true if the queue is empty
*/
bool queue_isempty(Queue queue);

/**
 * Frees the memory allocated for the queue.
*/
void queue_free(Queue queue);

/**
 * Frees the memory allocated for the queue AND resets it to the initial state of a Queue
 * @return 1 if the operation is successful
*/
int queue_reset(Queue *queue);

#endif