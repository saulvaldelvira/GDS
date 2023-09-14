/**
 * @file comparator.h
 * Defines functions to compare different data types.
 *
 *  Copyright (C) 2023 - Saúl Valdelvira \n
 *  License: BSD 3-Clause \n
 *  Email: saul@saulv.es
 */
/// @cond
#pragma once
#ifndef COMPARATOR_H
#define COMPARATOR_H
/// @endcond

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Type of a comparator function.
 * It receives 2 pointers (e_1 and e_2), and returns.
 * - <= -1 if e_1 < e_2
 * - 0     if e_1 == e_2
 * - >= 1  if e_1 > e_2
*/
typedef int (*comparator_function_t) (const void*,const void*);

int compare_int(const void *e_1, const void *e_2);              ///< Compares two int pointers
int compare_char(const void *e_1, const void *e_2);             ///< Compares two char pointers
int compare_float(const void *e_1, const void *e_2);            ///< Compares two float pointers
int compare_double(const void *e_1, const void *e_2);           ///< Compares two double pointers
int compare_long(const void *e_1, const void *e_2);             ///< Compares two long pointers
int compare_long_long(const void *e_1, const void *e_2);        ///< Compares two long long pointers

/*
 * Useless comparator functions.
 * For when you don't need to compare elements
 * inside a structure (e.g. in a stack, if you
 * only need to push and pop)
*/
int compare_equal(const void *e_1, const void *e_2); ///< Allways returns 0
int compare_lesser(const void *e_1, const void *e_2); ///< Allways returns -1
int compare_greater(const void *e_1, const void *e_2); ///< Allways returns 1

// Destructor functions

/**
 * Type of a destructor function.
 * It receives the address of the element to destroy.
*/
typedef void (*destructor_function_t) (void *);
void destroy_ptr(void *e);

#ifdef __cplusplus
}
#endif

#endif
