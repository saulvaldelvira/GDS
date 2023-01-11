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

#define index_t(i)    (index_t) {.value = i, .status = 1}
#define INDEXT_NOT_FOUND (index_t) {.value = 0, -1} // An index_t struct with status set to -1, to represent a failure in the procedure

#endif
