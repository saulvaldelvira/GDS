#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

LinkedList linked_list_init(bool (*comp) (void*, void*)){
    return (LinkedList) {
        .n_elements = 0,
        .root = NULL,
        .comp = comp,
        .free_elements_on_list_remove = DONT_FREE_ELEMENTS_ON_LIST_REMOVE,
        .free_elements_on_node_remove = DONT_FREE_ON_NODE_REMOVE
    };
}

static LLNode* linked_list_innit_node(void *info){
    LLNode *node = calloc(1, sizeof(LLNode));
    node->info = info;
    node->next = NULL;
    node->previous = NULL;
    return node;
}

void linked_list_configure(LinkedList *list, bool free_on_node_remove, bool free_on_list_remove){
    list->free_elements_on_list_remove = free_on_list_remove;
    list->free_elements_on_node_remove = free_on_node_remove;
}

int linked_list_append(LinkedList *list, void *element){
    if(element == NULL){
        fprintf(stderr, "Can't append a NULL element\n");
        return -1;
    }
    if(list->n_elements == 0){
        list->root = linked_list_innit_node(element);
        list->root->previous = list->root;
    }else{
        LLNode *aux = list->root->previous; //Last node
        aux->next = linked_list_innit_node(element);
        list->root->previous = aux->next;
    }
    list->n_elements++;
    return 1;
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
    while (aux != NULL && (*list.comp) (aux->info, element) != 0) {
        aux = aux->next;
    }
    return aux != NULL ? aux->info : NULL;
}

bool linked_list_exists(LinkedList list, void *element){
    return linked_list_get(list, element) != NULL;
}

int linked_list_remove(LinkedList *list, void *element){
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

static void linked_list_free_node(LLNode *node, int free_elements){
    if(node == NULL) return;
    linked_list_free_node(node->next, free_elements);
    if(free_elements == FREE_ELEMENTS_ON_LIST_REMOVE){
        free(node->info);
    }
    free(node);
}

void linked_list_free(LinkedList list){
    linked_list_free_node(list.root, list.free_elements_on_list_remove);
}

// COMPARATORS

bool compare_int(void *e_1, void *e_2){
    int n_1 = * (int *) e_1;
    int n_2 = * (int *) e_2;
    if (n_1 > n_2){
        return 1;
    }else if (n_2 > n_1){
        return -1;
    }else{
        return 0;
    }
}

bool compare_char(void *e_1, void *e_2){
    char c_1 = * (char *) e_1;
    char c_2 = * (char *) e_2;
    if (c_1 > c_2){
        return 1;
    }else if (c_2 > c_1){
        return -1;
    }else{
        return 0;
    }
}

bool compare_float(void *e_1, void *e_2){
    float f_1 = * (float *) e_1;
    float f_2 = * (float *) e_2;
    float precision = Comparators.float_precision;
    fabs(f_1 - f_2) <= precision * fmax(f_1, f_2);
    if (f_1 > f_2){
        return 1;
    }else if (f_2 > f_1){
        return -1;
    }else{
        return 0;
    }
}

bool compare_double(void *e_1, void *e_2){
    double d_1 = * (double *) e_1;
    double d_2 = * (double *) e_2;
    if (d_1 > d_2){
        return 1;
    }else if (d_2 > d_1){
        return -1;
    }else{
        return 0;
    }
}