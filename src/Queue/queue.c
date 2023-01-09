/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 08-01-2023
 * Copyright: Saúl Valdelvira Iglesias (2023)
 * Licensed under the GNU GPL V.3. See /LICENSE file for more info
*/
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include "../Util/checks.h"
#include <memory.h>

struct QueueNode {
    struct QueueNode *next;
    unsigned char info[];
};

Queue queue_init(size_t data_size, int (*cmp) (const void*, const void*)){
    return (Queue) {
        .head = NULL,
        .tail = NULL,
        .compare = cmp,
        .data_size = data_size
    };
}

/**
 * Initializes a new QueueNode with the element
*/
static QueueNode* queue_init_node(void *element, size_t size){
    QueueNode *node = malloc(offsetof(QueueNode, info) + size);
    if(!memcpy(node->info, element, size)){
        fprintf(stderr, "ERROR: Could not init node\n");
        return NULL;
    }
    node->next = NULL;
    return node;
}

int queue_enqueue(Queue *queue, void *element){
    CHECK_NULL(queue, queue_enqueue, NULL_PARAMETER)
    if (queue->head == NULL) {
        queue->head = queue_init_node(element, queue->data_size);
        CHECK_MEMORY(queue->head, queue_enqueue, ALLOCATION_ERROR)
        queue->tail = queue->head;
    }else {
        queue->tail->next = queue_init_node(element, queue->data_size);
        CHECK_MEMORY(queue->tail->next, queue_enqueue, ALLOCATION_ERROR)
        queue->tail = queue->tail->next;
    }
    return 1;
}

void* queue_dequeue(Queue *queue, void *dest){
    CHECK_NULL(queue, queue_dequeue, NULL)
    if (queue->head == NULL){
        return NULL;
    }
    QueueNode *aux = queue->head;    // Save the head
    queue->head = queue->head->next; // Change it to the next element
    if(!memcpy(dest, aux->info, queue->data_size)){       // Save the element
        fprintf(stderr, "ERROR: could not dequeue node\n");
        return NULL;
    }
    free(aux);                       // Free the old head
    return dest;                  // Return the element
}

void* queue_peek(Queue queue, void *dest){
    if (queue.head == NULL){
        return NULL;
    }
    else {
        return memcpy(dest, queue.head->info, queue.data_size);
    }
}

bool queue_search(Queue queue, void *element){
    QueueNode *aux = queue.head;
    while (aux != NULL){
        if((*queue.compare) (aux->info, element) == 0){
            return true;
        }
        aux = aux->next;
    }
    return false;
}

bool queue_isempty(Queue queue){
    return queue.head == NULL;
}

static void queue_free_node(QueueNode *node){
    if (node == NULL){
        return;
    }
    queue_free_node(node->next);
    free(node);
}

void queue_free(Queue queue){
    queue_free_node(queue.head);
}

void queue_reset(Queue *queue){
    queue_free_node(queue->head);
    queue->head = NULL;
    queue->tail = NULL;
}