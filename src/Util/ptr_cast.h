/**
 *  Copyright (C) 2023 - Saúl Valdelvira
 *  License: BSD 3-Clause
 *  Email: saulvaldelvira@gmail.com
 */

/**
 * ptr_cast.h:
 *
 * Defines functions to "cast" literal values as void pointers.
 * Useful when adding element to a structure.
 * Example:
 *     vector_add(v, cast_int(12));
 *     instead of
 *     int tmp = 12;
 *     vector_add(v, &tmp);
 *
 *  It can also be used for functions that require a destiny pointer.
 *  Example:
 *      int n = * (int*) vector_get_at(v, 2, cast_int(0));
 *      instead of
 *      int n;
 *      vector_get_at(v, 2, &n);
*/

#pragma once
#ifndef PTR_CAST_H
#define PTR_CAST_H

#ifdef __cplusplus
extern "C" {
#endif

static inline void* cast_int(int n){
	static int _int[4];
	static int i = 0;
	if (i >= 4)
		i = 0;
	_int[i] = n;
	return (void*) &_int[i++];
}

static inline void* cast_char(char c){
	static char _char[4];
	static int i = 0;
	if (i >= 4)
		i = 0;
	_char[i] = c;
	return (void*) &_char[i++];
}

static inline void* cast_float(float f){
	static float _float[4];
	static int i = 0;
	if (i >= 4)
		i = 0;
	_float[i] = f;
	return (void*) &_float[i++];
}

static inline void* cast_double(double d){
	static double _double[4];
	static int i = 0;
	if (i >= 4)
		i = 0;
	_double[i] = d;
	return (void*) &_double[i++];
}

static inline void* cast_long(long l){
	static long _long[4];
	static int i = 0;
	if (i >= 4)
		i = 0;
	_long[i] = l;
	return (void*) &_long[i++];
}

static inline void* cast_long_long(long long ll){
	static long long _llong[4];
	static int i = 0;
	if (i >= 4)
		i = 0;
	_llong[i] = ll;
	return (void*) &_llong[i++];
}

#ifdef __cplusplus
}
#endif

#endif // PTR_CAST_H
