/**
 *  Copyright (C) 2023 - Saúl Valdelvira
 *  License: BSD 3-Clause
 *  Email: saulvaldelvira@gmail.com
 */
#include "Queue.h"
#include <stdlib.h>
#include <stdio.h>
#define GDS_ENABLE_ERROR_MACROS
#include "./Util/error.h"
#include "./Util/definitions.h"
#include <string.h>
#include <stdarg.h>

typedef struct QueueNode {
	struct QueueNode *next;
	byte_t info[];
} QueueNode;

struct Queue {
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
		printerr_null_param();
		return NULL;
	}
	if (data_size == 0){
		printerr_data_size();
		return NULL;
	}
	// Allocate queue
	Queue *queue = malloc(sizeof(*queue));
	if (!queue){
		printerr_allocation();
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
		printerr_null_param();
		return;
	}
	queue->compare = cmp;
}

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
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	QueueNode *node = queue_init_node(element, queue->data_size);
	if (!node){
		return ALLOCATION_ERROR;
	}
	if (!queue->head){
		queue->head = node;
		queue->tail = node;
	}else{
		queue->tail->next = node;
		queue->tail = queue->tail->next;
	}
	queue->n_elements++;
	return SUCCESS;
}

int queue_enqueue_array(Queue *queue, void *array, size_t array_length){
	if (!queue || !array){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	while (array_length-- > 0){
		int status = queue_enqueue(queue, array);
		if (status != SUCCESS){
			return status;
		}
		array = void_offset(array, queue->data_size);
	}
	return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

/// DEQUEUE ///////////////////////////////////////////////////////////////////

void* queue_dequeue(Queue *queue, void *dest){
	if (!queue || !dest){
		printerr_null_param();
		return NULL;
	}
	if (queue->head == NULL){
		return NULL;
	}
	QueueNode *aux = queue->head;
	queue->head = queue->head->next;
	memcpy(dest, aux->info, queue->data_size);
	free(aux);
	queue->n_elements--;
	return dest;
}

int queue_dequeue_array(Queue *queue, void *array, size_t array_length){
	if (!queue || !array){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	while (array_length-- > 0) {
		if (!queue_dequeue(queue, array)){
			break;
		}
		array = void_offset(array, queue->data_size);
	}
	return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

/// PEEK-EXISTS-SIZE //////////////////////////////////////////////////////////

void* queue_peek(Queue *queue, void *dest){
	if (!queue || !dest){
		printerr_null_param();
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
		printerr_null_param();
		return false;
	}
	QueueNode *aux = queue->head;
	while (aux != NULL){
		if(queue->compare(aux->info, element) == 0){
			return true;
		}
		aux = aux->next;
	}
	return false;
}

size_t queue_size(Queue *queue){
	if (!queue){
		printerr_null_param();
		return 0;
	}
	return queue->n_elements;
}

bool queue_isempty(Queue *queue){
	if (!queue){
		printerr_null_param();
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
		printerr_null_param();
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
		printerr_null_param();
		return NULL;
	}
	queue_free_node(queue->head);
	queue->head = NULL;
	queue->tail = NULL;
	return queue;
}
