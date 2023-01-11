/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 11-01-2023
 * Copyright: Saúl Valdelvira Iglesias (2023)
 * Licensed under the GNU GPL V.3. See /LICENSE file for more info
*/
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
