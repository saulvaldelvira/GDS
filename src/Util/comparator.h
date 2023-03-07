/**
 *  Comparator header.
 *  Defines functions to compare different data types.
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
#pragma once
#ifndef COMPARATOR_H
#define COMPARATOR_H

	typedef int (*comparator_function_t) (const void*,const void*);

	// COMMON COMPARATOR FUNCTIONS
	int compare_int(const void *e_1, const void *e_2);
	int compare_char(const void *e_1, const void *e_2);
	int compare_float(const void *e_1, const void *e_2);
	int compare_double(const void *e_1, const void *e_2);
	int compare_long_long(const void *e_1, const void *e_2);

	void set_float_precision(int num_decimals);
	void set_double_precision(int num_decimals);

	#define DEFAULT_FLOAT_PRECISION 0.00000001f
	#define DEFAULT_DOUBLE_PRECISION 100000000000000000

#endif
