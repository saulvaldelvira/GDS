/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 03/01/2023
*/
#pragma once
#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

// free_on_delete posible values
#ifndef FREE_ON_DELETE
    #define FREE_ON_DELETE 1
#endif

#ifndef DONT_FREE_ON_DELETE
    #define DONT_FREE_ON_DELETE 0
#endif

typedef struct QueueNode {
    void *info;
    struct QueueNode *next;
} QueueNode;

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
    bool free_on_delete;
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
static inline void queue_configure(Queue *queue, int free_on_delete){
    queue->free_on_delete = free_on_delete;
}

extern int queue_enqueue(Queue *queue, void *element);

extern void* queue_dequeue(Queue *queue);

extern void* queue_peek(Queue queue);

extern bool queue_search(Queue queue, void *element);

extern bool queue_isempty(Queue queue);

extern void queue_free(Queue queue);

extern void queue_reset(Queue *queue);

#endif