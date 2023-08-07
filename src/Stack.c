/**
 * Stack.c
 * Implementation of the Stack.
 *
 *  Copyright (C) 2023 - Saúl Valdelvira \n
 *  License: BSD 3-Clause \n
 *  Email: saul@saulv.es
 */
#include "Stack.h"
#include <stdlib.h>
#include <stdio.h>
#define GDS_ENABLE_ERROR_MACROS
#include "./util/error.h"
#include "./util/definitions.h"
#include <string.h>
#include <stdarg.h>
#include <assert.h>

/// @cond
typedef struct StackNode {
	struct StackNode *next;
	byte info[];
} StackNode;
/// @endcond

/**
 * Stack struct
 * @headerfile Stack.h <GDS/Stack.h>
 * @see Stack.h
*/
struct Stack {
	StackNode *head;			///< Head node
	comparator_function_t compare;		///< Comparator function pointer
	destructor_function_t destructor;	///< Destructor function pointer
	size_t data_size;			///< Size (in bytes) of the data type being stored
	size_t n_elements;			///< Number of elements in the stack
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
	assert(stack);
	// Initialize stack
	stack->head = NULL;
	stack->compare = cmp;
	stack->data_size = data_size;
	stack->n_elements = 0;
	stack->destructor = NULL;
	return stack;
}

void stack_set_comparator(Stack *stack, comparator_function_t cmp){
	if (!stack || !cmp)
		printerr_null_param();
	else
		stack->compare = cmp;
}

void stack_set_destructor(Stack *stack, destructor_function_t destructor){
	if (!stack)
		printerr_null_param();
	else
		stack->destructor = destructor;
}

static StackNode* init_node(void *element, size_t size){
	StackNode *node = malloc(offsetof(StackNode, info) + size);
	assert(node);
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
		if (status != SUCCESS)
			return status;
		array = void_offset(array, stack->data_size);
	}
	return SUCCESS;
}

void* stack_pop(Stack *stack, void *dest){
	if(!stack || !dest){
		printerr_null_param();
		return NULL;
	}
	if(!stack->head)
		return NULL;
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
		if (!stack_pop(stack, array))
			return i;
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
	if(stack->head == NULL)
		return NULL;
	else
		return memcpy(dest, stack->head->info, stack->data_size);
}

bool stack_exists(Stack *stack, void *element){
	if(!stack || !element){
		printerr_null_param();
		return false;
	}
	StackNode *aux = stack->head;
	while (aux != NULL){
		if((*stack->compare) (element, aux) == 0)
			return true;
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
	while (*aux != NULL && stack->compare((*aux)->info, element) != 0)
		aux = &(*aux)->next;
	if (!*aux)
		return ELEMENT_NOT_FOUND_ERROR;
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

static void free_node(StackNode *node, destructor_function_t destructor){
	if (node == NULL)
		return;
	if (destructor)
		destructor(node->info);
	free_node(node->next, destructor);
	free(node);
}

int stack_free(Stack *stack){
	if (!stack){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	free_node(stack->head, stack->destructor);
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
	free_node(stack->head, stack->destructor);
	stack->head = NULL;
	return stack;
}
