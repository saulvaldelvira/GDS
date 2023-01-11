/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 08-01-2023
 * Copyright (C) 2022-2023 Saúl Valdelvira
 * Licensed under the GNU GPL V.3. See /LICENSE file for more info
*/
#pragma once
#ifndef COMPARATOR_H
#define COMPARATOR_H
	#include <stdbool.h>
	#include <math.h>

	#define COMPARATOR_FUNCTION(name) int (*name) (const void*,const void*)

	// COMMON COMPARATOR FUNCTIONS
	int compare_int(const void *e_1, const void *e_2);
	int compare_char(const void *e_1, const void *e_2);
	int compare_float(const void *e_1, const void *e_2);
	int compare_double(const void *e_1, const void *e_2);
	int compare_long_long(const void *e_1, const void *e_2);

	#define DEFAULT_FLOAT_PRECISION 100000000
	#define DEFAULT_DOUBLE_PRECISION 100000000000000000

	static struct {
		long long float_precision;
		long long double_precision;
	} Precision = {
		.float_precision = DEFAULT_FLOAT_PRECISION,
		.double_precision = DEFAULT_DOUBLE_PRECISION
	};

	static inline void set_float_precision(int num_decimals){
		Precision.float_precision = pow(10, num_decimals);
	}
	
	static inline void set_double_precision(int num_decimals){
		Precision.double_precision = pow(10, num_decimals);
	}
#endif