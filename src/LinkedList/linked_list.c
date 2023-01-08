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

struct LLNode {
    struct LLNode *next; // Pointer to the next node
    struct LLNode *previous; // Pointer to the previous node
    void *info; // Element stored in this node
};

LinkedList lnkd_list_init(int (*cmp) (const void*, const void*)){
    return (LinkedList) {
        .n_elements = 0,
        .head = NULL,
        .tail = NULL,
        .compare = cmp,
        .free_on_delete = DontFreeOnDelete
    };
}

/**
 * Initializes a new LLNode with the given info
*/
static LLNode* lnkd_list_innit_node(void *info){
    LLNode *node = calloc(1, sizeof(LLNode));
    CHECK_MEMORY(node, lnkd_list_append , NULL)
    node->info = info;
    node->next = NULL;
    node->previous = NULL;
    return node;
}

int lnkd_list_push_back(LinkedList *list, void *element){
    CHECK_NULL(element == NULL, lnkd_list_append, NULL_PARAMETER)
    if(list->n_elements == 0){ // We add to the head
        list->head = lnkd_list_innit_node(element);
        if(!list->head){
            return ALLOCATION_ERROR;
        }
        list->tail = list->head;
    }else{ // We add to the tail
        list->tail->next = lnkd_list_innit_node(element);
        if(!list->tail){
            return ALLOCATION_ERROR;
        }
        list->tail->next->previous = list->tail; // Create the "previous" reference in this new element
        list->tail = list->tail->next; // Update the tail to this new element
    }
    list->n_elements++;
    return 1;
}

int lnkd_list_push_front(LinkedList *list, void *element){
    CHECK_NULL(element == NULL, lnkd_list_append, NULL_PARAMETER)
    if(list->n_elements == 0){ // We add to the head
        list->head = lnkd_list_innit_node(element);
        if(!list->head){
            return ALLOCATION_ERROR;
        }
        list->tail = list->head;
    }else{ // We add to the head
        LLNode* aux = lnkd_list_innit_node(element);
        CHECK_MEMORY(aux, lnkd_list_push_front, ALLOCATION_ERROR)
        aux->next = list->head;
        list->head->previous = aux;
        list->head = aux;
    }
    list->n_elements++;
    return 1;
}

bool lnkd_list_set(LinkedList *list, void *element, void *replacement){
    LLNode *aux = list->head;
    while ( (*list->compare) (aux->info, element) != 0) {
        aux = aux->next;
        if(aux == NULL){
            return false;
        }
    }
    aux->info = replacement;
    return true;
}

void* lnkd_list_get(LinkedList list, void *element){
    LLNode *aux = list.head;
    while (aux != NULL && (*list.compare) (aux->info, element) != 0) {
        aux = aux->next;
    }
    return aux != NULL ? aux->info : NULL;
}

bool lnkd_list_exists(LinkedList list, void *element){
    return lnkd_list_get(list, element) != NULL;
}

bool lnkd_list_isempty(LinkedList list){
    return list.n_elements == 0;
}

bool lnkd_list_remove(LinkedList *list, void *element){
    CHECK_NULL(list == NULL || element == NULL, lnkd_list_remove, false)
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
        if (list->free_on_delete == FreeOnDelete){
            free(aux->info);
        }
        free(aux);
        list->n_elements--;
        return true;
    }
    return false;
}

static void lnkd_list_free_node(LLNode *node, free_on_delete_t free_elements){
    if(node == NULL) return;
    lnkd_list_free_node(node->next, free_elements);
    if(free_elements == FreeOnDelete){
        free(node->info);
    }
    free(node);
}

void lnkd_list_free(LinkedList list){
    lnkd_list_free_node(list.head, list.free_on_delete);
}

void lnkd_list_reset(LinkedList *list){
    lnkd_list_free_node(list->head, list->free_on_delete);
    list->head = NULL;
    list->tail = NULL;
    list->n_elements = 0;
}
