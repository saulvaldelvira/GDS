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

typedef struct Queue {
    QueueNode *head;
    QueueNode *tail;
    // Comparator function
    int (*compare) (void*, void*);
    bool free_on_delete;
} Queue;

extern Queue queue_init(int (*cmp) (void*, void*));

extern void queue_configure(Queue *queue, int free_on_delete);

extern int queue_enqueue(Queue *queue, void *element);

extern void* queue_dequeue(Queue *queue);

extern void* queue_peek(Queue queue);

extern bool queue_search(Queue queue, void *element);

extern bool queue_isempty(Queue queue);

extern void queue_free(Queue queue);

extern void queue_reset(Queue *queue);

#endif