/*
 * Vector.c - Vector implementation.
 * Author: Saúl Valdelvira (2023)
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include "./util/error.h"
#include "./util/definitions.h"
#include <string.h>
#include <stdarg.h>
#include "Vector.h"
#include <assert.h>

#define VECTOR_DEFAULT_SIZE 12

/**
 * Vector struct
 * @headerfile Vector.h <GDS/Vector.h>
 * @see Vector.h
*/
struct Vector {
	size_t n_elements;			///< Number of elements in the vector
	size_t max_elements;			///< Current capacity of the vector
	size_t data_size;			///< Size (in bytes) of the data type being stored
	comparator_function_t compare;		///< Comparator function pointer
	destructor_function_t destructor;	///< Destructor function pointer
	void *elements;
};

/// INITIALIZE ////////////////////////////////////////////////////////////////

Vector* vector_init(size_t data_size, comparator_function_t cmp){
	if (!cmp || data_size == 0)
		return NULL;
	Vector *vector = malloc(sizeof(*vector));
	assert(vector);
	vector->elements = malloc(VECTOR_DEFAULT_SIZE * data_size);
	assert(vector->elements);
	vector->data_size = data_size;
	vector->n_elements = 0;
	vector->max_elements = VECTOR_DEFAULT_SIZE;
	vector->compare = cmp;
	vector->destructor = NULL;
	return vector;
}

void vector_set_comparator(Vector *vector, comparator_function_t cmp){
	if (vector && cmp)
		vector->compare = cmp;
}

comparator_function_t vector_get_comparator(Vector *vector){
	return vector ? vector->compare : NULL;
}

void vector_set_destructor(Vector *vector, destructor_function_t destructor){
	if (vector)
		vector->destructor = destructor;
}

destructor_function_t vector_get_destructor(Vector *vector){
	return vector ? vector->destructor : NULL;
}

size_t vector_get_data_size(Vector *vector){
	return vector ? vector->data_size : 0;
}

static int check_and_transform_index(ptrdiff_t *index_1, ptrdiff_t *index_2, size_t n_elements){
	if (*index_1 < 0){
		if ((size_t)(*index_1 * -1) > n_elements)
			return INDEX_BOUNDS_ERROR;
		*index_1 = n_elements + *index_1;
	}
	if ((size_t)*index_1 >= n_elements)
		return INDEX_BOUNDS_ERROR;
	if (index_2 == NULL)
		return SUCCESS;
	if (*index_2 < 0){
		if ((size_t)(*index_2 * -1) > n_elements)
			return INDEX_BOUNDS_ERROR;
		*index_2 = n_elements + *index_2;
	}
	if ((size_t)*index_2 >= n_elements)
		return INDEX_BOUNDS_ERROR;
	return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

/// ADD-SET ///////////////////////////////////////////////////////////////////////

static void vector_resize(Vector *vector, size_t new_size){
	assert(vector->n_elements <= new_size);
	void *tmp = calloc(new_size, vector->data_size);
	assert(tmp);
	memcpy(tmp, vector->elements, vector->n_elements * vector->data_size);
	free(vector->elements);
	vector->elements = tmp;
	vector->max_elements = new_size;
}

int vector_append(Vector *vector, void *element){
	if (!vector || !element)
		return NULL_PARAMETER_ERROR;
	return vector_insert_at(vector, vector->n_elements, element);
}

int vector_push_front(Vector *vector, void *element){
	return vector_insert_at(vector, 0, element);
}

int vector_insert_array(Vector *vector, ptrdiff_t index, void *array, size_t array_length){
	if (!vector || !array)
		return NULL_PARAMETER_ERROR;
	if (vector->max_elements - vector->n_elements < array_length)
		vector_resize(vector, vector->max_elements + array_length);
	if (index >= 0 && (size_t)index == vector->n_elements){
		void *dst = void_offset(vector->elements, vector->n_elements * vector->data_size);
		memcpy(dst, array, array_length * vector->data_size);
	}else {
		int status = check_and_transform_index(&index, NULL, vector->n_elements);
		if (status != SUCCESS)
			return status;
		size_t n_elements_to_move = vector->n_elements - index;
		void *dst = void_offset(vector->elements, (index + array_length) * vector->data_size);
		void *src = void_offset(vector->elements, index * vector->data_size);
		memmove(dst, src, n_elements_to_move * vector->data_size);
		memcpy(src, array, array_length * vector->data_size);
	}
	vector->n_elements += array_length;
	return SUCCESS;
}

int vector_append_array(Vector *vector, void *array, size_t array_length){
	return vector_insert_array(vector, vector->n_elements, array, array_length);
}

int vector_push_front_array(Vector *vector, void *array, size_t array_length){
	return vector_insert_array(vector, 0, array, array_length);
}

int vector_set_at(Vector *vector, ptrdiff_t index, void *replacement){
	if (!vector || !replacement)
		return NULL_PARAMETER_ERROR;
	int status = check_and_transform_index(&index, NULL, vector->n_elements);
	if (status != SUCCESS)
		return status;
	void *tmp = void_offset(vector->elements, index * vector->data_size);
	memmove(tmp, replacement, vector->data_size);
	return SUCCESS;
}

int vector_set(Vector *vector, void *element, void *replacement){
	if (!vector || !element || !replacement)
		return NULL_PARAMETER_ERROR;
	for (size_t i=0; i < vector->n_elements; i++){
		void *ptr = void_offset(vector->elements, i * vector->data_size);
		if (vector->compare(ptr, element) == 0){
			memmove(ptr, replacement, vector->data_size);
			return SUCCESS;
		}
	}
	return ELEMENT_NOT_FOUND_ERROR;
}

int vector_insert(Vector *vector, void *element, void *insert){
	if (!vector || !element || !insert)
		return NULL_PARAMETER_ERROR;
	ptrdiff_t index = vector_indexof(vector, element);
	if (index < 0)
		return index;
	return vector_insert_at(vector, index, insert);
}

int vector_insert_at(Vector *vector, ptrdiff_t index, void *element){
	if (!vector || !element)
		return NULL_PARAMETER_ERROR;
	if (vector->n_elements == vector->max_elements)
		vector_resize(vector, vector->max_elements * 2);
	if (index >= 0 && (size_t)index == vector->n_elements){
		void *dst = void_offset(vector->elements, vector->n_elements * vector->data_size);
		memcpy(dst, element, vector->data_size);
	}else {
		int status = check_and_transform_index(&index, NULL, vector->n_elements);
		if (status != SUCCESS)
			return status;
		void *src = void_offset(vector->elements, index * vector->data_size);
		void *dst = void_offset(vector->elements, (index + 1) * vector->data_size);
		int n = vector->n_elements - index; // number of elements to shift
		memmove(dst, src, n * vector->data_size); // Shift elements to the right
		memcpy(src, element, vector->data_size); // Insert the element
	}
	vector->n_elements++;
	return SUCCESS;
}

int vector_populate(Vector *vector, void *templ){
	if (!vector || !templ)
		return NULL_PARAMETER_ERROR;
	void *tmp = vector->elements;
	for (size_t i = 0; i < vector->max_elements; i++){
		memcpy(tmp, templ, vector->data_size);
		tmp = void_offset(tmp, vector->data_size);
	}
	vector->n_elements = vector->max_elements;
	return SUCCESS;
}

int vector_process(Vector *vector, int (*func) (void *,void*), void *args){
	if (!vector || !func)
		return NULL_PARAMETER_ERROR;
	void *tmp = vector->elements;
	for (size_t i = 0; i < vector->n_elements; ++i){
		int status = func(tmp, args);
		if (status != SUCCESS)
			return status;
		tmp = void_offset(tmp, vector->data_size);
	}
	return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

/// REMOVE ////////////////////////////////////////////////////////////////////

int vector_remove_at(Vector *vector, ptrdiff_t index){
	if (!vector)
		return NULL_PARAMETER_ERROR;
	int status = check_and_transform_index(&index, NULL, vector->n_elements);
	if (status != SUCCESS)
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
	return SUCCESS;
}

int vector_remove(Vector *vector, void *element){
	if (!vector || !element)
		return NULL_PARAMETER_ERROR;
	ptrdiff_t index = vector_indexof(vector, element);
	if (index < 0)
		return index;
	return vector_remove_at(vector, index);
}

int vector_remove_front(Vector *vector){
	if (!vector)
		return NULL_PARAMETER_ERROR;
	if (vector->n_elements == 0)
		return SUCCESS;
	return vector_remove_at(vector, 0);
}

int vector_remove_back(Vector *vector){
	if (!vector)
		return NULL_PARAMETER_ERROR;
	if (vector->n_elements == 0)
		return SUCCESS;
	return vector_remove_at(vector, vector->n_elements - 1);
}

int vector_remove_array(Vector *vector, void *array, size_t array_length){
	if (!vector || !array)
		return NULL_PARAMETER_ERROR;
	for (size_t i = 0; i < array_length; i++){
		void *tmp = void_offset(array, i * vector->data_size);
		int status = vector_remove(vector, tmp);
		if (status != SUCCESS)
			return status;
	}
	return SUCCESS;
}

void* vector_pop_at(Vector *vector, ptrdiff_t index, void *dest){
	if (!vector)
		return NULL;
	int status = check_and_transform_index(&index, NULL, vector->n_elements);
	if (status != SUCCESS)
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

void* vector_pop(Vector *vector, void *element, void *dest){
	if (!vector || !element)
		return NULL;
	ptrdiff_t index = vector_indexof(vector, element);
	if (index < 0)
		return NULL;
	return vector_pop_at(vector, index, dest);
}

void* vector_pop_front(Vector *vector, void *dest){
	if (!vector)
		return NULL;
	if (vector->n_elements == 0)
		return NULL;
	return vector_pop_at(vector, 0, dest);
}

void* vector_pop_back(Vector *vector, void *dest){
	if (!vector)
		return NULL;
	if (vector->n_elements == 0)
		return NULL;
	return vector_pop_at(vector, vector->n_elements - 1, dest);
}

void* vector_pop_array(Vector *vector, void *array, size_t array_length, void *dest){
	if (!vector || !array)
		return NULL;
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

ptrdiff_t vector_indexof(Vector *vector, void *element){
	if (!vector || !element)
		return NULL_PARAMETER_ERROR;
	void *ptr = vector->elements; // Current element in the iteration
	for (size_t i = 0; i < vector->n_elements; i++){
		if (vector->compare(ptr, element) == 0){
			return i;
		}
		ptr = void_offset(ptr, vector->data_size);
	}
	return ELEMENT_NOT_FOUND_ERROR;
}

bool vector_exists(Vector *vector, void *element){
	return vector ? vector_indexof(vector, element) >= 0 : false;
}

bool vector_isempty(Vector *vector){
	return vector ? vector->n_elements == 0 : true;
}

void* vector_at(Vector *vector, ptrdiff_t index, void *dest){
	if (!vector || !dest)
		return NULL;
	int status = check_and_transform_index(&index, NULL, vector->n_elements);
	if (status != SUCCESS)
		return NULL;
	void *tmp = void_offset(vector->elements, index * vector->data_size);
	return memcpy(dest, tmp, vector->data_size);
}

void* vector_get(Vector *vector, void *element, void *dest){
	if (!vector || !element || !dest)
		return NULL;
	ptrdiff_t index = vector_indexof(vector, element);
	if (index < 0)
		return NULL;
	return vector_at(vector, index, dest);
}

void* vector_front(Vector *vector, void *dest){
	if (!vector || !dest)
		return NULL;
	if (vector->n_elements > 0)
		return vector_at(vector, 0, dest);
	else
		return NULL;
}

void* vector_back(Vector *vector, void *dest){
	if (!vector || !dest)
		return NULL;
	if (vector->n_elements > 0)
		return vector_at(vector, vector->n_elements - 1, dest);
	else
		return NULL;
}

void* vector_get_into_array(Vector *vector, void *array, size_t array_length){
	if (!vector || !array)
		return NULL;
	if (array_length > vector->n_elements)
		array_length = vector->n_elements;
	memcpy(array, vector->elements, array_length * vector->data_size);
	return array;
}

void* vector_get_array(Vector *vector, size_t array_length){
	if (!vector)
		return NULL;
	if (array_length == 0 || array_length > vector->n_elements){
		array_length = vector->n_elements;
	}
	void *array = malloc(vector->data_size * array_length);
	if (!array){
		return NULL;
	}
	if (!vector_get_into_array(vector, array, array_length)){
		free(array);
		return NULL;
	}
	return array;
}

///////////////////////////////////////////////////////////////////////////////

/// OTHER /////////////////////////////////////////////////////////////////////

int vector_swap(Vector *vector, ptrdiff_t index_1, ptrdiff_t index_2){
	if (!vector)
		return NULL_PARAMETER_ERROR;
	int status = check_and_transform_index(&index_1, &index_2, vector->n_elements);
	if (status != SUCCESS)
		return status;
	void *tmp = malloc(vector->data_size);
	assert(tmp);
	if (!vector_at(vector, index_1, tmp))
		return ERROR;

	void *e1 = void_offset(vector->elements, index_1 * vector->data_size);
	void *e2 = void_offset(vector->elements, index_2 * vector->data_size);
	memmove(e1, e2, vector->data_size);
	memcpy(e2, tmp, vector->data_size);

	free(tmp);
	return SUCCESS;
}

int vector_compare(Vector *vector, ptrdiff_t index_1, ptrdiff_t index_2){
	if (!vector)
		return NULL_PARAMETER_ERROR;
	int status = check_and_transform_index(&index_1, &index_2, vector->n_elements);
	if (status != SUCCESS)
		return status;
	void *e1 = void_offset(vector->elements, index_1 * vector->data_size);
	void *e2 = void_offset(vector->elements, index_2 * vector->data_size);
	return vector->compare(e1, e2);
}

size_t vector_size(Vector *vector){
	return vector ? vector->n_elements : 0;
}

size_t vector_capacity(Vector *vector){
	return vector ? vector->max_elements : 0;
}

int vector_reserve(Vector *vector, size_t n_elements){
	if (!vector)
		return NULL_PARAMETER_ERROR;
	if (vector->max_elements < n_elements)
		vector_resize(vector, n_elements);
	vector->n_elements = n_elements;
	return SUCCESS;
}

void vector_shrink(Vector *vector){
	if (vector)
		vector_resize(vector, vector->n_elements);
}

Vector* vector_dup(Vector *vector){
	if (!vector)
		return NULL;
	Vector *dup = vector_init(vector->data_size, vector->compare);
	vector_set_destructor(dup, vector->destructor);
	vector_reserve(dup, vector->n_elements); // vector_reserve also sets dup->n_elements to vector->n_elements
	memcpy(dup->elements, vector->elements, vector->n_elements * vector->data_size);
	return dup;
}

Vector* vector_join(Vector *vector_1, Vector *vector_2){
	if (!vector_1 || !vector_2 || vector_1->data_size != vector_2->data_size)
		return NULL;
	size_t n_elements = vector_1->n_elements + vector_2->n_elements;
	if (n_elements < VECTOR_DEFAULT_SIZE){
		n_elements = VECTOR_DEFAULT_SIZE;
	}
	Vector *vector_joint = vector_init(vector_1->data_size, vector_1->compare);
	vector_reserve(vector_joint, n_elements);

	int status = vector_append_array(vector_joint, vector_1->elements, vector_1->n_elements);
	if (status != SUCCESS){
		vector_free(vector_joint);
		return NULL;
	}
	status = vector_append_array(vector_joint, vector_2->elements, vector_2->n_elements);
	if (status != SUCCESS){
		vector_free(vector_joint);
		return NULL;
	}
	return vector_joint;
}

///////////////////////////////////////////////////////////////////////////////

/// FREE //////////////////////////////////////////////////////////////////////

static void destroy_content(Vector *vector){
	if (vector->destructor){
		void *tmp = vector->elements;
		for (size_t i = 0; i < vector->n_elements; i++){
			vector->destructor(tmp);
			tmp = void_offset(tmp, vector->data_size);
		}
	}
}

void vector_clear(Vector *vector){
	if (vector){
		destroy_content(vector);
		vector->n_elements = 0;
	}
}

void vector_free(Vector *vector){
	if (!vector)
		return;
	destroy_content(vector);
	free(vector->elements);
	free(vector);
}

void vector_free_all(unsigned int n, ...){
	va_list arg;
	va_start(arg, n);
	for (unsigned int i = 0; i < n; i++){
		Vector *ptr = va_arg(arg, Vector*);
		vector_free(ptr);
	}
	va_end(arg);
}

void vector_reset(Vector *vector){
	if (!vector)
		return;
	destroy_content(vector);
	free(vector->elements);
	vector->elements = malloc(VECTOR_DEFAULT_SIZE * vector->data_size);
	assert(vector->elements);
	vector->n_elements = 0;
	vector->max_elements = VECTOR_DEFAULT_SIZE;
}
