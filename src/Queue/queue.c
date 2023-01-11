/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 11-01-2023
 * Copyright: Saúl Valdelvira Iglesias (2023)
 * Licensed under the GNU GPL V.3. See /LICENSE file for more info
*/
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include "../Util/error.h"
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
	if (data_size <= 0){
		printerr_data_size(queue_init);
		return NULL;
	}
	if (!cmp){
		printerr_null_param(queue_init);
		return NULL;
	}
	// Allocate queue
	Queue *queue = malloc(sizeof(*queue));
	if (!queue){
		printerr_allocation(queue_init);
		return NULL;
	}
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
		printerr_memory_op(queue_init_node);
		return NULL;
	}
	node->next = NULL;
	return node;
}

int queue_enqueue(Queue *queue, void *element){
	if (!queue || !element){
		printerr_null_param(queue_enqueue);
		return NULL_PARAMETER_ERROR;
	}
	if (queue->head == NULL) {
		queue->head = queue_init_node(element, queue->data_size);
		if (!queue->head){
			return ALLOCATION_ERROR;
		}
		queue->tail = queue->head;
	}else {
		queue->tail->next = queue_init_node(element, queue->data_size);
		if (!queue->tail->next){
			return ALLOCATION_ERROR;
		}
		queue->tail = queue->tail->next;
	}
	return SUCCESS;
}

void* queue_dequeue(Queue *queue, void *dest){
	if (!queue || !dest){
		printerr_null_param(queue_dequeue);
		return NULL;
	}
	if (queue->head == NULL){
		return NULL;
	}
	QueueNode *aux = queue->head;    // Save the head
	queue->head = queue->head->next; // Change it to the next element
	if(!memcpy(dest, aux->info, queue->data_size)){       // Save the element
		printerr_memory_op(queue_dequeue);
		return NULL;
	}
	free(aux);                    // Free the old head
	return dest;                  // Return the element
}

void* queue_peek(Queue *queue, void *dest){
	if (!queue || !dest){
		printerr_null_param(queue_peek);
		return NULL;
	}
	if (queue->head == NULL){
		return NULL;
	}
	else {
		return memcpy(dest, queue->head->info, queue->data_size);
	}
}

bool queue_search(Queue *queue, void *element){
	if (!queue || !element){
		printerr_null_param(queue_search);
		return false;
	}
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
	if (!queue){
		printerr_null_param(queue_isempty);
		return false;
	}
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
	if (!queue){
		printerr_null_param(queue_free);
		return NULL_PARAMETER_ERROR;
	}
	queue_free_node(queue->head);
	free(queue);
	return SUCCESS;
}

Queue* queue_reset(Queue *queue){
	if (!queue){
		printerr_null_param(queue_reset);
		return NULL;
	}
	queue_free_node(queue->head);
	queue->head = NULL;
	queue->tail = NULL;
	return queue;
}