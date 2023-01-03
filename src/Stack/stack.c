#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include "../Util/checks.h"

Stack stack_init(bool (*cmp) (void*, void*)){
        return (Stack){
                .head = NULL,
                .compare = cmp,
                .free_on_delete = DONT_FREE_ON_DELETE
        };
}

void stack_configure(Stack *stack, int free_on_delete){
        stack->free_on_delete = free_on_delete;
}

/**
 * Initializes a new StackNode with the given info
*/
static StackNode* init_node(void *element){
        StackNode *node = malloc(sizeof(StackNode));
        if(!node){
                return NULL;
        }
        node->info = element;
        node->next = NULL;
}

int stack_push(Stack *stack, void *element){
        CHECK_NULL(stack == NULL || element == NULL, stack_push)
        if(stack->head == NULL){
                stack->head = init_node(element);
                CHECK_MEMORY(stack->head, stack_push, ALLOCATION_ERROR)
        }else{
                StackNode *aux = init_node(element);
                CHECK_MEMORY(aux, stack_push, ALLOCATION_ERROR)
                aux->info = element;
                aux->next = stack->head;
                stack->head = aux;
        }
        return 1;
}

void* stack_pop(Stack *stack){
        CHECK_NULL(stack == NULL, stack_pop)
        if(stack->head == NULL){
                return NULL;
        }else{
                void *result = stack->head->info;
                stack->head = stack->head->next;
                return result;
        }
}

void* stack_peek(Stack stack){
        if(stack.head->info == NULL){
                return NULL;
        }else{
                return stack.head->info;
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

static void free_node(StackNode *node, int free_element){
        if (node == NULL) {
                return;
        }
        if (free_element) {
                free(node->info);
        }
        free_node(node->next, free_element);
        free(node);
}

void stack_free(Stack stack){
        free_node(stack.head, stack.free_on_delete);

}

void stack_reset(Stack *stack){
        free_node(stack->head, stack->free_on_delete);
        stack->head = NULL;
}