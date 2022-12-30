#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

LinkedList lnkd_list_init(bool (*comp) (void*, void*)){
    return (LinkedList) {
        .n_elements = 0,
        .root = NULL,
        .comp = comp,
        .free_elements_on_list_remove = DONT_FREE_ELEMENTS_ON_LIST_REMOVE,
        .free_elements_on_node_remove = DONT_FREE_ON_NODE_REMOVE
    };
}

static LLNode* lnkd_list_innit_node(void *info){
    LLNode *node = calloc(1, sizeof(LLNode));
    node->info = info;
    node->next = NULL;
    node->previous = NULL;
    return node;
}

void lnkd_list_configure(LinkedList *list, bool free_on_node_remove, bool free_on_list_remove){
    list->free_elements_on_list_remove = free_on_list_remove;
    list->free_elements_on_node_remove = free_on_node_remove;
}

int lnkd_list_append(LinkedList *list, void *element){
    if(element == NULL){
        fprintf(stderr, "Can't append a NULL element\n");
        return -1;
    }
    if(list->n_elements == 0){
        list->root = lnkd_list_innit_node(element);
        list->root->previous = list->root;
    }else{
        LLNode *aux = list->root->previous; //Last node
        aux->next = lnkd_list_innit_node(element);
        list->root->previous = aux->next;
    }
    list->n_elements++;
    return 1;
}

int lnkd_list_set(LinkedList *list, void *element, void *replacement){
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

void* lnkd_list_get(LinkedList list, void *element){
    LLNode *aux = list.root;
    while (aux != NULL && (*list.comp) (aux->info, element) != 0) {
        aux = aux->next;
    }
    return aux != NULL ? aux->info : NULL;
}

bool lnkd_list_exists(LinkedList list, void *element){
    return lnkd_list_get(list, element) != NULL;
}

int lnkd_list_remove(LinkedList *list, void *element){
    if(list == NULL || element == NULL){
        fprintf(stderr, "Error: The given parameters can't be NULL\n");
        return -1;
    }
    LLNode *aux = list->root;
    while(aux != NULL && (*list->comp) (aux->info, element) != 0){
        aux = aux->next;
    }
    if(aux != NULL){
        if (aux == list->root){ // If the element to delete is the root
            list->root = list->root->next;
            if(list->root != NULL){ // If the root is not null after deleting (the root was not the only element)
                list->root->previous = aux->previous;
            }
        }else{
            if (aux->next != NULL){// We're in the last node, have to change the root "previous" pointer
                list->root->previous = aux->previous;
            }
            aux->previous->next = aux->next;
        }
        if (list->free_elements_on_node_remove == FREE_ON_NODE_REMOVE){
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
    if(free_elements == FREE_ELEMENTS_ON_LIST_REMOVE){
        free(node->info);
    }
    free(node);
}

void lnkd_list_free(LinkedList list){
    lnkd_list_free_node(list.root, list.free_elements_on_list_remove);
}

