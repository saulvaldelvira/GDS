/*
 * Queue.c - Queue implementation.
 * Author: Saúl Valdelvira (2023)
 */
#include "Queue.h"
#include <stdlib.h>
#include <stdio.h>
#include "./util/error.h"
#include "./util/definitions.h"
#include <string.h>
#include <stdarg.h>
#include <assert.h>

typedef struct QueueNode {
	struct QueueNode *next;
	byte info[];
} QueueNode;

struct Queue {
	QueueNode *head;	///< Head node
	QueueNode *tail;	///< Tail node. Last added node
	size_t data_size;	///< Size (in bytes) of the data type being stored
	size_t n_elements;	///< Number of elements in the queue
	comparator_function_t compare;		///< Comparator function pointer
	destructor_function_t destructor;	///< Destructor function pointer
};

/// INITIALIZE ////////////////////////////////////////////////////////////////

Queue* queue_init(size_t data_size, comparator_function_t cmp){
	assert(cmp && data_size > 0);
	// Allocate queue
	Queue *queue = malloc(sizeof(*queue));
	if (!queue) return NULL;
	// Initialize queue
	queue->head = NULL;
	queue->tail = NULL;
	queue->compare = cmp;
	queue->data_size = data_size;
	queue->n_elements = 0;
	queue->destructor = NULL;
	return queue;
}

void queue_set_comparator(Queue *queue, comparator_function_t cmp){
	if (queue && cmp)
		queue->compare = cmp;
}

void queue_set_destructor(Queue *queue, destructor_function_t destructor){
	if (queue)
		queue->destructor = destructor;
}

static QueueNode* queue_init_node(void *element, size_t size){
	QueueNode *node = malloc(offsetof(QueueNode, info) + size);
	if (!node) return NULL;
	memcpy(node->info, element, size);
	node->next = NULL;
	return node;
}

///////////////////////////////////////////////////////////////////////////////

/// ENQUEUE ///////////////////////////////////////////////////////////////////

int queue_enqueue(Queue *queue, void *element){
	assert(queue && element);
	QueueNode *node = queue_init_node(element, queue->data_size);
	if (!node) return ERROR;
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
	assert(queue && array);
	while (array_length-- > 0){
		int status = queue_enqueue(queue, array);
		if (status != SUCCESS)
			return status;
		array = void_offset(array, queue->data_size);
	}
	return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

/// DEQUEUE ///////////////////////////////////////////////////////////////////

void* queue_dequeue(Queue *queue, void *dest){
	assert(queue && dest);
	if (queue->head == NULL)
		return NULL;
	QueueNode *aux = queue->head;
	queue->head = queue->head->next;
	memcpy(dest, aux->info, queue->data_size);
	free(aux);
	queue->n_elements--;
	return dest;
}

int queue_dequeue_array(Queue *queue, void *array, size_t array_length){
	assert(queue && array);
	while (array_length-- > 0) {
		if (!queue_dequeue(queue, array))
			break;
		array = void_offset(array, queue->data_size);
	}
	return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

/// PEEK-EXISTS-SIZE //////////////////////////////////////////////////////////

void* queue_peek(Queue *queue, void *dest){
	assert(queue && dest);
	if (queue->head == NULL)
		return NULL;
	else
		return memcpy(dest, queue->head->info, queue->data_size);
}

bool queue_exists(Queue *queue, void *element){
	assert(queue && element);
	QueueNode *aux = queue->head;
	while (aux != NULL){
		if(queue->compare(aux->info, element) == 0)
			return true;
		aux = aux->next;
	}
	return false;
}

int queue_remove(Queue *queue, void *element){
	assert(queue && element);
	QueueNode** aux = &queue->head;
	while (*aux != NULL && queue->compare((*aux)->info, element) != 0)
		aux = &(*aux)->next;
	if (!*aux)
		return ELEMENT_NOT_FOUND_ERROR;
	QueueNode *del = *aux;
	*aux = (*aux)->next;
	free(del);
	queue->n_elements--;
	return SUCCESS;
}

size_t queue_size(Queue *queue){
	return queue ? queue->n_elements : 0;
}

bool queue_isempty(Queue *queue){
	return queue ? queue->head == NULL : true;
}

///////////////////////////////////////////////////////////////////////////////

/// FREE //////////////////////////////////////////////////////////////////////

static void queue_free_node(QueueNode *node, destructor_function_t destructor){
	if (node == NULL)
		return;
	if (destructor)
		destructor(node->info);
	queue_free_node(node->next, destructor);
	free(node);
}

void queue_free(Queue *queue){
	if (queue){
		queue_free_node(queue->head, queue->destructor);
		free(queue);
	}
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

void queue_clear(Queue *queue){
	if (queue){
		queue_free_node(queue->head, queue->destructor);
		queue->head = NULL;
		queue->tail = NULL;
	}
}
