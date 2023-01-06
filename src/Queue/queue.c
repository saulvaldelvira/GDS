#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include "../Util/checks.h"


Queue queue_init(int (*cmp) (void*, void*)){
    return (Queue) {
        .head = NULL,
        .tail = NULL,
        .compare = cmp
    };
}

/**
 * Initializes a new QueueNode with the element
*/
static QueueNode* queue_init_node(void *element){
    QueueNode *node = malloc(sizeof(QueueNode));
    node->info = element;
    node->next = NULL;
    return node;
}

int queue_enqueue(Queue *queue, void *element){
    CHECK_NULL(queue == NULL || element == NULL, queue_enqueue, NULL_PARAMETER)
    if (queue->head == NULL) {
        queue->head = queue_init_node(element);
        CHECK_MEMORY(queue->head, queue_enqueue, ALLOCATION_ERROR)
        queue->tail = queue->head;
    }else {
        queue->tail->next = queue_init_node(element);
        CHECK_MEMORY(queue->tail->next, queue_enqueue, ALLOCATION_ERROR)
        queue->tail = queue->tail->next;
    }
    return 1;
}

void* queue_dequeue(Queue *queue){
    CHECK_NULL(queue == NULL, queue_dequeue, NULL)
    if (queue->head == NULL){
        return NULL;
    }
    QueueNode *aux = queue->head;    // Save the head
    queue->head = queue->head->next; // Change it to the next element
    void *element = aux->info;       // Save the element
    free(aux);                       // Free the old head
    return element;                  // Return the element
}

void* queue_peek(Queue queue){
    if (queue.head == NULL){
        return NULL;
    }
    else {
        return queue.head->info;
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

static void queue_free_node(QueueNode *node, free_on_delete_t free_element){
    if (node == NULL){
        return;
    }
    if (free_element == FreeOnDelete){
        free(node->info);
    }
    queue_free_node(node->next, free_element);
    free(node);
}

void queue_free(Queue queue){
    queue_free_node(queue.head, queue.free_on_delete);
}

void queue_reset(Queue *queue){
    queue_free_node(queue->head, queue->free_on_delete);
    queue->head = NULL;
    queue->tail = NULL;
}