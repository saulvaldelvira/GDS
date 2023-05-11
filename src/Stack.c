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
#include "Stack.h"
#include <stdlib.h>
#include <stdio.h>
#include "./Util/error.h"
#include "./Util/definitions.h"
#include <string.h>
#include <stdarg.h>

typedef struct StackNode {
	struct StackNode *next;
	byte_t info[];
} StackNode;

struct Stack {
	StackNode *head;
	// Comparator function
	comparator_function_t compare;
	size_t data_size;
	size_t n_elements;
};

/// INITIALIZE ////////////////////////////////////////////////////////////////

Stack* stack_init(size_t data_size, comparator_function_t cmp){
	if (!cmp){
		printerr_null_param();
		return NULL;
	}
	if (data_size == 0){
		printerr_data_size();
		return NULL;
	}
	// Allocate stack
	Stack *stack = malloc(sizeof(*stack));
	if (!stack){
		printerr_allocation();
		return NULL;
	}
	// Initialize stack
	stack->head = NULL;
	stack->compare = cmp;
	stack->data_size = data_size;
	stack->n_elements = 0;
	return stack;
}

void stack_configure(Stack *stack, comparator_function_t cmp){
	if (!stack || !cmp){
		printerr_null_param();
		return;
	}
	stack->compare = cmp;
}

static StackNode* init_node(void *element, size_t size){
	StackNode *node = malloc(offsetof(StackNode, info) + size);
	if (!node){
		printerr_allocation();
		return NULL;
	}
	memcpy(node->info, element, size);
	node->next = NULL;
	return node;
}

///////////////////////////////////////////////////////////////////////////////

/// PUSH-POP //////////////////////////////////////////////////////////////////////

int stack_push(Stack *stack, void *element){
	if(!stack || !element){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	StackNode *old_head = stack->head;
	stack->head = init_node(element, stack->data_size);
	if (!stack->head){
		printerr_allocation();
		return ALLOCATION_ERROR;
	}
	stack->head->next = old_head;
	stack->n_elements++;
	return SUCCESS;
}

int stack_push_array(Stack *stack, void *array, size_t array_length){
	if (!stack || !array){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	int status;
	while (array_length-- > 0){
		status = stack_push(stack, array);
		if (status != SUCCESS){
			return status;
		}
		array = void_offset(array, stack->data_size);
	}
	return SUCCESS;
}

void* stack_pop(Stack *stack, void *dest){
	if(!stack || !dest){
		printerr_null_param();
		return NULL;
	}
	if(!stack->head){
		return NULL;
	}
	StackNode* aux = stack->head;
	stack->head = stack->head->next;
	memcpy(dest, aux->info, stack->data_size);
	free(aux);
	stack->n_elements--;
	return dest;
}

size_t stack_pop_array(Stack *stack, void *array, size_t array_length){
	if(!stack || !array){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	for (size_t i = 0; i < array_length; i++){
		// When the stack is empty, return
		if (!stack_pop(stack, array)){
			return i;
		}
		array = void_offset(array, stack->data_size);
	}
	return array_length;
}

///////////////////////////////////////////////////////////////////////////////

/// PEEK //////////////////////////////////////////////////////////////////////

void* stack_peek(Stack *stack, void *dest){
	if(!stack || !dest){
		printerr_null_param();
		return NULL;
	}
	if(stack->head == NULL){
		return NULL;
	}else{
		return memcpy(dest, stack->head->info, stack->data_size);
	}
}

bool stack_exists(Stack *stack, void *element){
	if(!stack || !element){
		printerr_null_param();
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

int stack_remove(Stack *stack, void *element){
	if(!stack || !element){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	StackNode** aux = &stack->head;
	while (*aux != NULL && stack->compare((*aux)->info, element) != 0){
		aux = &(*aux)->next;
	}
	if (!*aux){
		return ELEMENT_NOT_FOUND_ERROR;
	}
	StackNode *del = *aux;
	*aux = (*aux)->next;
	free(del);
	stack->n_elements--;
	return SUCCESS;
}

size_t stack_size(Stack *queue){
	if (!queue){
		printerr_null_param();
		return 0;
	}
	return queue->n_elements;
}

bool stack_isempty(Stack *stack){
	if (!stack){
		printerr_null_param();
		return false;
	}
	return stack->head == NULL;
}

///////////////////////////////////////////////////////////////////////////////

/// FREE //////////////////////////////////////////////////////////////////////

static void free_node(StackNode *node){
	if (node == NULL){
		return;
	}
	free_node(node->next);
	free(node);
}

int stack_free(Stack *stack){
	if (!stack){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	free_node(stack->head);
	free(stack);
	return SUCCESS;
}

void stack_free_all(unsigned int n, ...){
	va_list arg;
	va_start(arg, n);
	for (unsigned int i = 0; i < n; i++){
		Stack *ptr = va_arg(arg, Stack*);
		stack_free(ptr);
	}
	va_end(arg);
}

Stack* stack_reset(Stack *stack){
	if (!stack){
		printerr_null_param();
		return NULL;
	}
	free_node(stack->head);
	stack->head = NULL;
	return stack;
}
