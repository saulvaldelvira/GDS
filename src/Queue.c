/**
 *  Copyright (C) 2023 - Saúl Valdelvira
 *
 *  This library is free software. You can modify
 *  and/or redistribute it under the terms of the
 *  GNU Lesser General Public License version 2.1
 *  See <https://www.gnu.org/licenses/>
 *
 *  Email: saulvaldelvira@gmail.com
 */
#include "Queue.h"
#include <stdlib.h>
#include <stdio.h>
#include "./Util/error.h"
#include "./Util/definitions.h"
#include <string.h>
#include <stdarg.h>

typedef struct QueueNode {
	struct QueueNode *next;
	byte_t info[];
} QueueNode;

struct _Queue {
	QueueNode *head;
	QueueNode *tail;
	size_t data_size;
	size_t n_elements;
	// Comparator function
	comparator_function_t compare;
};

/// INITIALIZE ////////////////////////////////////////////////////////////////

Queue* queue_init(size_t data_size, comparator_function_t cmp){
	if (!cmp){
		printerr_null_param(queue_init);
		return NULL;
	}
	if (data_size == 0){
		printerr_data_size(queue_init);
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
	queue->n_elements = 0;
	return queue;
}

void queue_configure(Queue *queue, comparator_function_t cmp){
	if (!queue || !cmp){
		printerr_null_param(queue_configure);
		return;
	}
	queue->compare = cmp;
}

/**
 * Initializes a new QueueNode with the element
*/
static QueueNode* queue_init_node(void *element, size_t size){
	QueueNode *node = malloc(offsetof(QueueNode, info) + size);
	memcpy(node->info, element, size);
	node->next = NULL;
	return node;
}

///////////////////////////////////////////////////////////////////////////////

/// ENQUEUE ///////////////////////////////////////////////////////////////////

int queue_enqueue(Queue *queue, void *element){
	if (!queue || !element){
		printerr_null_param(queue_enqueue);
		return NULL_PARAMETER_ERROR;
	}
	if (queue->head == NULL){
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
	queue->n_elements++;
	return SUCCESS;
}

int queue_enqueue_array(Queue *queue, void *array, size_t array_length){
	if (!queue || !array){
		printerr_null_param(queue_enqueue_array);
		return NULL_PARAMETER_ERROR;
	}
	void *tmp;
	int status;
	for (size_t i = 0; i < array_length; i++){
		tmp = void_offset(array, i * queue->data_size);
		status = queue_enqueue(queue, tmp);
		if (status != SUCCESS){
			return status;
		}
	}
	return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

/// DEQUEUE ///////////////////////////////////////////////////////////////////

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
	memcpy(dest, aux->info, queue->data_size); // Save the element
	free(aux);                    // Free the old head
	queue->n_elements--;
	return dest;                  // Return the element
}

int queue_dequeue_array(Queue *queue, void *dest_array, size_t dest_length){
	if (!queue || !dest_array){
		printerr_null_param(queue_enqueue_array);
		return NULL_PARAMETER_ERROR;
	}
	void *tmp;
	for (size_t i = 0; i < dest_length; i++){
		tmp = void_offset(dest_array, i * queue->data_size);
		queue_dequeue(queue, tmp);
	}
	return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

/// PEEK-EXISTS-SIZE //////////////////////////////////////////////////////////

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

bool queue_exists(Queue *queue, void *element){
	if (!queue || !element){
		printerr_null_param(queue_exists);
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

size_t queue_size(Queue *queue){
	if (!queue){
		printerr_null_param(queue_size);
		return 0;
	}
	return queue->n_elements;
}

bool queue_isempty(Queue *queue){
	if (!queue){
		printerr_null_param(queue_isempty);
		return false;
	}
	return queue->head == NULL;
}

///////////////////////////////////////////////////////////////////////////////

/// FREE //////////////////////////////////////////////////////////////////////

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

void queue_free_all(unsigned int n, ...){
	va_list arg;
	va_start(arg, n);
	for (unsigned int i = 0; i < n; i++){
		Queue *ptr = va_arg(arg, Queue*);
		queue_free(ptr);
	}
	va_end(arg);
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
