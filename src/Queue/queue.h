/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 03/01/2023
*/
#pragma once
#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

#include "../definitions.h"

typedef struct QueueNode QueueNode;

/**
 * The Queue works in a similar way to a LinkedList. But in this case, the elements are removed from the head.
 * This is because a Queue is a Data Structure that follows a FIFO (First In First Out) dynamic. The first element added (the head)
 * is the first one that goes out.
*/
typedef struct Queue {
    QueueNode *head;
    QueueNode *tail;
    // Comparator function
    int (*compare) (void*, void*);
    free_on_delete_t free_on_delete;
} Queue;

/**
 * @return a new Queue structure
*/
extern Queue queue_init(int (*cmp) (void*, void*));

/**
 * \brief Configures the Queue.free_on_delete parameter. 
 * \note This can also be achieved by modifying the free_on_delete value itself, without any function call.
 * That's why i made it inline. The function is just so it's easy to understand what this instructiong does
*/
static inline void queue_configure(Queue *queue, free_on_delete_t free_on_delete){
    queue->free_on_delete = free_on_delete;
}

/**
 * Adds the element to the queue
*/
extern int queue_enqueue(Queue *queue, void *element);

/**
 * @return the corresponding element of the queue (the one added first) and removes it from the queue
*/
extern void* queue_dequeue(Queue *queue);

/**
 * @return the corresponding element of the queue (the one added first), without removing it
*/
extern void* queue_peek(Queue queue);

/**
 * @return true if the element is in the queue
*/
extern bool queue_search(Queue queue, void *element);

/**
 * @return true if the queue is empty
*/
extern bool queue_isempty(Queue queue);

/**
 * Frees the memory allocated for the queue
 * @note The behviour of this routine towards the memory for the elements of the Queue (the void pointers) is defined by 
 * stack.free_on_delete. See the Stack struct documentation for more info.
*/
extern void queue_free(Queue queue);

/**
 * Frees the memory allocated for the queue AND resets it to the initial state of a Queue
 * @note The behviour of this routine towards the memory for the elements of the Queue (the void pointers) is defined by 
 * stack.free_on_delete. See the Stack struct documentation for more info.
*/
extern void queue_reset(Queue *queue);

#endif