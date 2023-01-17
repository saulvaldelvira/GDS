/**
 *  Copyright (C) 2022-2023  Saúl Valdelvira Iglesias
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *  Email: saulvaldelvira@gmail.com
 *  Version: 14-01-2023
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Util/error.h"
#include "../Util/definitions.h"
#include <memory.h>

#include "array_list.h"

struct _ArrayList {
	size_t n_elements;
	size_t max_elements;
	size_t data_size;
	// Comparator function for 2 elements
	comparator_function_t compare;
	void *elements;
};


ArrayList* arrlist_empty(size_t data_size, comparator_function_t cmp){
	if (data_size <= 0){
		printerr_data_size(arrlist_empty);
		return NULL;
	}
	return arrlist_init(data_size, ARRAY_LIST_DEFAULT_SIZE, cmp);
}

ArrayList* arrlist_init(size_t data_size, size_t max_elements, comparator_function_t cmp){
	if (data_size <= 0){
		printerr_data_size(arrlist_init);
		return NULL;
	}
	if (!cmp){
		printerr_null_param(arrlist_init);
		return NULL;
	}

	ArrayList *list = malloc(sizeof(*list));
	void *elements = malloc(ARRAY_LIST_DEFAULT_SIZE * data_size);

	if (!list || !elements){
		printerr_allocation(arrlist_init);
		return NULL;
	}

	list->elements = elements;
	list->data_size = data_size;
	list->n_elements = 0;
	list->max_elements = max_elements;
	list->compare = cmp;
	return list;
}

int arrlist_append(ArrayList *list, void *element){
	if (!list || !element){
		printerr_null_param(arrlist_append);
		return NULL_PARAMETER_ERROR;
	}
	if(list->n_elements >= list->max_elements){ // If the list is empty, double the array size
		list->max_elements *= 2;
		list->elements = realloc(list->elements, list->max_elements * list->data_size);
		if(!list->elements){
			printerr_allocation(arrlist_append);
			return ALLOCATION_ERROR;
		}
	}

	void *tmp = void_offset(list->elements, list->n_elements * list->data_size);
	tmp = memmove(tmp , element, list->data_size);
	if (!tmp){
		printerr_memory_op(arrlist_append);
		return MEMORY_OP_ERROR;
	}

	list->n_elements++;
	return SUCCESS;
}

int arrlist_append_array(ArrayList *list, void *array, size_t array_length){
	if (!list || !array){
		printerr_null_param(arrlist_append_array);
		return NULL_PARAMETER_ERROR;
	}
	void *tmp;
	int status;
	for (size_t i = 0; i < array_length; i++){
		tmp = void_offset(array, i * list->data_size);
		status = arrlist_append(list, tmp);
		if (status != SUCCESS){
			return status;
		}
	}
	return SUCCESS;
}

index_t arrlist_indexof(ArrayList *list, void *element){
	if (!list || !element){
		printerr_null_param(arrlist_indexof);
		return index_t(0,NULL_PARAMETER_ERROR);
	}

	void *ptr; // Current element in the iteration
	for (size_t i=0; i < list->n_elements; i++){
		ptr = void_offset(list->elements, i * list->data_size);
		if ((*list->compare) (ptr, element) == 0){
			return index_t(i,SUCCESS);
		}
	}
	return index_t(0,ELEMENT_NOT_FOUND_ERROR);
}

bool arrlist_exists(ArrayList *list, void *element){
	if (!list){
		printerr_null_param(arrlist_exists);
		return false;
	}
	return arrlist_indexof(list, element).status == SUCCESS;
}

bool arrlist_isempty(ArrayList *list){
	if (!list){
		printerr_null_param(arrlist_isempty);
		return false;
	}
	return list->n_elements == 0;
}

size_t arrlist_size(ArrayList *list){
	if (!list){
		printerr_null_param(arrlist_size);
		return false;
	}
	return list->n_elements;
}

int arrlist_set_at(ArrayList *list, size_t index, void *element){
	if (!list || !element){
		printerr_null_param(arrlist_set_at);
		return NULL_PARAMETER_ERROR;
	}
	if (index >= list->n_elements){
		printerr_out_of_bounds(index, arrlist_set_at);
		return INDEX_BOUNDS_ERROR;
	}

	void *tmp = void_offset(list->elements, index * list->data_size);
	tmp = memmove(tmp, element , list->data_size);
	if(!tmp){
		printerr_memory_op(arrlist_set_at);
		return MEMORY_OP_ERROR;
	}
	return SUCCESS;
}

int arrlist_set(ArrayList *list, void *element, void *replacement){
	if (!list || !element || !replacement){
		printerr_null_param(arrlist_set);
		return NULL_PARAMETER_ERROR;
	}
	void *ptr;
	for (size_t i=0; i < list->n_elements; i++){
		ptr = void_offset(list->elements, i * list->data_size);
		if ((*list->compare) (ptr, element) == 0){
			ptr = memmove(ptr, replacement, list->data_size);
			if(ptr){
				return SUCCESS;
			}else{
				printerr_memory_op(arrlist_set);
				return MEMORY_OP_ERROR;
			}
		}
	}
	return ELEMENT_NOT_FOUND_ERROR;
}

void* arrlist_get_at(ArrayList *list, size_t index, void *dest){
	if (!list || !dest){
		printerr_null_param(arrlist_get_at);
		return NULL;
	}
	if (index >= list->n_elements){
		printerr_out_of_bounds(index, arrlist_get_at);
		return NULL;
	}
	void *tmp = void_offset(list->elements, index * list->data_size);
	tmp = memcpy(dest, tmp, list->data_size);
	return tmp;
}

void* arrlist_get(ArrayList *list, void *element, void *dest){
	if (!list || !element || !dest){
		printerr_null_param(arrlist_get);
		return NULL;
	}
	index_t index = arrlist_indexof(list, element);
	if (!index.status){
		return NULL;
	}
	return arrlist_get_at(list, index.value, dest);
}

int arrlist_remove_at(ArrayList *list, size_t index){
	if (!list){
		printerr_null_param(arrlist_remove_at);
		return NULL_PARAMETER_ERROR;
	}
	if (index >= list->n_elements){
		printerr_out_of_bounds(index, arrlist_remove_at);
		return INDEX_BOUNDS_ERROR;
	}

	if (index < list->n_elements - 1){
		size_t leftover = (list->n_elements - index - 1) * list->data_size;
		void *dst = void_offset(list->elements, index * list->data_size);
		void *src = void_offset(list->elements, (index+1) * list->data_size);
		if(!memmove(dst, src, leftover)){
			printerr_memory_op(arrlist_remove_at);
			return MEMORY_OP_ERROR;
		}
	}
	list->n_elements--;
	return SUCCESS;
}

int arrlist_remove(ArrayList *list, void *element){
	if (!list || !element){
		printerr_null_param(arrlist_remove);
		return NULL_PARAMETER_ERROR;
	}
	index_t i = arrlist_indexof(list, element);
	if (!i.status){
		return i.status;
	}
	return arrlist_remove_at(list, i.value);
}

int arrlist_remove_array(ArrayList *list, void *array, size_t array_length){
	if (!list || !array){
		printerr_null_param(arrlist_remove_array);
		return NULL_PARAMETER_ERROR;
	}
	void *tmp;
	int status;
	for (size_t i = 0; i < array_length; i++){
		tmp = void_offset(array, i * list->data_size);
		status = arrlist_remove(list, tmp);
		if (status != SUCCESS){
			return status;
		}
	}
	return SUCCESS;
}

int arrlist_free(ArrayList *list){
	if (!list){
		printerr_null_param(arrlist_free);
		return NULL_PARAMETER_ERROR;
	}
	free(list->elements);
	free(list);
	return SUCCESS;
}

ArrayList* arrlist_reset(ArrayList *list){
	if (!list){
		printerr_null_param(arrlist_reset);
		return NULL;
	}
	free(list->elements);
	list->elements = NULL;
	list->elements = malloc(ARRAY_LIST_DEFAULT_SIZE * list->data_size);

	if (!list->elements){
		printerr_allocation(arrlist_reset);
		return NULL;
	}

	list->n_elements = 0;
	list->max_elements = ARRAY_LIST_DEFAULT_SIZE;
	return list;
}
