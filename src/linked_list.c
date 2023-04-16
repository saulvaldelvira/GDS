/**
 *  Copyright (C) 2023 - Saúl Valdelvira
 *
 *  This program is free software. You can modify
 *  and/or redistribute it under the terms of the
 *  GNU General Public License version 2, or newer.
 *  See <https://www.gnu.org/licenses/>.
 *
 *  Email: saulvaldelvira@gmail.com
 *  Version: 17-01-2023
 */
#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "./Util/error.h"
#include "./Util/definitions.h"
#include <string.h>

/**
 * Node of a Linked List
*/
typedef struct LLNode {
	struct LLNode *next; // Pointer to the next node
	struct LLNode *prev; // Pointer to the previous node
	byte_t info[];
}LLNode;

struct _LinkedList {
	LLNode *head;
	LLNode *tail;
	size_t n_elements;
	size_t data_size;
	// Comparator function for 2 elements
	comparator_function_t compare;
};

/// INITIALIZE ////////////////////////////////////////////////////////////////

LinkedList* list_init(size_t data_size, comparator_function_t cmp){
	if (data_size <= 0){
		printerr_data_size(list_init);
		return NULL;
	}
	if (!cmp){
		printerr_null_param(list_init);
		return NULL;
	}
	if (data_size == 0){
		printerr_data_size(list_init);
		return NULL;
	}
	LinkedList *list = malloc(sizeof(*list));
	if (!list){
		printerr_allocation(list_init);
		return NULL;
	}
	list->n_elements = 0;
	list->head = NULL;
	list->tail = NULL;
	list->compare = cmp;
	list->data_size = data_size;
	return list;
}

void list_configure(LinkedList *list, comparator_function_t cmp){
	if (!list || !cmp){
		printerr_null_param(list_configure);
		return;
	}
	list->compare = cmp;
}

/**
 * Initializes a new LLNode with the given info
*/
static LLNode* list_innit_node(void *info, size_t size){
	LLNode *node = malloc(offsetof(LLNode, info) + size);
	if (!node){
		printerr_allocation(list_innit_node);
		return NULL;
	}
	node->next = NULL;
	node->prev = NULL;
	if(!memcpy(node->info, info, size)){
		printerr_memory_op(list_innit_node);
		return NULL;
	}
	return node;
}

///////////////////////////////////////////////////////////////////////////////

/// ADD-SET ///////////////////////////////////////////////////////////////////////

int list_append(LinkedList *list, void *element){
	if (!list || !element){
		printerr_null_param(list_append);
		return NULL_PARAMETER_ERROR;
	}
	LLNode **ref = list->head == NULL ? // If head NULL
			&list->head 	  : // Add to head
			&list->tail->next;  // Else add to the tail
	*ref = list_innit_node(element, list->data_size);
	if (!*ref){
		printerr_allocation(list_append);
		return ALLOCATION_ERROR;
	}
	(*ref)->prev = list->tail;
	list->tail = *ref;
	list->n_elements++;
	return SUCCESS;
}

int list_append_array(LinkedList *list, void *array, size_t array_length){
	if (!list || !array){
		printerr_null_param(list_append_array);
		return NULL_PARAMETER_ERROR;
	}
	void *tmp;
	int status;
	for (size_t i = 0; i < array_length; i++){
		tmp = void_offset(array, i * list->data_size);
		status = list_append(list, tmp);
		if (status != SUCCESS){
			return status;
		}
	}
	return SUCCESS;
}

int list_push_front(LinkedList *list, void *element){
	if (!list || !element){
		printerr_null_param(list_push_front);
		return NULL_PARAMETER_ERROR;
	}
	if (list->head == NULL){
		list->head = list_innit_node(element, list->data_size);
		if(!list->head){
			return ALLOCATION_ERROR;
		}
		list->tail = list->head;
	}else {
		list->head->prev = list_innit_node(element, list->data_size);
		if(!list->head->prev){
			return ALLOCATION_ERROR;
		}
		list->head->prev->next = list->head;
		list->head = list->head->prev;
	}
	list->n_elements++;
	return SUCCESS;
}

int list_push_front_array(LinkedList *list, void *array, size_t array_length){
	if (!list || !array){
		printerr_null_param(list_push_front_array);
		return NULL_PARAMETER_ERROR;
	}
	void *tmp;
	int status;
	for (size_t i = 0; i < array_length; i++){
		tmp = void_offset(array, i * list->data_size);
		status = list_push_front(list, tmp);
		if (status != SUCCESS){
			return status;
		}
	}
	return SUCCESS;
}

int list_set(LinkedList *list, void *element, void *replacement){
	if (!list || !element || !replacement){
		printerr_null_param(list_set);
		return NULL_PARAMETER_ERROR;
	}
	LLNode *aux = list->head;
	while ((*list->compare) (aux->info, element) != 0){
		aux = aux->next;
		if(aux == NULL){
			return ELEMENT_NOT_FOUND_ERROR;
		}
	}
	if(!memcpy(aux->info, replacement, list->data_size)){
		printerr_memory_op(list_set);
		return ALLOCATION_ERROR;
	}
	return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

/// GET ///////////////////////////////////////////////////////////////////////

void* list_get(LinkedList *list, void *element, void *dest){
	if (!list || !element || !dest){
		printerr_null_param(list_get);
		return NULL;
	}
	LLNode *aux = list->head;
	while (aux != NULL && (*list->compare) (aux->info, element) != 0){
		aux = aux->next;
	}
	return aux == NULL ? NULL : memcpy(dest, aux->info, list->data_size);
}

void* list_get_front(LinkedList *list, void *dest){
	if (!list || !dest){
		printerr_null_param(list_get_front);
		return NULL;
	}
	if (list->head == NULL){
		return NULL;
	}
	return memcpy(dest, list->head->info, list->data_size);
}

void* list_get_back(LinkedList *list, void *dest){
	if (!list || !dest){
		printerr_null_param(list_get_back);
		return NULL;
	}
	if (list->tail == NULL){
		return NULL;
	}
	return memcpy(dest, list->tail->info, list->data_size);
}

void* list_get_into_array(LinkedList *list, void *array, size_t array_length){
	if (!list || !array){
		printerr_null_param(list_get_into_array);
		return NULL;
	}
	if (array_length > list->n_elements){
		array_length = list->n_elements;
	}
	LLNode *aux = list->head;
	for (size_t i = 0; i < array_length; i++){
		void *dst = void_offset(array, i * list->data_size);
		if (!memcpy(dst, aux->info, list->data_size)){
			printerr_memory_op(list_get_into_array);
			return NULL;
		}
		aux = aux->next;
	}
	return array;

}

void* list_get_array(LinkedList *list, size_t array_length){
	if (!list){
		printerr_null_param(list_get_array);
		return NULL;
	}
	if (array_length == GET_ALL_ELEMENTS || array_length > list->n_elements){
		array_length = list->n_elements;
	}
	void *array = malloc(list->data_size * array_length);
	if (!array){
		return NULL;
	}
	if (!list_get_into_array(list, array, array_length)){
		free(array);
		return NULL;
	}
	return array;
}

///////////////////////////////////////////////////////////////////////////////

/// REMOVE ////////////////////////////////////////////////////////////////////

int list_remove(LinkedList *list, void *element){
	if (!list || !element){
		printerr_null_param(list_remove);
		return NULL_PARAMETER_ERROR;
	}
	LLNode **tmp = &list->head;
	while((*tmp) != NULL && list->compare ((*tmp)->info, element) != 0){
		tmp = &(*tmp)->next;
	}
	if((*tmp) != NULL){
		LLNode *del = *tmp;

		if (list->tail == *tmp){
			list->tail = (*tmp)->prev;
		}else {
			(*tmp)->next->prev = (*tmp)->prev;
		}
		*tmp = (*tmp)->next;

		free(del);
		list->n_elements--;
		return SUCCESS;
	}
	return ELEMENT_NOT_FOUND_ERROR;
}

void* list_pop_front(LinkedList *list, void *dest){
	if (!list || !dest){
		printerr_null_param(list_pop_front);
		return NULL;
	}
	if (list->head == NULL){
		return NULL;
	}
	dest = memcpy(dest, list->head->info, list->data_size);
	if (!dest){
		printerr_memory_op(list_pop_front);
		return NULL;
	}

	LLNode *del = list->head;
	list->head = list->head->next;
	free(del);
	list->n_elements--;
	return dest;
}

void* list_pop_back(LinkedList *list, void *dest){
	if (!list || !dest){
		printerr_null_param(list_pop_back);
		return NULL;
	}
	if (list->tail == NULL){
		return NULL;
	}
	dest = memcpy(dest, list->tail->info, list->data_size);
	if (!dest){
		printerr_memory_op(list_pop_back);
		return NULL;
	}

	LLNode *del = list->tail;
	list->tail = list->tail->prev;
	free(del);
	list->n_elements--;
	return dest;
}

int list_remove_array(LinkedList *list, void *array, size_t array_length){
	if (!list || !array){
		printerr_null_param(list_push_remove_array);
		return NULL_PARAMETER_ERROR;
	}
	void *tmp;
	int status;
	for (size_t i = 0; i < array_length; i++){
		tmp = void_offset(array, i * list->data_size);
		status = list_remove(list, tmp);
		if (status != SUCCESS){
			return status;
		}
	}
	return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

/// OTHER /////////////////////////////////////////////////////////////////////

bool list_exists(LinkedList *list, void *element){
	if (!list || !element){
		printerr_null_param(list_exists);
		return false;
	}
	LLNode *aux = list->head;
	while (aux != NULL){
		if ((*list->compare) (aux->info, element) == 0){
			return true;
		}
		aux = aux->next;
	}
	return false;
}

size_t list_size(LinkedList *list){
	if (!list){
		printerr_null_param(list_size);
		return 0;
	}
	return list->n_elements;
}

bool list_isempty(LinkedList *list){
	if (!list){
		printerr_null_param(list_isempty);
		return false;
	}
	return list->n_elements == 0;
}

LinkedList* list_join(LinkedList *list_1, LinkedList *list_2){
	if (!list_1 || !list_2){
		printerr_null_param(list_join);
		return NULL;
	}
	if (list_1->data_size != list_2->data_size){
		fprintf(stderr, "ERROR: the lists have different data sizes. In function list_join\n");
		return NULL;
	}

	LinkedList *list_joint = list_init(list_1->data_size, list_1->compare);
	if (!list_joint){
		return NULL;
	}

	int status;

	// Get the elements of the first list
	void *tmp = list_get_array(list_1, list_1->n_elements);
	if (tmp != NULL){
		// Add the elements of the first list
		status = list_append_array(list_joint, tmp, list_1->n_elements);
		free(tmp);
		if (status != SUCCESS){
			goto exit_err;
		}
	}

	// Get the elements of the second list
	tmp = list_get_array(list_2, list_2->n_elements);
	if (tmp != NULL){
		// Add the elements of the second list
		status = list_append_array(list_joint, tmp, list_2->n_elements);
		free(tmp);
		if (status != SUCCESS){
			exit_err:
			free(list_joint);
			return NULL;
		}
	}

	return list_joint;
}

///////////////////////////////////////////////////////////////////////////////

/// FREE //////////////////////////////////////////////////////////////////////

static void list_free_node(LLNode *node){
	if (node == NULL){
		return;
	}
	list_free_node(node->next);
	free(node);
}

int list_free(LinkedList *list){
	if (!list){
		printerr_null_param(list_free);
		return NULL_PARAMETER_ERROR;
	}
	list_free_node(list->head);
	free(list);
	return SUCCESS;
}

LinkedList* list_reset(LinkedList *list){
	if (!list){
		printerr_null_param(list_reset);
		return NULL;
	}
	list_free_node(list->head);
	list->head = NULL;
	list->tail = NULL;
	list->n_elements = 0;
	return list;
}
