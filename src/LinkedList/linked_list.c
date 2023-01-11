/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 08-01-2023
 * Copyright: Saúl Valdelvira Iglesias (2023)
 * Licensed under the GNU GPL V.3. See /LICENSE file for more info
*/
#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../Util/error.h"
#include "../Util/definitions.h"
#include <memory.h>

/**
 * Node of a Linked List
*/
typedef struct LLNode {
	struct LLNode *next; // Pointer to the next node
	struct LLNode *previous; // Pointer to the previous node
	unsigned char info[];
}LLNode;

struct _LinkedList {
	LLNode *head;
	LLNode *tail;
	size_t n_elements;
	size_t data_size;
	// Comparator function for 2 elements
	int (*compare) (const void*, const void*);
};

LinkedList* lnkd_list_init(size_t data_size, int (*cmp) (const void*, const void*)){
	if (data_size <= 0){
		printerr_data_size(lnkd_list_init);
		return NULL;
	}
	if (!cmp){
		printerr_null_param(lnkd_list_init);
		return NULL;
	}
	LinkedList *list = malloc(sizeof(*list));
	if (!list){
		printerr_allocation(lnkd_list_init);
		return NULL;
	}
	list->n_elements = 0;
	list->head = NULL;
	list->tail = NULL;
	list->compare = cmp;
	list->data_size = data_size;
	return list;
}

/**
 * Initializes a new LLNode with the given info
*/
static LLNode* lnkd_list_innit_node(void *info, size_t size){
	LLNode *node = malloc(offsetof(LLNode, info) + size);
	if (!node){
		printerr_allocation(lnkd_list_innit_node);
		return NULL;
	}
	node->next = NULL;
	node->previous = NULL;
	if(!memcpy(node->info, info, size)){
		printerr_memory_op(lnkd_list_innit_node);
		return NULL;
	}
	return node;
}

int lnkd_list_push_back(LinkedList *list, void *element){
	if (!list || !element){
		printerr_null_param(lnkd_list_push_back);
		return NULL_PARAMETER_ERROR;
	}
	if(list->n_elements == 0){ // We add to the head
		list->head = lnkd_list_innit_node(element, list->data_size);
		if(!list->head){
			return ALLOCATION_ERROR;
		}
		list->tail = list->head;
	}else{ // We add to the tail
		list->tail->next = lnkd_list_innit_node(element, list->data_size);
		if(!list->tail->next){
			return ALLOCATION_ERROR;
		}
		list->tail->next->previous = list->tail; // Create the "previous" reference in this new element
		list->tail = list->tail->next; // Update the tail to this new element
	}
	list->n_elements++;
	return SUCCESS;
}

int lnkd_list_push_front(LinkedList *list, void *element){
	if (!list || !element){
		printerr_null_param(lnkd_list_push_front);
		return NULL_PARAMETER_ERROR;
	}
	if(list->n_elements == 0){ // We add to the head
		list->head = lnkd_list_innit_node(element, list->data_size);
		if(!list->head){
			return ALLOCATION_ERROR;
		}
		list->tail = list->head;
	}else{ // We add to the head
		LLNode* aux = lnkd_list_innit_node(element, list->data_size);
		if (!aux){
			return ALLOCATION_ERROR;
		}
		aux->next = list->head;
		list->head->previous = aux;
		list->head = aux;
	}
	list->n_elements++;
	return SUCCESS;
}

int lnkd_list_set(LinkedList *list, void *element, void *replacement){
	if (!list || !element || !replacement){
		printerr_null_param(lnkd_list_set);
		return NULL_PARAMETER_ERROR;
	}
	LLNode *aux = list->head;
	while ((*list->compare) (aux->info, element) != 0) {
		aux = aux->next;
		if(aux == NULL){
			return ELEMENT_NOT_FOUND_ERROR;
		}
	}
	if(!memcpy(aux->info, replacement, list->data_size)){
		printerr_memory_op(lnkd_list_set);
		return ALLOCATION_ERROR;
	}
	return SUCCESS;
}

void* lnkd_list_get(LinkedList *list, void *element, void *dest){
	if (!list || !element || !dest){
		printerr_null_param(lnkd_list_get);
		return NULL;
	}
	LLNode *aux = list->head;
	while (aux != NULL && (*list->compare) (aux->info, element) != 0) {
		aux = aux->next;
	}
	return aux == NULL ? NULL : memcpy(dest, aux->info, list->data_size);
}

bool lnkd_list_exists(LinkedList *list, void *element){
	if (!list || !element){
		printerr_null_param(lnkd_list_exists);
		return false;
	}
	LLNode *aux = list->head;
	while (aux != NULL) {
		if ((*list->compare) (aux->info, element) == 0){
			return true;
		}
		aux = aux->next;
	}
	return false;
}

size_t lnkd_list_n_elements(LinkedList *list){
	if (!list){
		printerr_null_param(lnkd_list_n_elements);
		return 0;
	}
	return list->n_elements;
}

bool lnkd_list_isempty(LinkedList *list){
	if (!list){
		printerr_null_param(lnkd_list_isempty);
		return false;
	}
	return list->n_elements == 0;
}

int lnkd_list_remove(LinkedList *list, void *element){
	if (!list || !element){
		printerr_null_param(lnkd_list_remove);
		return NULL_PARAMETER_ERROR;
	}
	LLNode *aux = list->head;
	while(aux != NULL && (*list->compare) (aux->info, element) != 0){
		aux = aux->next;
	}
	if(aux != NULL){
		if (aux == list->head){ // If the element to delete is the head
			list->head = list->head->next;
		}else{
			if (aux->next == NULL){// We're in the last node, have to change the tail pointer
			list->tail = aux->previous;
			} else { // If there's a node after next, change it's "previous" pointer
			aux->next->previous = aux->previous;
			}
			aux->previous->next = aux->next;
		}

		free(aux);
		list->n_elements--;
		return SUCCESS;
	}
	return ELEMENT_NOT_FOUND_ERROR;
}

static void lnkd_list_free_node(LLNode *node){
	if (node == NULL) {
		return;
	}
	lnkd_list_free_node(node->next);
	free(node);
}

int lnkd_list_free(LinkedList *list){
	if (!list){
		printerr_null_param(lnkd_list_free);
		return NULL_PARAMETER_ERROR;
	}
	lnkd_list_free_node(list->head);
	free(list);
	return SUCCESS;
}

LinkedList* lnkd_list_reset(LinkedList *list){
	if (!list){
		printerr_null_param(lnkd_list_reset);
		return NULL;
	}
	lnkd_list_free_node(list->head);
	list->head = NULL;
	list->tail = NULL;
	list->n_elements = 0;
	return list;
}
