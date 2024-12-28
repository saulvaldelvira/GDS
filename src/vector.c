/*
 * vector.c - vector implementation.
 * Author: Saúl Valdelvira (2023)
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "definitions.h"
#include "compare.h"
#include <string.h>
#include <stdarg.h>
#include "vector.h"
#include <assert.h>
#include "gdsmalloc.h"

#define VECTOR_DEFAULT_SIZE 12

#ifndef VECTOR_GROW_FACTOR
#define VECTOR_GROW_FACTOR 2
#endif
static_assert(VECTOR_GROW_FACTOR > 1, "VECTOR_GROW_FACTOR must be > 1");

struct vector {
        size_t n_elements;                      ///< Number of elements in the vector
        size_t capacity;                        ///< Current capacity of the vector
        size_t data_size;                       ///< Size (in bytes) of the data type being stored
        comparator_function_t compare;          ///< Comparator function pointer
        destructor_function_t destructor;       ///< Destructor function pointer
        void *elements;
};

/// INITIALIZE ////////////////////////////////////////////////////////////////

__inline
vector_t* vector_init(size_t data_size, comparator_function_t cmp){
        return vector_with_capacity(data_size, cmp, VECTOR_DEFAULT_SIZE);
}

vector_t* vector_with_capacity(size_t data_size, comparator_function_t cmp, size_t capacity) {
        assert(cmp && data_size > 0);
        vector_t *vector = gdsmalloc(sizeof(*vector));
        if (unlikely(!vector)) return NULL;
        vector->elements = gdsmalloc(capacity * data_size);
        if (!vector->elements){
                gdsfree(vector);
                return NULL;
        }
        vector->data_size = data_size;
        vector->n_elements = 0;
        vector->capacity = capacity;
        vector->compare = cmp;
        vector->destructor = NULL;
        return vector;
}

__inline
void vector_set_comparator(vector_t *vector, comparator_function_t cmp){
        if (vector && cmp)
                vector->compare = cmp;
}

__inline
comparator_function_t vector_get_comparator(const vector_t *vector){
        return vector ? vector->compare : NULL;
}

__inline
void vector_set_destructor(vector_t *vector, destructor_function_t destructor){
        if (vector)
                vector->destructor = destructor;
}

__inline
destructor_function_t vector_get_destructor(const vector_t *vector){
        return vector ? vector->destructor : NULL;
}

__inline
size_t vector_get_data_size(const vector_t *vector){
        return vector ? vector->data_size : 0;
}

static int check_and_transform_index(ptrdiff_t *index_1, ptrdiff_t *index_2, size_t n_elements){
        if (*index_1 < 0)
                *index_1 = n_elements + *index_1;
        if ((size_t)*index_1 >= n_elements)
                return GDS_INDEX_BOUNDS_ERROR;
        if (index_2 == NULL)
                return GDS_SUCCESS;
        if (*index_2 < 0)
                *index_2 = n_elements + *index_2;
        if ((size_t)*index_2 >= n_elements)
                return GDS_INDEX_BOUNDS_ERROR;
        return GDS_SUCCESS;
}

__inline
void* vector_get_buffer(vector_t *self) {
        assert(self);
        return self->elements;
}

///////////////////////////////////////////////////////////////////////////////

/// ADD-SET ///////////////////////////////////////////////////////////////////////

static int resize_buffer(vector_t *vector, size_t new_size){
        assert(vector->n_elements <= new_size);
        void *ptr = gdsrealloc(vector->elements, new_size * vector->data_size);
        if (!ptr) return GDS_ERROR;
        vector->elements = ptr;
        vector->capacity = new_size;
        return GDS_SUCCESS;
}

int vector_append(vector_t *vector, void *element){
        assert(vector && element);
        return vector_insert_at(vector, vector->n_elements, element);
}

__inline
int vector_push_front(vector_t *vector, void *element){
        return vector_insert_at(vector, 0, element);
}

int vector_insert_array(vector_t *vector, ptrdiff_t index, const void *array, size_t array_length){
        assert(vector && array);
        if (vector->capacity - vector->n_elements < array_length){
                if (resize_buffer(vector, vector->capacity + array_length) == GDS_ERROR)
                        return GDS_ERROR;
        }
        if (index >= 0 && (size_t)index == vector->n_elements){
                void *dst = void_offset(vector->elements, vector->n_elements * vector->data_size);
                memcpy(dst, array, array_length * vector->data_size);
        }else {
                int status = check_and_transform_index(&index, NULL, vector->n_elements);
                if (status != GDS_SUCCESS)
                        return status;
                size_t n_elements_to_move = vector->n_elements - index;
                void *dst = void_offset(vector->elements, (index + array_length) * vector->data_size);
                void *src = void_offset(vector->elements, index * vector->data_size);
                memmove(dst, src, n_elements_to_move * vector->data_size);
                memcpy(src, array, array_length * vector->data_size);
        }
        vector->n_elements += array_length;
        return GDS_SUCCESS;
}

int vector_append_array(vector_t *vector, const void *array, size_t array_length){
        assert(vector);
        return vector_insert_array(vector, vector->n_elements, array, array_length);
}

__inline
int vector_push_front_array(vector_t *vector, const void *array, size_t array_length){
        return vector_insert_array(vector, 0, array, array_length);
}

int vector_set_at(vector_t *vector, ptrdiff_t index, void *replacement){
        assert(vector && replacement);
        int status = check_and_transform_index(&index, NULL, vector->n_elements);
        if (status != GDS_SUCCESS)
                return status;
        void *tmp = void_offset(vector->elements, index * vector->data_size);
        if (vector->destructor)
                vector->destructor(tmp);
        memmove(tmp, replacement, vector->data_size);
        return GDS_SUCCESS;
}

int vector_set(vector_t *vector, void *element, void *replacement){
        assert(vector && element && replacement);
        ptrdiff_t index = vector_indexof(vector, element);
        if (index < 0) return index;
        vector_set_at(vector, index, replacement);
        return GDS_SUCCESS;
}

int vector_insert(vector_t *vector, void *element, void *insert){
        assert(vector && element && insert);
        ptrdiff_t index = vector_indexof(vector, element);
        if (index < 0)
                return index;
        return vector_insert_at(vector, index, insert);
}

int vector_insert_at(vector_t *vector, ptrdiff_t index, void *element){
        assert(vector && element);
        if (vector->n_elements == vector->capacity){
                if (resize_buffer(vector, vector->capacity * VECTOR_GROW_FACTOR) == GDS_ERROR)
                        return GDS_ERROR;
        }
        if (index >= 0 && (size_t)index == vector->n_elements){
                void *dst = void_offset(vector->elements, vector->n_elements * vector->data_size);
                memcpy(dst, element, vector->data_size);
        }else {
                int status = check_and_transform_index(&index, NULL, vector->n_elements);
                if (status != GDS_SUCCESS)
                        return status;
                void *src = void_offset(vector->elements, index * vector->data_size);
                void *dst = void_offset(vector->elements, (index + 1) * vector->data_size);
                int n = vector->n_elements - index; // number of elements to shift
                memmove(dst, src, n * vector->data_size); // Shift elements to the right
                memcpy(src, element, vector->data_size); // Insert the element
        }
        vector->n_elements++;
        return GDS_SUCCESS;
}

void vector_map(vector_t *vector, void (*func) (void *,void*), void *args){
        assert(vector && func);
        void *tmp = vector->elements;
        for (size_t i = 0; i < vector->n_elements; ++i){
                func(tmp, args);
                tmp = void_offset(tmp, vector->data_size);
        }
}

vector_t* vector_filter(vector_t *vector, bool (*func) (void*)){
        assert(vector && func);
        vector_t *result = vector_init(vector->data_size, vector->compare);
        if (!result) return NULL;
        void *tmp = vector->elements;
        for (size_t i = 0; i < vector->n_elements; ++i){
                if (func(tmp))
                        vector_append(result, tmp);
                tmp = void_offset(tmp, vector->data_size);
        }
        return result;
}

__inline
void vector_sort(vector_t *vector){
        if (vector)
                qsort(vector->elements, vector->n_elements, vector->data_size, vector->compare);
}

void* vector_reduce(vector_t *vector, void (*func) (const void*,void*), void *dest){
        assert(vector && func && dest);
        void *tmp = vector->elements;
        for (size_t i = 0; i < vector->n_elements; ++i){
                func(tmp,dest);
                tmp = void_offset(tmp, vector->data_size);
        }
        return dest;
}

///////////////////////////////////////////////////////////////////////////////

/// REMOVE ////////////////////////////////////////////////////////////////////

int vector_remove_at(vector_t *vector, ptrdiff_t index){
        assert(vector);
        int status = check_and_transform_index(&index, NULL, vector->n_elements);
        if (status != GDS_SUCCESS)
                return status;
        void *remove = void_offset(vector->elements, index * vector->data_size);
        if (vector->destructor)
                vector->destructor(remove);
        if ((size_t)index < vector->n_elements - 1){
                size_t leftover = (vector->n_elements - index - 1) * vector->data_size;
                void *src = void_offset(vector->elements, (index+1) * vector->data_size);
                memmove(remove, src, leftover);
        }
        vector->n_elements--;
        return GDS_SUCCESS;
}

int vector_remove(vector_t *vector, void *element){
        assert(vector && element);
        ptrdiff_t index = vector_indexof(vector, element);
        if (index < 0)
                return index;
        return vector_remove_at(vector, index);
}

int vector_remove_front(vector_t *vector){
        assert(vector);
        if (vector->n_elements == 0)
                return GDS_SUCCESS;
        return vector_remove_at(vector, 0);
}

int vector_remove_back(vector_t *vector){
        assert(vector);
        if (vector->n_elements == 0)
                return GDS_SUCCESS;
        return vector_remove_at(vector, vector->n_elements - 1);
}

int vector_remove_array(vector_t *vector, void *array, size_t array_length){
        assert(vector && array);
        for (size_t i = 0; i < array_length; i++){
                void *tmp = void_offset(array, i * vector->data_size);
                int status = vector_remove(vector, tmp);
                if (status != GDS_SUCCESS)
                        return status;
        }
        return GDS_SUCCESS;
}

void* vector_pop_at(vector_t *vector, ptrdiff_t index, void *dest){
        assert(vector);
        int status = check_and_transform_index(&index, NULL, vector->n_elements);
        if (status != GDS_SUCCESS)
                return NULL;
        void *remove = void_offset(vector->elements, index * vector->data_size);
        if (dest)
                memcpy(dest, remove, vector->data_size);
        if ((size_t)index < vector->n_elements - 1){
                size_t leftover = (vector->n_elements - index - 1) * vector->data_size;
                void *src = void_offset(vector->elements, (index+1) * vector->data_size);
                memmove(remove, src, leftover);
        }
        vector->n_elements--;
        return dest;
}

void* vector_pop(vector_t *vector, void *element, void *dest){
        assert(vector && element);
        ptrdiff_t index = vector_indexof(vector, element);
        if (index < 0)
                return NULL;
        return vector_pop_at(vector, index, dest);
}

void* vector_pop_front(vector_t *vector, void *dest){
        assert(vector);
        if (vector->n_elements == 0)
                return NULL;
        return vector_pop_at(vector, 0, dest);
}

void* vector_pop_back(vector_t *vector, void *dest){
        assert(vector);
        if (vector->n_elements == 0)
                return NULL;
        return vector_pop_at(vector, vector->n_elements - 1, dest);
}

void* vector_pop_array(vector_t *vector, void *array, size_t array_length, void *dest){
        assert(vector && array);
        while (array_length-- > 0){
                vector_pop(vector, array, dest);
                array = void_offset(array, vector->data_size);
                if (dest)
                        dest = void_offset(dest, vector->data_size);
        }
        return dest;
}

///////////////////////////////////////////////////////////////////////////////

/// GET ////////////////////////////////////////////////////////////////

ptrdiff_t vector_indexof(const vector_t *vector, void *element){
        assert(vector && element);
        void *ptr = vector->elements; // Current element in the iteration
        for (size_t i = 0; i < vector->n_elements; i++){
                if (vector->compare(ptr, element) == 0){
                        return i;
                }
                ptr = void_offset(ptr, vector->data_size);
        }
        return GDS_ELEMENT_NOT_FOUND_ERROR;
}

__inline
bool vector_exists(const vector_t *vector, void *element){
        return vector ? vector_indexof(vector, element) >= 0 : false;
}

__inline
bool vector_isempty(const vector_t *vector){
        return vector ? vector->n_elements == 0 : true;
}

__inline
static void* __get_at(const vector_t *vector, ptrdiff_t index) {
        assert(vector);
        int status = check_and_transform_index(&index, NULL, vector->n_elements);
        if (status != GDS_SUCCESS)
                return NULL;
        return void_offset(vector->elements, index * vector->data_size);
}

void* vector_at(const vector_t *vector, ptrdiff_t index, void *dest){
        assert(vector && dest);
        const void *tmp = __get_at(vector, index);
        return tmp ? memcpy(dest, tmp, vector->data_size) : NULL;
}

void* vector_get(const vector_t *vector, void *element, void *dest){
        assert(vector && element && dest);
        ptrdiff_t index = vector_indexof(vector, element);
        if (index < 0)
                return NULL;
        return vector_at(vector, index, dest);
}

void* vector_front(const vector_t *vector, void *dest){
        assert(vector && dest);
        if (vector->n_elements > 0)
                return vector_at(vector, 0, dest);
        else
                return NULL;
}

void* vector_back(const vector_t *vector, void *dest){
        assert(vector && dest);
        if (vector->n_elements > 0)
                return vector_at(vector, vector->n_elements - 1, dest);
        else
                return NULL;
}

void* vector_at_ref(vector_t *self, ptrdiff_t index) {
        assert(self);
        return __get_at(self, index);
}

void* vector_get_ref(vector_t *vector, void *element) {
        assert(vector);
        ptrdiff_t index = vector_indexof(vector, element);
        if (index < 0)
                return NULL;
        return vector_at_ref(vector, index);
}

void* vector_front_ref(vector_t *vector){
        assert(vector);
        if (vector->n_elements > 0)
                return vector_at_ref(vector, 0);
        else
                return NULL;
}

void* vector_back_ref(vector_t *vector){
        assert(vector);
        if (vector->n_elements > 0)
                return vector_at_ref(vector, vector->n_elements - 1);
        else
                return NULL;
}

void* vector_get_into_array(const vector_t *vector, void *array, size_t array_length){
        assert(vector && array);
        if (array_length > vector->n_elements)
                array_length = vector->n_elements;
        memcpy(array, vector->elements, array_length * vector->data_size);
        return array;
}

void* vector_get_array(const vector_t *vector, size_t array_length){
        assert(vector);
        if (array_length == 0 || array_length > vector->n_elements){
                array_length = vector->n_elements;
        }
        void *array = gdsmalloc(vector->data_size * array_length);
        if (!array){
                return NULL;
        }
        if (!vector_get_into_array(vector, array, array_length)){
                gdsfree(array);
                return NULL;
        }
        return array;
}

///////////////////////////////////////////////////////////////////////////////

/// OTHER /////////////////////////////////////////////////////////////////////

int vector_swap(vector_t *vector, ptrdiff_t index_1, ptrdiff_t index_2){
        assert(vector);
        int status = check_and_transform_index(&index_1, &index_2, vector->n_elements);
        if (status != GDS_SUCCESS)
                return status;
        void *tmp = gdsmalloc(vector->data_size);
        if (!tmp || !vector_at(vector, index_1, tmp)){
                gdsfree(tmp);
                return GDS_ERROR;
        }

        void *e1 = void_offset(vector->elements, index_1 * vector->data_size);
        void *e2 = void_offset(vector->elements, index_2 * vector->data_size);
        memmove(e1, e2, vector->data_size);
        memcpy(e2, tmp, vector->data_size);

        gdsfree(tmp);
        return GDS_SUCCESS;
}

int vector_compare(const vector_t *vector, ptrdiff_t index_1, ptrdiff_t index_2){
        assert(vector);
        int status = check_and_transform_index(&index_1, &index_2, vector->n_elements);
        if (status != GDS_SUCCESS)
                return status;
        void *e1 = void_offset(vector->elements, index_1 * vector->data_size);
        void *e2 = void_offset(vector->elements, index_2 * vector->data_size);
        return vector->compare(e1, e2);
}

__inline
size_t vector_size(const vector_t *vector){
        return vector ? vector->n_elements : 0;
}

__inline
size_t vector_capacity(const vector_t *vector){
        return vector ? vector->capacity : 0;
}

int vector_reserve(vector_t *vector, size_t n_elements){
        assert(vector);
        if (vector->capacity < n_elements){
                if (resize_buffer(vector, n_elements) == GDS_ERROR)
                        return GDS_ERROR;
        }
        return GDS_SUCCESS;
}

int vector_resize(vector_t *vector, size_t n_elements, constructor_function_t constructor){
        assert(vector);
        if (n_elements == vector->n_elements) return GDS_SUCCESS;

        if (vector->capacity < n_elements){
                if (resize_buffer(vector, n_elements) == GDS_ERROR)
                        return GDS_ERROR;
        }

        #define foreach(start,end,f) \
                void *ptr = void_offset(vector->elements, start * vector->data_size);\
                for (size_t i = start; i < end; i++) {\
                        f(ptr);\
                        ptr = void_offset(ptr, vector->data_size);\
                }

        if (constructor && vector->n_elements < n_elements) {
                foreach(vector->n_elements, n_elements, constructor);
        }
        if (vector->destructor && vector->n_elements > n_elements) {
                foreach(n_elements, vector->n_elements, vector->destructor);
        }
        vector->n_elements = n_elements;
        return GDS_SUCCESS;
}

int vector_shrink(vector_t *vector){
        assert(vector);
        return resize_buffer(vector, vector->n_elements);
}

vector_t* vector_dup(vector_t *vector){
        assert(vector);
        vector_t *dup = vector_init(vector->data_size, vector->compare);
        vector_set_destructor(dup, vector->destructor);
        vector_resize(dup, vector->n_elements, NULL);
        memcpy(dup->elements, vector->elements, vector->n_elements * vector->data_size);
        return dup;
}

vector_t* vector_join(const vector_t *vector_1, const vector_t *vector_2){
        assert(vector_1 && vector_2 && vector_1->data_size == vector_2->data_size);
        vector_t *vector_joint = vector_init(vector_1->data_size, vector_1->compare);

        int status = vector_append_array(vector_joint, vector_1->elements, vector_1->n_elements);
        if (status != GDS_SUCCESS){
                vector_free(vector_joint);
                return NULL;
        }
        status = vector_append_array(vector_joint, vector_2->elements, vector_2->n_elements);
        if (status != GDS_SUCCESS){
                vector_free(vector_joint);
                return NULL;
        }
        return vector_joint;
}

///////////////////////////////////////////////////////////////////////////////

/// FREE //////////////////////////////////////////////////////////////////////

static void destroy_content(vector_t *vector){
        if (vector->destructor){
                void *tmp = vector->elements;
                for (size_t i = 0; i < vector->n_elements; i++){
                        vector->destructor(tmp);
                        tmp = void_offset(tmp, vector->data_size);
                }
        }
}

void vector_clear(vector_t *vector){
        if (vector){
                destroy_content(vector);
                vector->n_elements = 0;
        }
}

static void _vector_free(vector_t *vector){
        if (!vector)
                return;
        destroy_content(vector);
        gdsfree(vector->elements);
        gdsfree(vector);
}

void (vector_free)(vector_t *v, ...){
        if (!v)
                return;
        va_list arg;
        va_start(arg, v);
        do {
                _vector_free(v);
                v = va_arg(arg, vector_t*);
        } while (v);
        va_end(arg);
}

void vector_reset(vector_t *vector){
        if (!vector)
                return;
        destroy_content(vector);
        gdsfree(vector->elements);
        vector->elements = gdsmalloc(VECTOR_DEFAULT_SIZE * vector->data_size);
        assert(vector->elements);
        vector->n_elements = 0;
        vector->capacity = VECTOR_DEFAULT_SIZE;
}

/* ITERATOR */

vector_iterator_t vector_iterator(const vector_t *vector) {
        return (vector_iterator_t) {
                .vector = vector,
                .next = 0,
                .prev = -1
        };
}

vector_iterator_t vector_iterator_from_back(const vector_t *vector) {
        return (vector_iterator_t) {
                .vector = vector,
                .prev = vector->n_elements > 0 ? vector->n_elements - 1 : 0,
                .next = -1,
        };
}

void* vector_it_next(vector_iterator_t *it, void *dst) {
        assert(it && dst);
        if ((size_t)it->next >= it->vector->n_elements)
                return NULL;
        it->prev = it->next;
        it->next++;
        return vector_at(it->vector, it->prev, dst);
}

void* vector_it_prev(vector_iterator_t *it, void *dst) {
        assert(it && dst);
        if (it->prev < 0)
                return NULL;
        it->next = it->prev;
        it->prev--;
        return vector_at(it->vector, it->next, dst);
}

void* vector_it_peek_next(vector_iterator_t *it, void *dst) {
        assert(it && dst);
        return vector_at(it->vector, it->next, dst);
}

void* vector_it_peek_prev(vector_iterator_t *it, void *dst) {
        assert(it && dst);
        if (it->prev < 0)
                return NULL;
        return vector_at(it->vector, it->prev, dst);
}

bool vector_it_has_next(vector_iterator_t *it) {
        assert(it);
        return (size_t)it->next < it->vector->n_elements;
}

bool vector_it_has_prev(vector_iterator_t *it) {
        assert(it);
        return it->prev >= 0;
}

