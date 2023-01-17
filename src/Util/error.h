/**
 *  Defines error codes and macros to print error mesages.
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
#ifndef CHECKS_H
#define CHECKS_H

#define SUCCESS		         1
#define ERROR                    0
#define INDEX_BOUNDS_ERROR      -1
#define ALLOCATION_ERROR        -2
#define NULL_PARAMETER_ERROR    -3
#define MEMORY_OP_ERROR         -4
#define ELEMENT_NOT_FOUND_ERROR -5
#define REPEATED_ELEMENT_ERROR  -6

#define printerr_allocation(func) fprintf(stderr, "ERROR: Unable to allocate memory for on function " #func "\n");

#define printerr_null_param(func) fprintf(stderr, "ERROR: NULL parameter(s) in function " #func "\n");

#define printerr_out_of_bounds(index, func) fprintf(stderr, "ERROR: Index %lu out of bounds. At function " #func "\n", index);

#define printerr_memory_op(func) fprintf(stderr, "ERROR: At memory operation in function " #func "\n");

#define printerr_data_size(func) fprintf(stderr, "ERROR: Data size must be >= 0. In function " #func "\n");

#endif