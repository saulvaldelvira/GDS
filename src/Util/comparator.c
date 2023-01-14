/**
 *  Comparator module. 
 *  Implements functions to compare different data types.
 * 
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
 *  Version: 11-01-2023
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

/**
 * I know these float and double comparator functions are not very "sophisitcated"
 * I really aim to learn how floating point arithmetic really works in computers. 
 * This semester we are gonna learn about it in class :D.
 * For now this functions only wrap a simple comparison, but I would like to make better functions that warantee 
 * a precision. Time will come.
 *  ~ 11-01-2023
*/

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

/* This is just garbage (for now...)

#include <math.h>

static struct {
	long long float_precision;
	long long double_precision;
} Precision = {
	.float_precision = DEFAULT_FLOAT_PRECISION,
	.double_precision = DEFAULT_DOUBLE_PRECISION
};

void set_float_precision(int num_decimals){
	Precision.float_precision = pow(10, num_decimals);
}

void set_double_precision(int num_decimals){
	Precision.double_precision = pow(10, num_decimals);
}*/
