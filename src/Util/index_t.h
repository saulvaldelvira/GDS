#pragma once
#ifndef INDEX_T_H
#define INDEX_T_H

/**
 * Represents a position on the list. Used in methods to return a value alongside a status identifier of the operation result.
*/
typedef struct index{
	size_t value;
	int status;
} index_t;

#define index_t(i,s)    (index_t) {.value = i, .status = s}

#endif
