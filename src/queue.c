/*
 * queue.c - queue implementation.
 * Author: Saúl Valdelvira (2023)
 */
#include <stddef.h>
#include <stdlib.h>
#include "error.h"
#include "definitions.h"
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include "gdsmalloc.h"
#include "deque.h"

#define queue_t deque_t
#include "queue.h"

/// INITIALIZE ////////////////////////////////////////////////////////////////

queue_t* queue_init(size_t data_size, comparator_function_t cmp){
        assert(cmp && data_size > 0);
        return deque_init(data_size, cmp);
}

void queue_set_comparator(queue_t *queue, comparator_function_t cmp){
        deque_set_comparator(queue, cmp);
}

void queue_set_destructor(queue_t *queue, destructor_function_t destructor){
        deque_set_destructor(queue, destructor);
}

///////////////////////////////////////////////////////////////////////////////

/// ENQUEUE ///////////////////////////////////////////////////////////////////

int queue_enqueue(queue_t *queue, void *element){
        assert(queue && element);
        return deque_push_back(queue, element);
}

int queue_enqueue_array(queue_t *queue, void *array, size_t array_length){
        assert(queue && array);
        return deque_push_back_array(queue, array, array_length);
}

///////////////////////////////////////////////////////////////////////////////

/// DEQUEUE ///////////////////////////////////////////////////////////////////

void* queue_dequeue(queue_t *queue, void *dest){
        assert(queue && dest);
        return deque_pop_front(queue, dest);
}

///////////////////////////////////////////////////////////////////////////////

/// PEEK-EXISTS-SIZE //////////////////////////////////////////////////////////

void* queue_peek(const queue_t *queue, void *dest){
        assert(queue && dest);
        return deque_at(queue, 0, dest);
}

bool queue_exists(const queue_t *queue, void *element){
        assert(queue && element);
        return deque_exists(queue, element);
}

int queue_remove(queue_t *queue, void *element){
        assert(queue && element);
        return deque_remove(queue, element);
}

size_t queue_size(const queue_t *queue){
        return deque_size(queue);
}

bool queue_isempty(const queue_t *queue){
        return deque_isempty(queue);
}

void queue_clear(queue_t *queue){
        if (queue)
                deque_clear(queue);
}

///////////////////////////////////////////////////////////////////////////////

/// FREE //////////////////////////////////////////////////////////////////////

void (queue_free)(queue_t *q, ...){
        if (!q)
                return;
        va_list arg;
        va_start(arg, q);
        do {
                deque_free(q);
                q = va_arg(arg, queue_t*);
        } while (q);
        va_end(arg);
}
