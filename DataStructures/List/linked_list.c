#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>

LinkedList linked_list_init(bool (*comp) (void*, void*)){
    return (LinkedList) {
        .n_elements = 0,
        .root = NULL,
        .comp = comp
    };
}

static LLNode* linked_list_innit_node(void *info){
    LLNode *node = calloc(1, sizeof(LLNode));
    node->info = info;
    node->next = NULL;
    return node;
}

void linked_list_append(LinkedList *list, void *element){
    if(element == NULL){
        fprintf(stderr, "Can't append a NULL element\n");
        return;
    }
    if(list->n_elements == 0){
        list->root = linked_list_innit_node(element);
    }else{
        LLNode *aux = list->root;
        while(aux->next != NULL){
            aux = aux->next;
        }
        aux->next = linked_list_innit_node(element);
    }
    list->n_elements++;
}



int linked_list_set(LinkedList *list, void *element, void *replacement){
    LLNode *aux = list->root;
    while ( (*list->comp) (aux->info, element) != 0) {
        aux = aux->next;
        if(aux == NULL){
            return 0;
        }
    }
    aux->info = replacement;
    return 1;
}

void* linked_list_get(LinkedList list, void *element){
    LLNode *aux = list.root;
    while ( (*list.comp) (aux->info, element) != 0) {
        aux = aux->next;
        if(aux == NULL){
            return NULL;
        }  
    }
    return aux->info;
}

bool linked_list_exists(LinkedList list, void *element){
    return linked_list_get(list, element) != NULL;
}

void linked_list_free(LinkedList list);