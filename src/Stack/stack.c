/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 08-01-2023
 * Copyright: Saúl Valdelvira Iglesias (2023)
 * Licensed under the GNU GPL V.3. See /LICENSE file for more info
*/
#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include "../Util/checks.h"
#include <memory.h>

struct StackNode {
        struct StackNode *next;
        unsigned char info[];
};

Stack stack_init(size_t data_size, int (*cmp) (const void*, const void*)){
        return (Stack){
                .head = NULL,
                .compare = cmp,
                .data_size = data_size
        };
}

/**
 * Initializes a new StackNode with the given info
*/
static StackNode* init_node(void *element, size_t size){
        StackNode *node = malloc(offsetof(StackNode, info) + size);

        if(!node || !memcpy(node->info, element, size)){
                fprintf(stderr, "ERROR: Could not initilize node\n");
                return NULL;
        }

        node->next = NULL;
        return node;
}

int stack_push(Stack *stack, void *element){
        CHECK_NULL(stack, stack_push, NULL_PARAMETER)
        if(stack->head == NULL){
                stack->head = init_node(element, stack->data_size);
                CHECK_MEMORY(stack->head, stack_push, ALLOCATION_ERROR)
        }else{ // Push an element to the head
                StackNode *aux = init_node(element, stack->data_size);
                CHECK_MEMORY(aux, stack_push, ALLOCATION_ERROR)

                aux->next = stack->head;
                stack->head = aux;
        }
        return 1;
}

void* stack_pop(Stack *stack, void *dest){
        CHECK_NULL(stack, stack_pop, NULL)
        if(stack->head == NULL){
                return NULL;
        }else{
                StackNode* aux = stack->head;    // Save the head
                stack->head = stack->head->next; // Change it to the next element
                if(!memcpy(dest, aux->info, stack->data_size)){       // Save the element
                        fprintf(stderr, "ERROR: Could not pop element\n");
                        return NULL;
                }
                free(aux);                       // Free the old head
                return dest;                  // Return the element
        }
}

void* stack_peek(Stack stack, void *dest){
        if(stack.head->info == NULL){
                return NULL;
        }else{
                return memcpy(dest, stack.head->info, stack.data_size);
        }
}

bool stack_search(Stack stack, void *element){
        StackNode *aux = stack.head;
        while (aux != NULL){
                if((*stack.compare) (element, aux) == 0){
                        return true;
                }
                aux = aux->next;
        } 
        return false;
}

bool stack_isempty(Stack stack){
        return stack.head == NULL;
}

static void free_node(StackNode *node){
        if (node == NULL) {
                return;
        }
        free_node(node->next);
        free(node);
}

void stack_free(Stack stack){
        free_node(stack.head);

}

void stack_reset(Stack *stack){
        free_node(stack->head);
        stack->head = NULL;
}