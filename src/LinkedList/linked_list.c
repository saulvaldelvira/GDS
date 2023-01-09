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
#include "../Util/checks.h"
#include <memory.h>

struct LLNode {
    struct LLNode *next; // Pointer to the next node
    struct LLNode *previous; // Pointer to the previous node
    unsigned char info[];
};

LinkedList lnkd_list_init(size_t data_size, int (*cmp) (const void*, const void*)){
    return (LinkedList) {
        .n_elements = 0,
        .head = NULL,
        .tail = NULL,
        .compare = cmp,
        .data_size = data_size
    };
}

/**
 * Initializes a new LLNode with the given info
*/
static LLNode* lnkd_list_innit_node(void *info, size_t size){
    LLNode *node = malloc(offsetof(LLNode, info) + size);
    CHECK_MEMORY(node, lnkd_list_innit_node , NULL)
    node->next = NULL;
    node->previous = NULL;
    if(!memcpy(node->info, info, size)){
        fprintf(stderr, "ERROR: couldn't allocate node\n");
    }
    return node;
}

int lnkd_list_push_back(LinkedList *list, void *element){
    CHECK_NULL(list, lnkd_list_push_back, NULL_PARAMETER)
    CHECK_NULL(element, lnkd_list_push_back, NULL_PARAMETER)
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
    return 1;
}

int lnkd_list_push_front(LinkedList *list, void *element){
    CHECK_NULL(list, lnkd_list_push_front, NULL_PARAMETER)
    CHECK_NULL(list, lnkd_list_push_front, NULL_PARAMETER)
    if(list->n_elements == 0){ // We add to the head
        list->head = lnkd_list_innit_node(element, list->data_size);
        if(!list->head){
            return ALLOCATION_ERROR;
        }
        list->tail = list->head;
    }else{ // We add to the head
        LLNode* aux = lnkd_list_innit_node(element, list->data_size);
        CHECK_MEMORY(aux, lnkd_list_push_front, ALLOCATION_ERROR)
        aux->next = list->head;
        list->head->previous = aux;
        list->head = aux;
    }
    list->n_elements++;
    return 1;
}

int lnkd_list_set(LinkedList *list, void *element, void *replacement){
    CHECK_NULL(list, lnkd_list_set, NULL_PARAMETER)
    CHECK_NULL(element, lnkd_list_set, NULL_PARAMETER)
    CHECK_NULL(replacement, lnkd_list_set, NULL_PARAMETER)
    LLNode *aux = list->head;
    while ( (*list->compare) (aux->info, element) != 0) {
        aux = aux->next;
        if(aux == NULL){
            return -1;
        }
    }
    if(!memcpy(aux->info, replacement, list->data_size)){
        fprintf(stderr, "ERROR: lnkd_list_set\n");
        return ALLOCATION_ERROR;
    }
    return 1;
}

void* lnkd_list_get(LinkedList list, void *element, void *dest){
    CHECK_NULL(element, lnkd_list_get, NULL)
    CHECK_NULL(dest, lnkd_list_get, NULL)
    LLNode *aux = list.head;
    while (aux != NULL && (*list.compare) (aux->info, element) != 0) {
        aux = aux->next;
    }
    return aux == NULL ? NULL : memcpy(dest, aux->info, list.data_size);
}

bool lnkd_list_exists(LinkedList list, void *element){
    CHECK_NULL(element, lnkd_list_exists, false)
    LLNode *aux = list.head;
    while (aux != NULL) {
        if ((*list.compare) (aux->info, element) == 0){
            return true;
        }
        aux = aux->next;
    }
    return false;
}

bool lnkd_list_isempty(LinkedList list){
    return list.n_elements == 0;
}

int lnkd_list_remove(LinkedList *list, void *element){
    CHECK_NULL(list, lnkd_list_remove, NULL_PARAMETER)
    CHECK_NULL(element, lnkd_list_remove, NULL_PARAMETER)
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
        return 1;
    }
    return -1;
}

static void lnkd_list_free_node(LLNode *node){
    if(node == NULL) return;
    lnkd_list_free_node(node->next);
    free(node);
}

void lnkd_list_free(LinkedList list){
    lnkd_list_free_node(list.head);
}

int lnkd_list_reset(LinkedList *list){
    CHECK_NULL(list, lnkd_list_reset, NULL_PARAMETER)
    lnkd_list_free_node(list->head);
    list->head = NULL;
    list->tail = NULL;
    list->n_elements = 0;
    return 1;
}
