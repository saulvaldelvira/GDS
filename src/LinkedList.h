/**
 * @file LinkedList.h
 * Definition of the Linked List.
 *
 *  Copyright (C) 2023 - Saúl Valdelvira \n
 *  License: BSD 3-Clause \n
 *  Email: saul@saulv.es
 */
/// @cond
#pragma once
#ifndef LINKED_LIST_H
#define LINKED_LIST_H
/// @endcond

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include "./util/comparator.h"

/**
 * Linked List structure.
 */
typedef struct LinkedList LinkedList;

/**
 * Initializes the linked list
 * @param data_size size of the data stored.
 * @param cmp pointer to a function that receives two void pointers and returns 1 if the first one is greater than the
 *              second, 0 if they are equal and -1 if the first one is smaller than the second one
*/
LinkedList* list_init(size_t data_size, comparator_function_t cmp);

/**
 * Changes the comparator function of the list
 * @param cmp the new comparator function
*/
void list_set_comparator(LinkedList *list, comparator_function_t cmp);

/**
 * Changes the comparator function of the list
 * @param destructor the new destructor function
*/
void list_set_destructor(LinkedList *list, destructor_function_t destructor);

/**
 * Adds the given element to the end of the list
 * @return 1 if the operation is successful
*/
int list_append(LinkedList *list, void *element);

/**
 * Pushes back in the list the first [array_length] elements in the array
*/
int list_append_array(LinkedList *list, void *array, size_t array_length);

/**
 * Adds the given element to the start of the list
 * @return 1 if the operation is successful
*/
int list_push_front(LinkedList *list, void *element);

/**
 * Pushes front in the list the first [array_length] elements in the array
*/
int list_push_front_array(LinkedList *list, void *array, size_t array_length);

/**
 * Changes the given element in the list with the replacement, if it exists.
 *
 * @return 1 if the operation is successful
*/
int list_set(LinkedList *list, void *element, void *replacement);

/**
 * @param[out] dest adress to store the element into.
 * @return a pointer to the first element in the list that is equal to the parameter element.
*/
void* list_get(LinkedList *list, void *element, void *dest);

/**
 * @return The first element in the list, or NULL if it does not exists
*/
void* list_get_front(LinkedList *list, void *dest);

/**
 * @return The last element in the list, or NULL if it does not exists
*/
void* list_get_back(LinkedList *list, void *dest);

/**
 * Copies into the array the first [array_length] elements of the list
*/
void* list_get_into_array(LinkedList *list, void *array, size_t array_length);

/**
 * Creates an array with the first [array_length] elements of the list.
 * @param array_length the number of elements to get. Pass 0 to get all the elements in the list
 * @note Remember to free the array.
*/
void* list_get_array(LinkedList *list, size_t array_length);

/**
* @return true if the element exists in the list
*/
bool list_exists(LinkedList *list, void *element);

/**
 * @return the number of elements in the list
*/
size_t list_size(LinkedList *list);

/**
* @return true if the list is empty
*/
bool list_isempty(LinkedList *list);

/**
 * Removes the specified element.
 * @note If defined, the destructor will be called on the removed element.
 * @note If you don't want that, see list_pop
 * @return 1 if the operation is successful.
 */
int list_remove(LinkedList *list, void *element);

/**
 * Removes the first element.
 * @note If defined, the destructor will be called on the removed element.
 * @note If you don't want that, see list_pop_front
 * @return 1 if the operation is successful.
 */
int list_remove_front(LinkedList *list);

/**
 * Removes the last element.
 * @note If defined, the destructor will be called on the removed element.
 * @note If you don't want that, see list_pop_back
 * @return 1 if the operation is successful.
 */
int list_remove_back(LinkedList *list);

/**
 * Removes from the first [array_length] elements of the given array.
 * @note If defined, the destructor will be called on the removed elements.
 * @note If you don't want that, see list_pop_array
 * @return 1 if the operation is successful
*/
int list_remove_array(LinkedList *list, void *array, size_t array_length);

/**
 * Pops the given element
 * @param[out] dest if not NULL, copies the element into it.
 * @note If you want the destructor to be called on the element, use list_remove instead
 * @return the dest pointer
 */
void* list_pop(LinkedList *list, void *element, void *dest);

/**
 * Pops the frist element
 * @param[out] dest if not NULL, copies the element into it.
 * @note If you want the destructor to be called on the element, use list_remove_front instead
 * @return the dest pointer
 */
void* list_pop_front(LinkedList *list, void *dest);

/**
 * Pops the last element
 * @param[out] dest if not NULL, copies the element into it.
 * @note If you want the destructor to be called on the element, use list_remove_back instead
 * @return the dest pointer
 */
void* list_pop_back(LinkedList *list, void *dest);

/**
 * Pops from the first [array_length] elements of the given array.
 * @param[out] dest an array. If not NULL, copies the elements into it.
 *             Must be large enough to, at least, hold [array_length] elements
 * @note If you want the destructor to be called on the element, use list_remove_array instead
 * @return the dest pointer
*/
void* list_pop_array(LinkedList *list, void *array, size_t array_length, void *dest);

/**
 * @return a new LinkedList with the elements of the two given lists.
 * @note No particular order of elements is guaranteed.
 * @note ATTENTION: The lists must store the same data. At least, they must have
 * the same data size. Also, the comparator function of list_1 will be taken,
 * because it is assumed that both list have the same comparator functions.
*/
LinkedList* list_join(LinkedList *list_1, LinkedList *list_2);

/**
 * Frees the memory allocated on the list.
*/
int list_free(LinkedList *list);

/**
 * Frees multiple lists at once.
 * @param n number of pointers to free.
*/
void list_free_all(unsigned int n, ...);

/**
 * Frees the memory allocated on the list, and resets it to it's initial state
 * @return the same pointer if sucess, NULL if the operation fails
*/
LinkedList* list_reset(LinkedList *list);

#ifdef __cplusplus
}
#endif

#endif
