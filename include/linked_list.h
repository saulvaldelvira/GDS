/*
 * linked_list.h - Doubly Linked List definition.
 * Author: Saúl Valdelvira (2023)
 */
#pragma once
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "compare.h"

#ifdef LINKED_LIST_IMPL

        /*
         * Node of a Linked List
        */
        typedef struct LLNode {
                struct LLNode *next;
                struct LLNode *prev;
                byte info[];
        }LLNode;

#endif


/**
 * Linked List structure.
 */
typedef struct linked_list_t linked_list_t;

/**
 * Initializes the linked list
 * @param data_size size of the data stored.
 * @param cmp pointer to a function that receives two void pointers and returns 1 if the first one is greater than the
 *              second, 0 if they are equal and -1 if the first one is smaller than the second one
*/
linked_list_t* list_init(size_t data_size, comparator_function_t cmp);

/**
 * Changes the comparator function of the list
 * @param cmp the new comparator function
*/
void list_set_comparator(linked_list_t *list, comparator_function_t cmp);

/**
 * Changes the comparator function of the list
 * @param destructor the new destructor function
*/
void list_set_destructor(linked_list_t *list, destructor_function_t destructor);

/**
 * Adds the given element to the end of the list
 * @return 1 if the operation is successful
*/
int list_append(linked_list_t *list, void *element);

/**
 * Pushes back in the list the first [array_length] elements in the array
*/
int list_append_array(linked_list_t *list, void *array, size_t array_length);

/**
 * Adds the given element to the start of the list
 * @return 1 if the operation is successful
*/
int list_push_front(linked_list_t *list, void *element);

/**
 * Pushes front in the list the first [array_length] elements in the array
*/
int list_push_front_array(linked_list_t *list, void *array, size_t array_length);

/**
 * Changes the given element in the list with the replacement, if it exists.
 *
 * @return 1 if the operation is successful
*/
int list_set(linked_list_t *list, void *element, void *replacement);

/**
 * @param[out] dest adress to store the element into.
 * @return a pointer to the first element in the list that is equal to the parameter element.
*/
void* list_get(const linked_list_t *list, void *element, void *dest);

/**
 * @return The first element in the list, or NULL if it does not exists
*/
void* list_get_front(const linked_list_t *list, void *dest);

/**
 * @return The last element in the list, or NULL if it does not exists
*/
void* list_get_back(const linked_list_t *list, void *dest);

/**
 * Copies into the array the first [array_length] elements of the list
*/
void* list_get_into_array(const linked_list_t *list, void *array, size_t array_length);

/**
 * Creates an array with the first [array_length] elements of the list.
 * @param array_length the number of elements to get. Pass 0 to get all the elements in the list
 * @note Remember to free the array.
*/
void* list_get_array(const linked_list_t *list, size_t array_length);

/**
* @return true if the element exists in the list
*/
bool list_exists(const linked_list_t *list, void *element);

/**
 * @return the number of elements in the list
*/
size_t list_size(const linked_list_t *list);

/**
* @return true if the list is empty
*/
bool list_isempty(const linked_list_t *list);

/**
 * Removes all the elements from the list
*/
void list_clear(linked_list_t *list);

/**
 * Removes the specified element.
 * @note If defined, the destructor will be called on the removed element.
 * @note If you don't want that, see list_pop
 * @return 1 if the operation is successful.
 */
int list_remove(linked_list_t *list, void *element);

/**
 * Removes the first element.
 * @note If defined, the destructor will be called on the removed element.
 * @note If you don't want that, see list_pop_front
 * @return 1 if the operation is successful.
 */
int list_remove_front(linked_list_t *list);

/**
 * Removes the last element.
 * @note If defined, the destructor will be called on the removed element.
 * @note If you don't want that, see list_pop_back
 * @return 1 if the operation is successful.
 */
int list_remove_back(linked_list_t *list);

/**
 * Removes from the first [array_length] elements of the given array.
 * @note If defined, the destructor will be called on the removed elements.
 * @note If you don't want that, see list_pop_array
 * @return 1 if the operation is successful
*/
int list_remove_array(linked_list_t *list, void *array, size_t array_length);

/**
 * Pops the given element
 * @param[out] dest if not NULL, copies the element into it.
 * @note If you want the destructor to be called on the element, use list_remove instead
 * @return the dest pointer
 */
void* list_pop(linked_list_t *list, void *element, void *dest);

/**
 * Pops the frist element
 * @param[out] dest if not NULL, copies the element into it.
 * @note If you want the destructor to be called on the element, use list_remove_front instead
 * @return the dest pointer
 */
void* list_pop_front(linked_list_t *list, void *dest);

/**
 * Pops the last element
 * @param[out] dest if not NULL, copies the element into it.
 * @note If you want the destructor to be called on the element, use list_remove_back instead
 * @return the dest pointer
 */
void* list_pop_back(linked_list_t *list, void *dest);

/**
 * Pops from the first [array_length] elements of the given array.
 * @param[out] dest an array. If not NULL, copies the elements into it.
 *             Must be large enough to, at least, hold [array_length] elements
 * @note If you want the destructor to be called on the element, use list_remove_array instead
 * @return the dest pointer
*/
void* list_pop_array(linked_list_t *list, void *array, size_t array_length, void *dest);

/**
 * @return a new linked_list_t with the elements of the two given
 *         lists, or NULL if the lists have different data_size.
*/
linked_list_t* list_join(const linked_list_t *list_1, const linked_list_t *list_2);

void list_free(linked_list_t *h, ...);

/**
 * Frees all the given lists.
 */
#define list_free(...) list_free(__VA_ARGS__, 0L)

/* ITERATOR */
typedef struct list_iterator_t {
#ifdef LINKED_LIST_IMPL
        LLNode
#else
        void
#endif
        *next, *prev;
        uint16_t data_size;
} list_iterator_t;

/*
 * Create an iterator of the given list.
 * CAUTION:
 * DO NOT use this iterator after the original list has been freed.
 * */
list_iterator_t list_iterator(const linked_list_t *list);
list_iterator_t list_iterator_from_back(const linked_list_t *list);

void* list_it_next(list_iterator_t *it, void *dst);
void* list_it_prev(list_iterator_t *it, void *dst);
void* list_it_peek_next(list_iterator_t *it, void *dst);
void* list_it_peek_prev(list_iterator_t *it, void *dst);
bool list_it_has_next(list_iterator_t *it);
bool list_it_has_prev(list_iterator_t *it);

#ifdef __cplusplus
}
#endif

#endif
