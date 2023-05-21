/**
 *  Comparator header.
 *  Defines functions to compare different data types.
 *
 *  Copyright (C) 2023 - Saúl Valdelvira
 *  License: BSD 3-Clause
 *  Email: saulvaldelvira@gmail.com
 */
#pragma once
#ifndef COMPARATOR_H
#define COMPARATOR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*comparator_function_t) (const void*,const void*);

// COMMON COMPARATOR FUNCTIONS
int compare_int(const void *e_1, const void *e_2);
int compare_char(const void *e_1, const void *e_2);
int compare_float(const void *e_1, const void *e_2);
int compare_double(const void *e_1, const void *e_2);
int compare_long_long(const void *e_1, const void *e_2);

/**
 * An useless comparator function.
 * For when you don't need to compare elements
 * inside a structure (e.g. in a stack, if you
 * only need to push and pop)
*/
int compare_ignore(const void *e_1, const void *e_2);

#ifdef __cplusplus
}
#endif

#endif
