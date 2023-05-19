/**
 *  Copyright (C) 2023 - Saúl Valdelvira
 *  License: BSD 3-Clause
 *  Email: saulvaldelvira@gmail.com
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#define GDS_ENABLE_ERROR_MACROS
#include "./Util/error.h"
#include "./Util/definitions.h"
#include <string.h>
#include <stdarg.h>
#include "Vector.h"

#define VECTOR_DEFAULT_SIZE 12

struct Vector {
	size_t n_elements;
	size_t max_elements;
	size_t data_size;
	// Comparator function for 2 elements
	comparator_function_t compare;
	void *elements;
};

/// INITIALIZE ////////////////////////////////////////////////////////////////

Vector* vector_init(size_t data_size, comparator_function_t cmp){
	if (data_size == 0){
		printerr_data_size();
		return NULL;
	}
	if (!cmp){
		printerr_null_param();
		return NULL;
	}

	Vector *vector = malloc(sizeof(*vector));
	if (!vector){
		printerr_allocation();
		return NULL;
	}

	vector->elements = malloc(VECTOR_DEFAULT_SIZE * data_size);
	if (!vector->elements){
		printerr_allocation();
		free(vector);
		return NULL;
	}

	vector->data_size = data_size;
	vector->n_elements = 0;
	vector->max_elements = VECTOR_DEFAULT_SIZE;
	vector->compare = cmp;
	return vector;
}

void vector_configure(Vector *vector, comparator_function_t cmp){
	if (!vector || !cmp){
		printerr_null_param();
		return;
	}
	vector->compare = cmp;
}

size_t vector_get_data_size(Vector *vector){
	if (!vector){
		printerr_null_param();
		return 0;
	}
	return vector->data_size;
}

comparator_function_t vector_get_comparator(Vector *vector){
	if (!vector){
		printerr_null_param();
		return NULL;
	}
	return vector->compare;
}

///////////////////////////////////////////////////////////////////////////////

/// ADD-SET ///////////////////////////////////////////////////////////////////////

static int vector_resize(Vector *vector, size_t new_size){
	void *tmp = calloc(new_size, vector->data_size);
	if(!tmp){
		printerr_allocation();
		return ALLOCATION_ERROR;
	}
	memcpy(tmp, vector->elements, vector->n_elements * vector->data_size);
	free(vector->elements);
	vector->elements = tmp;
	vector->max_elements = new_size;
	return SUCCESS;
}

int vector_append(Vector *vector, void *element){
	if (!vector || !element){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	if(vector->n_elements >= vector->max_elements){
		// If the vector is full, double the array size
		int status = vector_resize(vector, vector->n_elements * 2);
		if (status != SUCCESS){
			return status;
		}
	}
	void *tmp = void_offset(vector->elements, vector->n_elements * vector->data_size);
	memcpy(tmp , element, vector->data_size);
	vector->n_elements++;
	return SUCCESS;
}

int vector_push_front(Vector *vector, void *element){
	if (!vector || !element){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	if(vector->n_elements >= vector->max_elements){
		int status = vector_resize(vector, vector->max_elements * 2);
		if (status != SUCCESS){
			return status;
		}
	}
	void *tmp = void_offset(vector->elements, vector->data_size);
	memmove(tmp, vector->elements, vector->n_elements * vector->data_size);
	memcpy(vector->elements, element, vector->data_size);
	vector->n_elements++;
	return SUCCESS;
}

int vector_append_array(Vector *vector, void *array, size_t array_length){
	if (!vector || !array){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	if (vector->n_elements + array_length > vector->max_elements){
		size_t new_size = (vector->max_elements + array_length) * 2;
		int status = vector_resize(vector, new_size);
		if (status != SUCCESS){
			return status;
		}
	}
	void *tmp = void_offset(vector->elements, vector->n_elements * vector->data_size);
	memcpy(tmp, array, array_length * vector->data_size);
	vector->n_elements += array_length;
	return SUCCESS;
}

int vector_push_front_array(Vector *vector, void *array, size_t array_length){
	if (!vector || !array){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	if (vector->n_elements + array_length > vector->max_elements){
		size_t new_size = (vector->max_elements + array_length) * 2;
		int status = vector_resize(vector, new_size);
		if (status != SUCCESS){
			return status;
		}
	}
	void *tmp = void_offset(vector->elements, array_length * vector->data_size);
	memmove(tmp, vector->elements, vector->n_elements * vector->data_size);
	memcpy(vector->elements, array, array_length * vector->data_size);
	vector->n_elements += array_length;
	return SUCCESS;
}

int vector_set_at(Vector *vector, size_t index, void *element){
	if (!vector || !element){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	if (index >= vector->n_elements){
		printerr_out_of_bounds(index, vector->n_elements-1);
		return INDEX_BOUNDS_ERROR;
	}
	void *tmp = void_offset(vector->elements, index * vector->data_size);
	memmove(tmp, element , vector->data_size);
	return SUCCESS;
}

int vector_set(Vector *vector, void *element, void *replacement){
	if (!vector || !element || !replacement){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	void *ptr;
	for (size_t i=0; i < vector->n_elements; i++){
		ptr = void_offset(vector->elements, i * vector->data_size);
		if (vector->compare(ptr, element) == 0){
			memmove(ptr, replacement, vector->data_size);
			return SUCCESS;
		}
	}
	return ELEMENT_NOT_FOUND_ERROR;
}

int vector_insert(Vector *vector, void *element, void *insert){
	if (!vector || !element || !insert){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	index_t index = vector_indexof(vector, element);
	if (index.status != SUCCESS){
		return index.status;
	}
	return vector_insert_at(vector, index.value, insert);
}

int vector_insert_at(Vector *vector, size_t index, void *element){
	if (!vector || !element){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	if (vector->n_elements == vector->max_elements){
		int status = vector_resize(vector, vector->max_elements * 2);
		if (status != SUCCESS){
			return status;
		}
	}
	void *src = void_offset(vector->elements, index * vector->data_size);
	void *dst = void_offset(vector->elements, (index + 1) * vector->data_size);
	int n = vector->n_elements - index; // number of elements to shift
	memmove(dst, src, n * vector->data_size); // Shift elements to the right
	memcpy(src, element, vector->data_size); // Insert the element
	vector->n_elements++;
	return SUCCESS;
}

int vector_populate(Vector *vector, void *template){
	if (!vector || !template){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	void *tmp = vector->elements;
	for (size_t i = 0; i < vector->max_elements; i++){
		memcpy(tmp, template, vector->data_size);
		tmp = void_offset(tmp, vector->data_size);
	}
	vector->n_elements = vector->max_elements;
	return SUCCESS;
}

int vector_process(Vector *vector, int (*func) (void *,void*), void *args){
	if (!vector || !func){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	void *tmp = vector->elements;
	for (size_t i = 0; i < vector->n_elements; ++i){
		int status = func(tmp, args);
		if (status != SUCCESS){
			printerr("Failed to process vector, at element %p",, tmp);
			return status;
		}
		tmp = void_offset(tmp, vector->data_size);
	}
	return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

/// REMOVE ////////////////////////////////////////////////////////////////////

int vector_remove_at(Vector *vector, size_t index){
	if (!vector){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	if (index >= vector->n_elements){
		printerr_out_of_bounds(index, vector->n_elements-1);
		return INDEX_BOUNDS_ERROR;
	}

	if (index < vector->n_elements - 1){
		size_t leftover = (vector->n_elements - index - 1) * vector->data_size;
		void *dst = void_offset(vector->elements, index * vector->data_size);
		void *src = void_offset(vector->elements, (index+1) * vector->data_size);
		memmove(dst, src, leftover);
	}
	vector->n_elements--;
	return SUCCESS;
}

int vector_remove(Vector *vector, void *element){
	if (!vector || !element){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	index_t i = vector_indexof(vector, element);
	if (!i.status){
		return i.status;
	}
	return vector_remove_at(vector, i.value);
}

void* vector_pop_front(Vector *vector, void *dest){
	if (!vector || !dest){
		printerr_null_param();
		return NULL;
	}
	if (vector->n_elements == 0){
		return NULL;
	}
	memcpy(dest, vector->elements, vector->data_size);
	vector_remove_at(vector, 0);
	return dest;
}

void* vector_pop_back(Vector *vector, void *dest){
	if (!vector || !dest){
		printerr_null_param();
		return NULL;
	}
	if (vector->n_elements == 0){
		return NULL;
	}
	void *src = void_offset(vector->elements, (vector->n_elements - 1) * vector->data_size);
	memcpy(dest, src, vector->data_size);
	vector_remove_at(vector, vector->n_elements - 1);
	return dest;
}

int vector_remove_array(Vector *vector, void *array, size_t array_length){
	if (!vector || !array){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	void *tmp;
	int status;
	for (size_t i = 0; i < array_length; i++){
		tmp = void_offset(array, i * vector->data_size);
		status = vector_remove(vector, tmp);
		if (status != SUCCESS){
			return status;
		}
	}
	return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

/// SEARCH-GET ////////////////////////////////////////////////////////////////

index_t vector_indexof(Vector *vector, void *element){
	if (!vector || !element){
		printerr_null_param();
		return index_t(0,NULL_PARAMETER_ERROR);
	}

	void *ptr = vector->elements; // Current element in the iteration
	for (size_t i = 0; i < vector->n_elements; i++){
		if ((*vector->compare) (ptr, element) == 0){
			return index_t(i,SUCCESS);
		}
		ptr = void_offset(ptr, vector->data_size);
	}
	return index_t(0,ELEMENT_NOT_FOUND_ERROR);
}

bool vector_exists(Vector *vector, void *element){
	if (!vector){
		printerr_null_param();
		return false;
	}
	return vector_indexof(vector, element).status == SUCCESS;
}

bool vector_isempty(Vector *vector){
	if (!vector){
		printerr_null_param();
		return false;
	}
	return vector->n_elements == 0;
}

void* vector_get_at(Vector *vector, size_t index, void *dest){
	if (!vector || !dest){
		printerr_null_param();
		return NULL;
	}
	if (index >= vector->n_elements){
		printerr_out_of_bounds(index, vector->n_elements-1);
		return NULL;
	}
	void *tmp = void_offset(vector->elements, index * vector->data_size);
	tmp = memcpy(dest, tmp, vector->data_size);
	return tmp;
}

void* vector_get(Vector *vector, void *element, void *dest){
	if (!vector || !element || !dest){
		printerr_null_param();
		return NULL;
	}
	index_t index = vector_indexof(vector, element);
	if (!index.status){
		return NULL;
	}
	return vector_get_at(vector, index.value, dest);
}

void* vector_get_front(Vector *vector, void *dest){
	if (!vector || !dest){
		printerr_null_param();
		return NULL;
	}
	return vector_get_at(vector, 0, dest);
}

void* vector_get_back(Vector *vector, void *dest){
	if (!vector || !dest){
		printerr_null_param();
		return NULL;
	}
	return vector_get_at(vector, vector->n_elements - 1, dest);
}

void* vector_get_into_array(Vector *vector, void *array, size_t array_length){
	if (!vector || !array){
		printerr_null_param();
		return NULL;
	}
	if (array_length > vector->n_elements){
		array_length = vector->n_elements;
	}
	memcpy(array, vector->elements, array_length * vector->data_size);
	return array;
}

void* vector_get_array(Vector *vector, size_t array_length){
	if (!vector){
		printerr_null_param();
		return NULL;
	}
	if (array_length == GET_ALL_ELEMENTS || array_length > vector->n_elements){
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

static void* get_position(Vector *vector, size_t index){
	return void_offset(vector->elements, index * vector->data_size);
}

int vector_swap(Vector *vector, size_t index_1, size_t index_2){
	if (!vector){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	if (index_1 >= vector->n_elements){
		printerr_out_of_bounds(index_1, vector->n_elements-1);
		return INDEX_BOUNDS_ERROR;
	}
	if (index_2 >= vector->n_elements){
		printerr_out_of_bounds(index_2, vector->n_elements-1);
		return INDEX_BOUNDS_ERROR;
	}

	void *tmp = malloc(vector->data_size);
	if (!tmp){
		printerr_allocation();
		return ALLOCATION_ERROR;
	}

	if (!vector_get_at(vector, index_1, tmp)){
		return ERROR;
	}

	void *e1 = get_position(vector, index_1);
	void *e2 = get_position(vector, index_2);
	memcpy(e1, e2, vector->data_size);
	memcpy(e2, tmp, vector->data_size);

	free(tmp);
	return SUCCESS;
}

int vector_compare(Vector *vector, size_t index_1, size_t index_2){
	if (!vector){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	void *e1 = get_position(vector, index_1);
	void *e2 = get_position(vector, index_2);
	return (*vector->compare) (e1, e2);
}

size_t vector_size(Vector *vector){
	if (!vector){
		printerr_null_param();
		return false;
	}
	return vector->n_elements;
}

int vector_reserve(Vector *vector, size_t n_elements){
	if (!vector){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	if (vector->max_elements >= n_elements){
		vector->n_elements = n_elements;
		return SUCCESS;
	}
	int status = vector_resize(vector, n_elements);
	if (status == SUCCESS){
		vector->n_elements = n_elements;
	}
	return status;
}

Vector* vector_join(Vector *vector_1, Vector *vector_2){
	if (!vector_1 || !vector_2){
		printerr_null_param();
		return NULL;
	}
	if (vector_1->data_size != vector_2->data_size){
		printerr("The vectors have different data sizes (%zu and %zu)",, vector_1->data_size, vector_2->data_size);
		return NULL;
	}

	size_t n_elements = vector_1->n_elements + vector_2->n_elements;
	if (n_elements < VECTOR_DEFAULT_SIZE){
		n_elements = VECTOR_DEFAULT_SIZE;
	}
	Vector *vector_joint = vector_init(vector_1->data_size, vector_1->compare);
	if (!vector_joint){
		return NULL;
	}
	vector_reserve(vector_joint, n_elements);

	int status;

	// Get the elements of the first vector
	void *tmp = vector_get_array(vector_1, vector_1->n_elements);
	if (tmp != NULL){
		// Add the elements of the first vector
		status = vector_append_array(vector_joint, tmp, vector_1->n_elements);
		free(tmp);
		if (status != SUCCESS){
			goto exit_err;
		}
	}

	// Get the elements of the second vector
	tmp = vector_get_array(vector_2, vector_2->n_elements);
	if (tmp != NULL){
		// Add the elements of the second vector
		status = vector_append_array(vector_joint, tmp, vector_2->n_elements);
		free(tmp);
		if (status != SUCCESS){
			exit_err:
			free(vector_joint);
			return NULL;
		}
	}

	return vector_joint;
}

///////////////////////////////////////////////////////////////////////////////

/// FREE //////////////////////////////////////////////////////////////////////

int vector_free(Vector *vector){
	if (!vector){
		printerr_null_param();
		return NULL_PARAMETER_ERROR;
	}
	free(vector->elements);
	free(vector);
	return SUCCESS;
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

Vector* vector_reset(Vector *vector){
	if (!vector){
		printerr_null_param();
		return NULL;
	}
	free(vector->elements);
	vector->elements = malloc(VECTOR_DEFAULT_SIZE * vector->data_size);

	if (!vector->elements){
		printerr_allocation();
		return NULL;
	}

	vector->n_elements = 0;
	vector->max_elements = VECTOR_DEFAULT_SIZE;
	return vector;
}
