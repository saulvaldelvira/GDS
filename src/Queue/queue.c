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
#include "../Util/definitions.h"
#include <memory.h>

typedef struct QueueNode {
	struct QueueNode *next;
	unsigned char info[];
} QueueNode;

struct _Queue {
	QueueNode *head;
	QueueNode *tail;
	size_t data_size;
	// Comparator function
	int (*compare) (const void*, const void*);
};

Queue* queue_init(size_t data_size, int (*cmp) (const void*, const void*)){
	CHECK_NULL(cmp, queue_init, NULL)
	// Allocate queue
	Queue *queue = malloc(sizeof(Queue));
	CHECK_MEMORY(queue, queue_init, NULL)
	// Initialize queue
	queue->head = NULL;
	queue->tail = NULL;
	queue->compare = cmp;
	queue->data_size = data_size;
	return queue;
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
	CHECK_NULL(element, queue_enqueue, NULL_PARAMETER)
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
	CHECK_NULL(dest, queue_dequeue, NULL)
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

void* queue_peek(Queue *queue, void *dest){
	CHECK_NULL(queue, queue_peek, NULL)
	CHECK_NULL(dest, queue_peek, NULL)
	if (queue->head == NULL){
		return NULL;
	}
	else {
		return memcpy(dest, queue->head->info, queue->data_size);
	}
}

bool queue_search(Queue *queue, void *element){
	CHECK_NULL(queue, queue_search, false)
	CHECK_NULL(element, queue_search, false)
	QueueNode *aux = queue->head;
	while (aux != NULL){
		if((*queue->compare) (aux->info, element) == 0){
			return true;
		}
		aux = aux->next;
	}
	return false;
}

bool queue_isempty(Queue *queue){
	CHECK_NULL(queue, queue_isempty, false)
	return queue->head == NULL;
}

static void queue_free_node(QueueNode *node){
	if (node == NULL){
		return;
	}
	queue_free_node(node->next);
	free(node);
}

int queue_free(Queue *queue){
	CHECK_NULL(queue, queue_free, NULL_PARAMETER)
	queue_free_node(queue->head);
	free(queue);
	return 1;
}

Queue* queue_reset(Queue *queue){
	CHECK_NULL(queue, queue_reset, NULL)
	queue_free_node(queue->head);
	queue->head = NULL;
	queue->tail = NULL;
	return queue;
}