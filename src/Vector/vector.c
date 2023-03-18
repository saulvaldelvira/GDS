/**
 *  Copyright (C) 2023 - Saúl Valdelvira
 *
 *  This program is free software. You can modify
 *  and/or redistribute it under the terms of the
 *  GNU General Public License version 2, or newer.
 *  See <https://www.gnu.org/licenses/>.
 *
 *  Email: saulvaldelvira@gmail.com
 *  Version: 17-01-2023
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Util/error.h"
#include "../Util/definitions.h"
#include <string.h>

#include "vector.h"
#define VECTOR_DEFAULT_SIZE 12

struct _Vector {
	size_t n_elements;
	size_t max_elements;
	size_t data_size;
	// Comparator function for 2 elements
	comparator_function_t compare;
	void *elements;
};

/// INITIALIZE ////////////////////////////////////////////////////////////////

Vector* vector_empty(size_t data_size, comparator_function_t cmp){
	if (data_size == 0){
		printerr_data_size(vector_empty);
		return NULL;
	}
	if (!cmp){
		printerr_null_param(vector_empty);
		return NULL;
	}
	return vector_init(data_size, VECTOR_DEFAULT_SIZE, cmp);
}

Vector* vector_init(size_t data_size, size_t max_elements, comparator_function_t cmp){
	if (data_size == 0){
		printerr_data_size(vector_init);
		return NULL;
	}
	if (!cmp){
		printerr_null_param(vector_init);
		return NULL;
	}

	Vector *vector = malloc(sizeof(*vector));
	void *elements = malloc(max_elements * data_size);

	if (!vector || !elements){
		printerr_allocation(vector_init);
		return NULL;
	}

	vector->elements = elements;
	vector->data_size = data_size;
	vector->n_elements = 0;
	vector->max_elements = max_elements;
	vector->compare = cmp;
	return vector;
}

void vector_configure(Vector *vector, comparator_function_t cmp){
	if (!vector || !cmp){
		printerr_null_param(vector_configure);
		return;
	}
	vector->compare = cmp;
}

size_t vector_get_data_size(Vector *vector){
	if (!vector){
		printerr_null_param(vector_data_size);
		return 0;
	}
	return vector->data_size;
}

comparator_function_t vector_get_comparator(Vector *vector){
	if (!vector){
		printerr_null_param(vector_get_comparator);
		return NULL;
	}
	return vector->compare;
}

///////////////////////////////////////////////////////////////////////////////

/// ADD-SET ///////////////////////////////////////////////////////////////////////

static int vector_resize(Vector *vector, size_t new_size){
	vector->max_elements = new_size;
	void *tmp = realloc(vector->elements, vector->max_elements * vector->data_size);
	if(!tmp){
		printerr_allocation(vector_resize);
		free(vector->elements);
		return ALLOCATION_ERROR;
	}
	vector->elements = tmp;
	return SUCCESS;
}

int vector_append(Vector *vector, void *element){
	if (!vector || !element){
		printerr_null_param(vector_append);
		return NULL_PARAMETER_ERROR;
	}
	if(vector->n_elements >= vector->max_elements){
		// If the vector is empty, double the array size
		int status = vector_resize(vector, vector->n_elements * 2);
		if (status != SUCCESS){
			return status;
		}
	}

	void *tmp = void_offset(vector->elements, vector->n_elements * vector->data_size);
	tmp = memmove(tmp , element, vector->data_size);
	if (!tmp){
		printerr_memory_op(vector_append);
		return MEMORY_OP_ERROR;
	}

	vector->n_elements++;
	return SUCCESS;
}

int vector_push_front(Vector *vector, void *element){
	if (!vector || !element){
		printerr_null_param(vector_push_front);
		return NULL_PARAMETER_ERROR;
	}
	if(vector->n_elements >= vector->max_elements){ // If the vector is empty, double the array size
		int status = vector_resize(vector, vector->max_elements * 2);
		if (status != SUCCESS){
			return status;
		}
	}

	void *tmp = void_offset(vector->elements, vector->data_size);
	tmp = memmove(tmp, vector->elements, vector->n_elements * vector->data_size);
	void *dst = memcpy(vector->elements, element, vector->data_size);
	if (!tmp || !dst){
		printerr_memory_op(vector_push_front);
		return MEMORY_OP_ERROR;
	}
	vector->n_elements++;
	return SUCCESS;
}

int vector_append_array(Vector *vector, void *array, size_t array_length){
	if (!vector || !array){
		printerr_null_param(vector_append_array);
		return NULL_PARAMETER_ERROR;
	}

	if (vector->n_elements + array_length > vector->max_elements){
		int status = vector_resize(vector, (vector->max_elements + array_length) * 2);
		if (status != SUCCESS){
			return status;
		}
	}
	void *tmp = void_offset(vector->elements, vector->n_elements * vector->data_size);
	tmp = memcpy(tmp, array, array_length * vector->data_size);
	if (!tmp){
		printerr_memory_op(vector_append_array);
		return MEMORY_OP_ERROR;
	}
	vector->n_elements += array_length;

	return SUCCESS;
}

int vector_push_front_array(Vector *vector, void *array, size_t array_length){
	if (!vector || !array){
		printerr_null_param(vector_push_front_array);
		return NULL_PARAMETER_ERROR;
	}
	if (vector->n_elements + array_length > vector->max_elements){
		int status = vector_resize(vector, (vector->max_elements + array_length) * 2);
		if (status != SUCCESS){
			return status;
		}
	}
	void *tmp = void_offset(vector->elements, array_length * vector->data_size);
	tmp = memmove(tmp, vector->elements, vector->n_elements * vector->data_size);
	if (!tmp){
		printerr_memory_op(vector_push_front_array);
		return MEMORY_OP_ERROR;
	}
	tmp = memmove(vector->elements, array, array_length * vector->data_size);
	if (!tmp){
		printerr_memory_op(vector_push_front_array);
		return MEMORY_OP_ERROR;
	}
	vector->n_elements += array_length;
	return SUCCESS;
}

int vector_set_at(Vector *vector, size_t index, void *element){
	if (!vector || !element){
		printerr_null_param(vector_set_at);
		return NULL_PARAMETER_ERROR;
	}
	if (index >= vector->n_elements){
		printerr_out_of_bounds(index, vector_set_at, vector->n_elements);
		return INDEX_BOUNDS_ERROR;
	}
	void *tmp = void_offset(vector->elements, index * vector->data_size);
	tmp = memmove(tmp, element , vector->data_size);
	if(!tmp){
		printerr_memory_op(vector_set_at);
		return MEMORY_OP_ERROR;
	}
	return SUCCESS;
}

int vector_set(Vector *vector, void *element, void *replacement){
	if (!vector || !element || !replacement){
		printerr_null_param(vector_set);
		return NULL_PARAMETER_ERROR;
	}
	void *ptr;
	for (size_t i=0; i < vector->n_elements; i++){
		ptr = void_offset(vector->elements, i * vector->data_size);
		if ((*vector->compare) (ptr, element) == 0){
			ptr = memmove(ptr, replacement, vector->data_size);
			if(ptr){
				return SUCCESS;
			}else{
				printerr_memory_op(vector_set);
				return MEMORY_OP_ERROR;
			}
		}
	}
	return ELEMENT_NOT_FOUND_ERROR;
}

int vector_insert(Vector *vector, void *element, void *insert){
	if (!vector || !element || !insert){
		printerr_null_param(vector_insert_at);
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
		printerr_null_param(vector_insert_at);
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
	dst = memmove(dst, src, n * vector->data_size); // Shift elements to the right
	src = memcpy(src, element, vector->data_size); // Insert the element
	if (!dst || !src){
		printerr_memory_op(vector_insert_at);
		return MEMORY_OP_ERROR;
	}
	vector->n_elements++;
	return SUCCESS;
}

int vector_populate(Vector *vector, void *template){
	if (!vector || !template){
		printerr_null_param(vector_populate);
		return NULL_PARAMETER_ERROR;
	}
	void *tmp = vector->elements;
	for (size_t i = 0; i < vector->max_elements; i++){
		tmp = memcpy(tmp, template, vector->data_size);
		if (!tmp){
			printerr_memory_op(vector_populate);
			return MEMORY_OP_ERROR;
		}
		tmp = void_offset(tmp, vector->data_size);
	}
	vector->n_elements = vector->max_elements;
	return SUCCESS;
}

int vector_process(Vector *vector, int (*func) (void *,void*), void *args){
	if (!vector || !func){
		printerr_null_param(vector_process);
		return NULL_PARAMETER_ERROR;
	}
	void *tmp = vector->elements;
	for (size_t i = 0; i < vector->n_elements; ++i){
		int status = func(tmp, args);
		if (status != SUCCESS){
			printerr(vector_process, "Failed to process vector, at element %p",, tmp);
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
		printerr_null_param(vector_remove_at);
		return NULL_PARAMETER_ERROR;
	}
	if (index >= vector->n_elements){
		printerr_out_of_bounds(index, vector_remove_at, vector->n_elements);
		return INDEX_BOUNDS_ERROR;
	}

	if (index < vector->n_elements - 1){
		size_t leftover = (vector->n_elements - index - 1) * vector->data_size;
		void *dst = void_offset(vector->elements, index * vector->data_size);
		void *src = void_offset(vector->elements, (index+1) * vector->data_size);
		if(!memmove(dst, src, leftover)){
			printerr_memory_op(vector_remove_at);
			return MEMORY_OP_ERROR;
		}
	}
	vector->n_elements--;
	return SUCCESS;
}

int vector_remove(Vector *vector, void *element){
	if (!vector || !element){
		printerr_null_param(vector_remove);
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
		printerr_null_param(vector_pop_front);
		return NULL;
	}
	if (vector->n_elements == 0){
		return NULL;
	}
	dest = memcpy(dest, vector->elements, vector->data_size);
	if (!dest){
		printerr_memory_op(vector_pop_front);
		return NULL;
	}
	vector_remove_at(vector, 0);
	return dest;
}

void* vector_pop_back(Vector *vector, void *dest){
	if (!vector || !dest){
		printerr_null_param(vector_pop_back);
		return NULL;
	}
	if (vector->n_elements == 0){
		return NULL;
	}
	void *src = void_offset(vector->elements, (vector->n_elements - 1) * vector->data_size);
	dest = memcpy(dest, src, vector->data_size);
	if (!dest){
		printerr_memory_op(vector_pop_front);
		return NULL;
	}
	vector_remove_at(vector, vector->n_elements - 1);
	return dest;
}

int vector_remove_array(Vector *vector, void *array, size_t array_length){
	if (!vector || !array){
		printerr_null_param(vector_remove_array);
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
		printerr_null_param(vector_indexof);
		return index_t(0,NULL_PARAMETER_ERROR);
	}

	void *ptr; // Current element in the iteration
	for (size_t i=0; i < vector->n_elements; i++){
		ptr = void_offset(vector->elements, i * vector->data_size);
		if ((*vector->compare) (ptr, element) == 0){
			return index_t(i,SUCCESS);
		}
	}
	printerr(vector_indexof, "The element %p does not exists",, element);
	return index_t(0,ELEMENT_NOT_FOUND_ERROR);
}

bool vector_exists(Vector *vector, void *element){
	if (!vector){
		printerr_null_param(vector_exists);
		return false;
	}
	return vector_indexof(vector, element).status == SUCCESS;
}

bool vector_isempty(Vector *vector){
	if (!vector){
		printerr_null_param(vector_isempty);
		return false;
	}
	return vector->n_elements == 0;
}

void* vector_get_at(Vector *vector, size_t index, void *dest){
	if (!vector || !dest){
		printerr_null_param(vector_get_at);
		return NULL;
	}
	if (index >= vector->n_elements){
		printerr_out_of_bounds(index, vector_get_at, vector->n_elements);
		return NULL;
	}
	void *tmp = void_offset(vector->elements, index * vector->data_size);
	tmp = memcpy(dest, tmp, vector->data_size);
	return tmp;
}

void* vector_get(Vector *vector, void *element, void *dest){
	if (!vector || !element || !dest){
		printerr_null_param(vector_get);
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
		printerr_null_param(vector_get_front);
		return NULL;
	}
	return vector_get_at(vector, 0, dest);
}

void* vector_get_back(Vector *vector, void *dest){
	if (!vector || !dest){
		printerr_null_param(vector_get_back);
		return NULL;
	}
	return vector_get_at(vector, vector->n_elements - 1, dest);
}

void* vector_get_into_array(Vector *vector, void *array, size_t array_length){
	if (!vector || !array){
		printerr_null_param(vector_get_into_array);
		return NULL;
	}
	if (array_length > vector->n_elements){
		array_length = vector->n_elements;
	}
	if (!memcpy(array, vector->elements, array_length * vector->data_size)){
		printerr_memory_op(vector_get_into_array);
		return NULL;
	}

	return array;
}

void* vector_get_array(Vector *vector, size_t array_length){
	if (!vector){
		printerr_null_param(vector_get_array);
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
		printerr_null_param(vector_swap);
		return NULL_PARAMETER_ERROR;
	}
	if (index_1 >= vector->n_elements){
		printerr_out_of_bounds(index_1, vector_swap, vector->n_elements);
		return INDEX_BOUNDS_ERROR;
	}
	if (index_2 >= vector->n_elements){
		printerr_out_of_bounds(index_2, vector_swap, vector->n_elements);
		return INDEX_BOUNDS_ERROR;
	}

	void *tmp = malloc(vector->data_size);
	if (!tmp){
		printerr_allocation(vector_swap);
		return ALLOCATION_ERROR;
	}

	if (!vector_get_at(vector, index_1, tmp)){
		return ERROR;
	}

	void *e1 = get_position(vector, index_1);
	void *e2 = get_position(vector, index_2);
	if(!memcpy(e1, e2, vector->data_size)){
		printerr_memory_op(vector_swap);
		return MEMORY_OP_ERROR;
	}

	if(!memcpy(e2, tmp, vector->data_size)){
		printerr_memory_op(vector_swap);
		return MEMORY_OP_ERROR;
	}

	free(tmp);

	return SUCCESS;

}

int vector_compare(Vector *vector, size_t index_1, size_t index_2){
	if (!vector){
		printerr_null_param(vector_swap);
		return NULL_PARAMETER_ERROR;
	}
	void *e1 = get_position(vector, index_1);
	void *e2 = get_position(vector, index_2);
	return (*vector->compare) (e1, e2);
}

size_t vector_size(Vector *vector){
	if (!vector){
		printerr_null_param(vector_size);
		return false;
	}
	return vector->n_elements;
}

int vector_reserve(Vector *vector, size_t n_elements){
	if (!vector){
		printerr_null_param(vector_reserve);
		return NULL_PARAMETER_ERROR;
	}
	vector->n_elements = n_elements;
	if (vector->max_elements >= n_elements){
		return SUCCESS;
	}
	return vector_resize(vector, n_elements);
}

Vector* vector_join(Vector *vector_1, Vector *vector_2){
	if (!vector_1 || !vector_2){
		printerr_null_param(vector_join);
		return NULL;
	}
	if (vector_1->data_size != vector_2->data_size){
		fprintf(stderr, "ERROR: the vectors have different data sizes. In function vector_join\n");
		return NULL;
	}

	size_t n_elements = vector_1->n_elements + vector_2->n_elements;
	if (n_elements < VECTOR_DEFAULT_SIZE){
		n_elements = VECTOR_DEFAULT_SIZE;
	}
	Vector *vector_joint = vector_init(vector_1->data_size, n_elements, vector_1->compare);
	if (!vector_joint){
		return NULL;
	}

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
		printerr_null_param(vector_free);
		return NULL_PARAMETER_ERROR;
	}
	free(vector->elements);
	free(vector);
	return SUCCESS;
}

Vector* vector_reset(Vector *vector){
	if (!vector){
		printerr_null_param(vector_reset);
		return NULL;
	}
	free(vector->elements);
	vector->elements = malloc(VECTOR_DEFAULT_SIZE * vector->data_size);

	if (!vector->elements){
		printerr_allocation(vector_reset);
		return NULL;
	}

	vector->n_elements = 0;
	vector->max_elements = VECTOR_DEFAULT_SIZE;
	return vector;
}
