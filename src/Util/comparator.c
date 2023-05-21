/**
 *  Comparator module.
 *  Implements functions to compare different data types.
 *
 *  Copyright (C) 2023 - Saúl Valdelvira
 *  License: BSD 3-Clause
 *  Email: saulvaldelvira@gmail.com
 */
#include "comparator.h"

// COMPARATORS
int compare_int(const void *e_1, const void *e_2){
	int n_1 = * (int *) e_1;
	int n_2 = * (int *) e_2;
	if (n_1 > n_2){
		return 1;
	}else if (n_2 > n_1){
		return -1;
	}else{
		return 0;
	}
}

int compare_char(const void *e_1, const void *e_2){
	char c_1 = * (char *) e_1;
	char c_2 = * (char *) e_2;
	if (c_1 > c_2){
		return 1;
	}else if (c_2 > c_1){
		return -1;
	}else{
		return 0;
	}
}

int compare_long_long(const void *e_1, const void *e_2){
	long long l_1 = * (long long *) e_1;
	long long l_2 = * (long long *) e_2;
	if (l_1 > l_2){
		return 1;
	}else if (l_2 > l_1){
		return -1;
	}else{
		return 0;
	}
}

int compare_float(const void *e_1, const void *e_2){
	float f_1 = * (float *) e_1;
	float f_2 = * (float *) e_2;
	if (f_1 > f_2){
		return 1;
	}else if (f_2 > f_1){
		return -1;
	}else{
		return 0;
	}
}

int compare_double(const void *e_1, const void *e_2){
	double d_1 = * (double *) e_1;
	double d_2 = * (double *) e_2;
	if (d_1 > d_2){
		return 1;
	}else if (d_2 > d_1){
		return -1;
	}else{
		return 0;
	}
}

int compare_ignore(const void *e_1, const void *e_2){
	(void) e_1;
	(void) e_2;
	return 0;
}
