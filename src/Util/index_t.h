/**
 *  Defines the index_t struct, to represent an index.
 *
 *  Copyright (C) 2023 - Saúl Valdelvira
 *
 *  This program is free software. You can modify
 *  and/or redistribute it under the terms of the GNU
 *  General Public License version 3, or any later version.
 *  See <https://www.gnu.org/licenses/>.
 *
 *  Email: saulvaldelvira@gmail.com
 *  Version: 11-01-2023
 */
#pragma once
#ifndef INDEX_T_H
#define INDEX_T_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Represents a position.
 * This structure is used when an index must be returned by a function. Since these strcutures use
 * size_t (long unsigned int) we must wrap this index along an integer that represent if the value
 * is actually useful or else the operation has failed.
 * If status inidcates success, the value is valid. Else, it is just garbage.
*/
typedef struct index{
	size_t value;
	int status;
} index_t;

#define index_t(i,s)    (index_t) {.value = i, .status = s}

#ifdef __cplusplus
}
#endif

#endif
