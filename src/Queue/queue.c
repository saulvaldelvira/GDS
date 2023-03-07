/**
 *  Copyright (C) 2022-2023  Saúl Valdelvira Iglesias
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *  Email: saulvaldelvira@gmail.com
 *  Version: 17-01-2023
 */
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include "../Util/error.h"
#include "../Util/definitions.h"
#include <memory.h>

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

Queue* queue_init(size_t data_size, comparator_function_t cmp){
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
		tmp = queue_dequeue(queue, tmp);
		if (!tmp){
			return MEMORY_OP_ERROR;
		}
	}
	return SUCCESS;
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
