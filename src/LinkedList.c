/**
 *  Doubly Linked List implementation.
 *
 *  Copyright (C) 2023 - Saúl Valdelvira
 *  License: BSD 3-Clause
 *  Email: saulvaldelvira@gmail.com
 */
#include "LinkedList.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define GDS_ENABLE_ERROR_MACROS
#include "./Util/error.h"
#include "./Util/definitions.h"
#include <string.h>
#include <stdarg.h>

/**
 * Node of a Linked List
*/
typedef struct LLNode {
	struct LLNode *next;
	struct LLNode *prev;
	byte_t info[];
}LLNode;

struct LinkedList {
	LLNode *head;
	LLNode *tail;
	size_t n_elements;
	size_t data_size;
	// Comparator function for 2 elements
	comparator_function_t compare;
};

/// INITIALIZE ////////////////////////////////////////////////////////////////

LinkedList* list_init(size_t data_size, comparator_function_t cmp){
	if (!cmp){
		printerr_null_param();
		return NULL;
	}
	if (data_size == 0){
		printerr_data_size();
		return NULL;
	}
	LinkedList *list = malloc(sizeof(*list));
	if (!list){
		printerr_allocation();
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
		printerr_null_param();
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
		printerr_allocation();
		return NULL;
	}
	node->next = NULL;
	node->prev = NULL;
	memcpy(node->info, info, size);
	return node;
}

///////////////////////////////////////////////////////////////////////////////

/// ADD-SET ///////////////////////////////////////////////////////////////////////

int list_append(LinkedList *list, void *element){
	if (!list || !element){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	LLNode **ref = list->head == NULL ? // If head NULL
			&list->head 	  : // Add to head
			&list->tail->next;  // Else add to the tail
	*ref = list_innit_node(element, list->data_size);
	if (!*ref){
		printerr_allocation();
		return ALLOCATION_ERROR;
	}
	(*ref)->prev = list->tail;
	list->tail = *ref;
	list->n_elements++;
	return SUCCESS;
}

int list_append_array(LinkedList *list, void *array, size_t array_length){
	if (!list || !array){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	while (array_length-- > 0){
		int status = list_append(list, array);
		if (status != SUCCESS){
			return status;
		}
		array = void_offset(array, list->data_size);
	}
	return SUCCESS;
}

int list_push_front(LinkedList *list, void *element){
	if (!list || !element){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	LLNode *old_head = list->head;
	list->head = list_innit_node(element, list->data_size);
	if(!list->head){
		return ALLOCATION_ERROR;
	}
	list->head->next = old_head;
	if (old_head){
		old_head->prev = list->head;
	}
	list->n_elements++;
	return SUCCESS;
}

int list_push_front_array(LinkedList *list, void *array, size_t array_length){
	if (!list || !array){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	void *tmp = array;
	int status;
	while (array_length-- > 0){
		status = list_push_front(list, tmp);
		if (status != SUCCESS){
			return status;
		}
		tmp = void_offset(tmp, list->data_size);
	}
	return SUCCESS;
}

int list_set(LinkedList *list, void *element, void *replacement){
	if (!list || !element || !replacement){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	LLNode *aux = list->head;
	while ((*list->compare) (aux->info, element) != 0){
		aux = aux->next;
		if(aux == NULL){
			return ELEMENT_NOT_FOUND_ERROR;
		}
	}
	memcpy(aux->info, replacement, list->data_size);
	return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

/// GET ///////////////////////////////////////////////////////////////////////

void* list_get(LinkedList *list, void *element, void *dest){
	if (!list || !element || !dest){
		printerr_null_param();
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
		printerr_null_param();
		return NULL;
	}
	if (list->head == NULL){
		return NULL;
	}
	return memcpy(dest, list->head->info, list->data_size);
}

void* list_get_back(LinkedList *list, void *dest){
	if (!list || !dest){
		printerr_null_param();
		return NULL;
	}
	if (list->tail == NULL){
		return NULL;
	}
	return memcpy(dest, list->tail->info, list->data_size);
}

void* list_get_into_array(LinkedList *list, void *array, size_t array_length){
	if (!list || !array){
		printerr_null_param();
		return NULL;
	}
	if (array_length > list->n_elements){
		array_length = list->n_elements;
	}
	LLNode *aux = list->head;
	void *dst = array;
	while (array_length-- > 0){
		memcpy(dst, aux->info, list->data_size);
		aux = aux->next;
		dst = void_offset(dst, list->data_size);
	}
	return array;

}

void* list_get_array(LinkedList *list, size_t array_length){
	if (!list){
		printerr_null_param();
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
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	LLNode *tmp = list->head;
	while(tmp){
		if (list->compare (tmp->info, element) == 0){
			if (list->tail == tmp){
				list->tail = tmp->prev;
			}else{
				tmp->next->prev = tmp->prev;
			}
			if (list->head == tmp){
				list->head = tmp->next;
			}else{
				tmp->prev->next = tmp->next;
			}
			free(tmp);
			list->n_elements--;
			return SUCCESS;
		}
		tmp = tmp->next;
	}
	return ELEMENT_NOT_FOUND_ERROR;
}

void* list_pop_front(LinkedList *list, void *dest){
	if (!list || !dest){
		printerr_null_param();
		return NULL;
	}
	if (list->head == NULL){
		return NULL;
	}
	memcpy(dest, list->head->info, list->data_size);
	LLNode *del = list->head;
	list->head = list->head->next;
	free(del);
	list->n_elements--;
	return dest;
}

void* list_pop_back(LinkedList *list, void *dest){
	if (!list || !dest){
		printerr_null_param();
		return NULL;
	}
	if (list->tail == NULL){
		return NULL;
	}
	memcpy(dest, list->tail->info, list->data_size);
	LLNode *del = list->tail;
	list->tail = list->tail->prev;
	free(del);
	list->n_elements--;
	return dest;
}

int list_remove_array(LinkedList *list, void *array, size_t array_length){
	if (!list || !array){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	while (array_length-- > 0){
		list_remove(list, array);
		array = void_offset(array, list->data_size);
	}
	return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

/// OTHER /////////////////////////////////////////////////////////////////////

bool list_exists(LinkedList *list, void *element){
	if (!list || !element){
		printerr_null_param();
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
		printerr_null_param();
		return 0;
	}
	return list->n_elements;
}

bool list_isempty(LinkedList *list){
	if (!list){
		printerr_null_param();
		return false;
	}
	return list->n_elements == 0;
}

LinkedList* list_join(LinkedList *list_1, LinkedList *list_2){
	if (!list_1 || !list_2){
		printerr_null_param();
		return NULL;
	}
	if (list_1->data_size != list_2->data_size){
		printerr("The lists have different data sizes (%zu and %zu)",, list_1->data_size, list_2->data_size);
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
			free(list_joint);
			return NULL;
		}
	}
	// Get the elements of the second list
	tmp = list_get_array(list_2, list_2->n_elements);
	if (tmp != NULL){
		// Add the elements of the second list
		status = list_append_array(list_joint, tmp, list_2->n_elements);
		free(tmp);
		if (status != SUCCESS){
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
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	list_free_node(list->head);
	free(list);
	return SUCCESS;
}

void list_free_all(unsigned int n, ...){
	va_list arg;
	va_start(arg, n);
	for (unsigned int i = 0; i < n; i++){
		LinkedList *ptr = va_arg(arg, LinkedList*);
		list_free(ptr);
	}
	va_end(arg);
}

LinkedList* list_reset(LinkedList *list){
	if (!list){
		printerr_null_param();
		return NULL;
	}
	list_free_node(list->head);
	list->head = NULL;
	list->tail = NULL;
	list->n_elements = 0;
	return list;
}
