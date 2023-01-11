/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 11-01-2023
 * Copyright (C) 2022-2023 Saúl Valdelvira
 * Licensed under the GNU GPL V.3. See /LICENSE file for more info
*/
#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include "../Util/error.h"
#include "../Util/definitions.h"
#include <memory.h>

typedef struct StackNode {
	struct StackNode *next;
	byte_t info[];
} StackNode;

struct _Stack {
	StackNode *head;
	// Comparator function
	comparator_function_t compare;
	size_t data_size;
};

Stack* stack_init(size_t data_size, comparator_function_t cmp){
	if (data_size <= 0){
		printerr_data_size(stack_init);
		return NULL;
	}
	if (!cmp){
		printerr_null_param(stack_init);
		return NULL;
	}
	// Allocate stack
	Stack *stack = malloc(sizeof(*stack));
	if (!stack){
		printerr_allocation(stack_init);
		return NULL;
	}
	// Initialize stack
	stack->head = NULL;
	stack->compare = cmp;
	stack->data_size = data_size;
	return stack;
}

/**
 * Initializes a new StackNode with the given info
*/
static StackNode* init_node(void *element, size_t size){
	StackNode *node = malloc(offsetof(StackNode, info) + size);
	if (!node){
		printerr_allocation(init_node);
		return NULL;
	}
	if (!memcpy(node->info, element, size)){
		printerr_memory_op(init_node);
		return NULL;
	}

	node->next = NULL;
	return node;
}

int stack_push(Stack *stack, void *element){
	if(!stack || !element){
		printerr_null_param(stack_push);
		return NULL_PARAMETER_ERROR;
	}
	if(stack->head == NULL){
		stack->head = init_node(element, stack->data_size);
		if (!stack->head){
			printerr_allocation(stack_push);
			return ALLOCATION_ERROR;
		}
	}else{ // Push an element to the head
		StackNode *aux = init_node(element, stack->data_size);
		if (!aux){
			printerr_allocation(stack_push);
			return ALLOCATION_ERROR;
		}
		aux->next = stack->head;
		stack->head = aux;
	}
	return SUCCESS;
}

void* stack_pop(Stack *stack, void *dest){
	if(!stack || !dest){
		printerr_null_param(stack_pop);
		return NULL;
	}
	if(stack->head != NULL){
		StackNode* aux = stack->head;    // Save the head
		stack->head = stack->head->next; // Change it to the next element
		dest = memcpy(dest, aux->info, stack->data_size);
		if(!dest){       // Save the element
			printerr_memory_op(stack_pop);
			return NULL;
		}
		free(aux);                       // Free the old head
		return dest;  
	}
	return NULL;
}

void* stack_peek(Stack *stack, void *dest){
	if(!stack || !dest){
		printerr_null_param(stack_peek);
		return NULL;
	}
	if(stack->head == NULL){
		return NULL;
	}else{
		return memcpy(dest, stack->head->info, stack->data_size);
	}
}

bool stack_search(Stack *stack, void *element){
	if(!stack || !element){
		printerr_null_param(stack_search);
		return false;
	}
	StackNode *aux = stack->head;
	while (aux != NULL){
		if((*stack->compare) (element, aux) == 0){
			return true;
		}
		aux = aux->next;
	} 
	return false;
}

bool stack_isempty(Stack *stack){
	if (!stack){
		printerr_null_param(stack_isempty);
		return false;
	}
	return stack->head == NULL;
}

static void free_node(StackNode *node){
	if (node == NULL) {
		return;
	}
	free_node(node->next);
	free(node);
}

int stack_free(Stack *stack){
	if (!stack){
		printerr_null_param(stack_free);
		return NULL_PARAMETER_ERROR;
	}
	free_node(stack->head);
	free(stack);
	return 1;
}

Stack* stack_reset(Stack *stack){
	if (!stack){
		printerr_null_param(stack_reset);
		return NULL;
	}
	free_node(stack->head);
	stack->head = NULL;
	return stack;
}