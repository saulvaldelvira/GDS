#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../Util/checks.h"

LinkedList lnkd_list_init(bool (*cmp) (void*, void*)){
    return (LinkedList) {
        .n_elements = 0,
        .head = NULL,
        .tail = NULL,
        .compare = cmp,
        .free_on_delete = DONT_FREE_ON_DELETE
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

void lnkd_list_configure(LinkedList *list, bool free_on_delete){
    list->free_on_delete = free_on_delete;
}

int lnkd_list_append(LinkedList *list, void *element){
    CHECK_NULL(element == NULL, lnkd_list_append)
    if(list->n_elements == 0){
        list->head = lnkd_list_innit_node(element);
        CHECK_NULL(list->head == NULL, lnkd_list_append)
        list->tail = list->head;
    }else{
        list->tail->next = lnkd_list_innit_node(element);
        CHECK_NULL(list->tail->next == NULL, lnkd_list_append)
        list->tail->next->previous = list->tail;
        list->tail = list->tail->next;
    }
    list->n_elements++;
    return 1;
}

int lnkd_list_set(LinkedList *list, void *element, void *replacement){
    LLNode *aux = list->head;
    while ( (*list->compare) (aux->info, element) != 0) {
        aux = aux->next;
        if(aux == NULL){
            return 0;
        }
    }
    aux->info = replacement;
    return 1;
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

int lnkd_list_remove(LinkedList *list, void *element){
    CHECK_NULL(list == NULL || element == NULL, lnkd_list_remove)
    LLNode *aux = list->head;
    while(aux != NULL && (*list->compare) (aux->info, element) != 0){
        aux = aux->next;
    }
    if(aux != NULL){
        if (aux == list->head){ // If the element to delete is the head
            list->head = list->head->next;
        }else{
            if (aux->next != NULL){// We're in the last node, have to change the tail pointer
                list->tail = aux->previous;
            }
            aux->previous->next = aux->next;
            aux->next->previous = aux;
        }
        if (list->free_on_delete == FREE_ON_DELETE){
            free(aux->info);
        }
        free(aux);
        list->n_elements--;
        return 1;
    }
    return 0;
}

static void lnkd_list_free_node(LLNode *node, int free_elements){
    if(node == NULL) return;
    lnkd_list_free_node(node->next, free_elements);
    if(free_elements == FREE_ON_DELETE){
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
