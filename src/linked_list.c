/*
 * linked_list.c - Doubly Linked List implementation.
 * Author: Saúl Valdelvira (2023)
 */
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include "error.h"
#include "definitions.h"
#define LINKED_LIST_IMPL
#include "linked_list.h"

struct linked_list_t {
        LLNode *head;   ///< Head node
        LLNode *tail;   ///< Tail node. Last added node
        size_t n_elements;      ///< Number of elements in the list
        size_t data_size;       ///< Size (in bytes) of the data type
        comparator_function_t compare;          ///< Comparator function pointer
        destructor_function_t destructor;       ///< Destructor function pointer
};

/// INITIALIZE ////////////////////////////////////////////////////////////////

linked_list_t* list_init(size_t data_size, comparator_function_t cmp){
        assert(cmp && data_size > 0);
        linked_list_t *list = malloc(sizeof(*list));
        if (!list) return NULL;
        list->n_elements = 0;
        list->head = NULL;
        list->tail = NULL;
        list->compare = cmp;
        list->destructor = NULL;
        list->data_size = data_size;
        return list;
}

void list_set_comparator(linked_list_t *list, comparator_function_t cmp){
        if (list && cmp)
                list->compare = cmp;
}

void list_set_destructor(linked_list_t *list, destructor_function_t destructor){
        if (list)
                list->destructor = destructor;
}

/**
 * Initializes a new LLNode with the given info
*/
static LLNode* list_init_node(void *info, size_t size){
        LLNode *node = malloc(offsetof(LLNode, info) + size);
        if (!node) return NULL;
        node->next = NULL;
        node->prev = NULL;
        memcpy(node->info, info, size);
        return node;
}

///////////////////////////////////////////////////////////////////////////////

/// ADD-SET ///////////////////////////////////////////////////////////////////////

int list_append(linked_list_t *list, void *element){
        assert(list && element);
        LLNode **ref = list->head == NULL ? // If head NULL
                        &list->head           : // Add to head
                        &list->tail->next;  // Else add to the tail
        *ref = list_init_node(element, list->data_size);
        if (!*ref) return ERROR;
        (*ref)->prev = list->tail;
        list->tail = *ref;
        list->n_elements++;
        return SUCCESS;
}

int list_append_array(linked_list_t *list, void *array, size_t array_length){
        assert(list && array);
        while (array_length-- > 0){
                int status = list_append(list, array);
                if (status != SUCCESS)
                        return status;
                array = void_offset(array, list->data_size);
        }
        return SUCCESS;
}

int list_push_front(linked_list_t *list, void *element){
        assert(list && element);
        LLNode *old_head = list->head;
        list->head = list_init_node(element, list->data_size);
        if (!list->head) return ERROR;
        list->head->next = old_head;
        if (old_head)
                old_head->prev = list->head;
        list->n_elements++;
        return SUCCESS;
}

int list_push_front_array(linked_list_t *list, void *array, size_t array_length){
        assert(list && array);
        void *tmp = array;
        while (array_length-- > 0){
                int status;
                status = list_push_front(list, tmp);
                if (status != SUCCESS)
                        return status;
                tmp = void_offset(tmp, list->data_size);
        }
        return SUCCESS;
}

int list_set(linked_list_t *list, void *element, void *replacement){
        assert(list && element && replacement);
        LLNode *aux = list->head;
        while (list->compare(aux->info, element) != 0){
                aux = aux->next;
                if(aux == NULL)
                        return ELEMENT_NOT_FOUND_ERROR;
        }
        if (list->destructor)
                list->destructor(aux->info);
        memcpy(aux->info, replacement, list->data_size);
        return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

/// GET ///////////////////////////////////////////////////////////////////////

void* list_get(linked_list_t *list, void *element, void *dest){
        assert(list && element && dest);
        LLNode *aux = list->head;
        while (aux != NULL){
                if (list->compare(aux->info, element) == 0)
                        return memcpy(dest, aux->info, list->data_size);
                aux = aux->next;
        }
        return NULL;
}

void* list_get_front(linked_list_t *list, void *dest){
        assert(list && dest);
        if (list->head == NULL)
                return NULL;
        return memcpy(dest, list->head->info, list->data_size);
}

void* list_get_back(linked_list_t *list, void *dest){
        assert(list && dest);
        if (list->tail == NULL)
                return NULL;
        return memcpy(dest, list->tail->info, list->data_size);
}

void* list_get_into_array(linked_list_t *list, void *array, size_t array_length){
        assert(list && array);
        if (array_length > list->n_elements)
                array_length = list->n_elements;
        LLNode *aux = list->head;
        void *dst = array;
        while (array_length-- > 0){
                memcpy(dst, aux->info, list->data_size);
                aux = aux->next;
                dst = void_offset(dst, list->data_size);
        }
        return array;

}

void* list_get_array(linked_list_t *list, size_t array_length){
        assert(list);
        if (array_length == 0 || array_length > list->n_elements)
                array_length = list->n_elements;
        void *array = malloc(list->data_size * array_length);
        assert(array);
        if (!list_get_into_array(list, array, array_length)){
                free(array);
                return NULL;
        }
        return array;
}

///////////////////////////////////////////////////////////////////////////////

/// REMOVE ////////////////////////////////////////////////////////////////////

int list_remove(linked_list_t *list, void *element){
        assert(list && element);
        LLNode *tmp = list->head;
        while(tmp){
                if (list->compare(tmp->info, element) == 0){
                        if (list->tail == tmp)
                                list->tail = tmp->prev;
                        else
                                tmp->next->prev = tmp->prev;

                        if (list->head == tmp)
                                list->head = tmp->next;
                        else
                                tmp->prev->next = tmp->next;

                        if (list->destructor)
                                list->destructor(tmp->info);
                        free(tmp);
                        list->n_elements--;
                        return SUCCESS;
                }
                tmp = tmp->next;
        }
        return ELEMENT_NOT_FOUND_ERROR;
}

int list_remove_front(linked_list_t *list){
        assert(list);
        if (list->head == NULL)
                return SUCCESS;
        LLNode *del = list->head;
        if (list->tail == list->head)
                list->tail = NULL;
        list->head = list->head->next;
        if (list->destructor)
                list->destructor(del->info);
        free(del);
        list->n_elements--;
        return SUCCESS;
}

int list_remove_back(linked_list_t *list){
        assert(list);
        if (list->tail == NULL)
                return SUCCESS;
        LLNode *del = list->tail;
        if (list->tail == list->head)
                list->head = NULL;
        list->tail = list->tail->prev;
        if (list->destructor)
                list->destructor(del->info);
        free(del);
        list->n_elements--;
        return SUCCESS;
}

int list_remove_array(linked_list_t *list, void *array, size_t array_length){
        assert(list && array);
        while (array_length-- > 0){
                list_remove(list, array);
                array = void_offset(array, list->data_size);
        }
        return SUCCESS;
}

void* list_pop(linked_list_t *list, void *element, void *dest){
        assert(list && element);
        LLNode *tmp = list->head;
        while(tmp){
                if (list->compare(tmp->info, element) == 0){
                        if (list->tail == tmp)
                                list->tail = tmp->prev;
                        else
                                tmp->next->prev = tmp->prev;

                        if (list->head == tmp)
                                list->head = tmp->next;
                        else
                                tmp->prev->next = tmp->next;

                        if (dest)
                                memcpy(dest, tmp->info, list->data_size);
                        free(tmp);
                        list->n_elements--;
                        return dest;
                }
                tmp = tmp->next;
        }
        return NULL;
}

void* list_pop_front(linked_list_t *list, void *dest){
        assert(list);
        if (list->head == NULL)
                return NULL;
        LLNode *del = list->head;
        if (list->tail == list->head)
                list->tail = NULL;
        list->head = list->head->next;
        if (dest)
                memcpy(dest, del->info, list->data_size);
        free(del);
        list->n_elements--;
        return dest;
}

void* list_pop_back(linked_list_t *list, void *dest){
        assert(list);
        if (list->tail == NULL)
                return NULL;
        LLNode *del = list->tail;
        if (list->tail == list->head)
                list->head = NULL;
        list->tail = list->tail->prev;
        if (dest)
                memcpy(dest, del->info, list->data_size);
        free(del);
        list->n_elements--;
        return dest;
}

void* list_pop_array(linked_list_t *list, void *array, size_t array_length, void *dest){
        assert(list && array);
        while (array_length-- > 0){
                list_pop(list, array, dest);
                array = void_offset(array, list->data_size);
                if (dest)
                        dest = void_offset(dest, list->data_size);
        }
        return dest;
}

///////////////////////////////////////////////////////////////////////////////

/// OTHER /////////////////////////////////////////////////////////////////////

bool list_exists(linked_list_t *list, void *element){
        if (!list || !element)
                return false;
        LLNode *aux = list->head;
        while (aux != NULL){
                if (list->compare(aux->info, element) == 0)
                        return true;
                aux = aux->next;
        }
        return false;
}

size_t list_size(linked_list_t *list){
        return list ? list->n_elements : 0;
}

bool list_isempty(linked_list_t *list){
        return list ? list->n_elements == 0 : true;
}

linked_list_t* list_join(linked_list_t *list_1, linked_list_t *list_2){
        assert(list_1 && list_2);
        if (list_1->data_size != list_2->data_size)
                return NULL;
        linked_list_t *list_joint = list_init(list_1->data_size, list_1->compare);
        LLNode *aux = list_1->head;
        while (aux){
                list_append(list_joint, aux->info);
                aux = aux->next;
        }
        aux = list_2->head;
        while (aux){
                list_append(list_joint, aux->info);
                aux = aux->next;
        }
        return list_joint;
}

///////////////////////////////////////////////////////////////////////////////

/// FREE //////////////////////////////////////////////////////////////////////

static void list_free_node(LLNode *node, destructor_function_t destructor){
        if (node == NULL)
                return;
        if (destructor)
                destructor(node->info);
        list_free_node(node->next, destructor);
        free(node);
}

static void _list_free(linked_list_t *list){
        if (list){
                list_free_node(list->head, list->destructor);
                free(list);
        }
}

void (list_free)(linked_list_t *l, ...){
        if (!l)
                return;
        va_list arg;
        va_start(arg, l);
        do {
                _list_free(l);
                l = va_arg(arg, linked_list_t*);
        } while (l);
        va_end(arg);
}

void list_clear(linked_list_t *list){
        if (!list)
                return;
        list_free_node(list->head, list->destructor);
        list->head = NULL;
        list->tail = NULL;
        list->n_elements = 0;
}

/* ITERATOR  */

inline list_iterator_t list_iterator(linked_list_t *list) {
        assert(list);
        return (list_iterator_t){
                .next = list->head,
                .data_size = list->data_size,
        };
}

inline list_iterator_t list_iterator_from_back(linked_list_t *list) {
        assert(list);
        return (list_iterator_t){
                .prev = list->tail,
                .data_size = list->data_size,
        };
}

void* list_it_next(list_iterator_t *it, void *dst) {
        assert(it && dst);
        if (it->next) {
                it->prev = it->next;
                it->next = it->next->next;
                return memcpy(dst, it->prev->info, it->data_size);
        }
        return NULL;
}

void* list_it_prev(list_iterator_t *it, void *dst) {
        assert(it && dst);
        if (it->prev) {
                it->next = it->prev;
                it->prev = it->prev->prev;
                return memcpy(dst, it->next->info, it->data_size);
        }
        return NULL;
}

inline void* list_it_peek_next(list_iterator_t *it, void *dst) {
        assert(it && dst);
        return it->next ?
                memcpy(dst, it->next->info, it->data_size) : NULL;
}

inline void* list_it_peek_prev(list_iterator_t *it, void *dst) {
        assert(it && dst);
        return it->prev ?
                memcpy(dst, it->prev->info, it->data_size) : NULL;
}

inline bool list_it_has_next(list_iterator_t *it) {
        assert(it);
        return it->next;
}

inline bool list_it_has_prev(list_iterator_t *it) {
        assert(it);
        return it->prev;
}

