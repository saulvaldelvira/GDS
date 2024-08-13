/*
 * queue.c - queue_t implementation.
 * Author: Saúl Valdelvira (2023)
 */
#include "queue.h"
#include <stdlib.h>
#include "error.h"
#include "definitions.h"
#include <string.h>
#include <stdarg.h>
#include <assert.h>

typedef struct queue_tNode {
        struct queue_tNode *next;
        byte info[];
} queue_tNode;

struct queue_t {
        queue_tNode *head;        ///< Head node
        queue_tNode *tail;        ///< Tail node. Last added node
        size_t data_size;       ///< Size (in bytes) of the data type being stored
        size_t n_elements;      ///< Number of elements in the queue
        comparator_function_t compare;          ///< Comparator function pointer
        destructor_function_t destructor;       ///< Destructor function pointer
};

/// INITIALIZE ////////////////////////////////////////////////////////////////

queue_t* queue_init(size_t data_size, comparator_function_t cmp){
        assert(cmp && data_size > 0);
        // Allocate queue
        queue_t *queue = malloc(sizeof(*queue));
        if (!queue) return NULL;
        // Initialize queue
        queue->head = NULL;
        queue->tail = NULL;
        queue->compare = cmp;
        queue->data_size = data_size;
        queue->n_elements = 0;
        queue->destructor = NULL;
        return queue;
}

void queue_set_comparator(queue_t *queue, comparator_function_t cmp){
        if (queue && cmp)
                queue->compare = cmp;
}

void queue_set_destructor(queue_t *queue, destructor_function_t destructor){
        if (queue)
                queue->destructor = destructor;
}

static queue_tNode* queue_init_node(void *element, size_t size){
        queue_tNode *node = malloc(offsetof(queue_tNode, info) + size);
        if (!node) return NULL;
        memcpy(node->info, element, size);
        node->next = NULL;
        return node;
}

///////////////////////////////////////////////////////////////////////////////

/// ENQUEUE ///////////////////////////////////////////////////////////////////

int queue_enqueue(queue_t *queue, void *element){
        assert(queue && element);
        queue_tNode *node = queue_init_node(element, queue->data_size);
        if (!node) return ERROR;
        if (!queue->head){
                queue->head = node;
                queue->tail = node;
        }else{
                queue->tail->next = node;
                queue->tail = queue->tail->next;
        }
        queue->n_elements++;
        return SUCCESS;
}

int queue_enqueue_array(queue_t *queue, void *array, size_t array_length){
        assert(queue && array);
        while (array_length-- > 0){
                int status = queue_enqueue(queue, array);
                if (status != SUCCESS)
                        return status;
                array = void_offset(array, queue->data_size);
        }
        return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

/// DEQUEUE ///////////////////////////////////////////////////////////////////

void* queue_dequeue(queue_t *queue, void *dest){
        assert(queue && dest);
        if (queue->head == NULL)
                return NULL;
        queue_tNode *aux = queue->head;
        queue->head = queue->head->next;
        memcpy(dest, aux->info, queue->data_size);
        free(aux);
        queue->n_elements--;
        return dest;
}

int queue_dequeue_array(queue_t *queue, void *array, size_t array_length){
        assert(queue && array);
        while (array_length-- > 0) {
                if (!queue_dequeue(queue, array))
                        break;
                array = void_offset(array, queue->data_size);
        }
        return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

/// PEEK-EXISTS-SIZE //////////////////////////////////////////////////////////

void* queue_peek(queue_t *queue, void *dest){
        assert(queue && dest);
        if (queue->head == NULL)
                return NULL;
        else
                return memcpy(dest, queue->head->info, queue->data_size);
}

bool queue_exists(queue_t *queue, void *element){
        assert(queue && element);
        queue_tNode *aux = queue->head;
        while (aux != NULL){
                if(queue->compare(aux->info, element) == 0)
                        return true;
                aux = aux->next;
        }
        return false;
}

int queue_remove(queue_t *queue, void *element){
        assert(queue && element);
        queue_tNode** aux = &queue->head;
        while (*aux != NULL && queue->compare((*aux)->info, element) != 0)
                aux = &(*aux)->next;
        if (!*aux)
                return ELEMENT_NOT_FOUND_ERROR;
        queue_tNode *del = *aux;
        *aux = (*aux)->next;
        free(del);
        queue->n_elements--;
        return SUCCESS;
}

size_t queue_size(queue_t *queue){
        return queue ? queue->n_elements : 0;
}

bool queue_isempty(queue_t *queue){
        return queue ? queue->head == NULL : true;
}

///////////////////////////////////////////////////////////////////////////////

/// FREE //////////////////////////////////////////////////////////////////////

static void queue_free_node(queue_tNode *node, destructor_function_t destructor){
        if (node == NULL)
                return;
        if (destructor)
                destructor(node->info);
        queue_free_node(node->next, destructor);
        free(node);
}

static void _queue_free(queue_t *queue){
        if (queue){
                queue_free_node(queue->head, queue->destructor);
                free(queue);
        }
}

void (queue_free)(queue_t *q, ...){
        if (!q)
                return;
        va_list arg;
        va_start(arg, q);
        do {
                _queue_free(q);
                q = va_arg(arg, queue_t*);
        } while (q);
        va_end(arg);
}

void queue_clear(queue_t *queue){
        if (queue){
                queue_free_node(queue->head, queue->destructor);
                queue->head = NULL;
                queue->tail = NULL;
        }
}
