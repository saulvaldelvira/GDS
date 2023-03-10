/**
 *  Defines error codes and macros to print error mesages.
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
#ifndef CHECKS_H
#define CHECKS_H

#define SUCCESS		         1
#define ERROR                    0
#define INDEX_BOUNDS_ERROR      -0xE001
#define ALLOCATION_ERROR        -0xE002
#define NULL_PARAMETER_ERROR    -0xE003
#define MEMORY_OP_ERROR         -0xE004
#define ELEMENT_NOT_FOUND_ERROR -0xE005
#define REPEATED_ELEMENT_ERROR  -0xE006

#define printerr_allocation(func) fprintf(stderr, "ERROR: Unable to allocate memory for on function " #func "\n");

#define printerr_null_param(func) fprintf(stderr, "ERROR: NULL parameter(s) in function " #func "\n");

#define printerr_out_of_bounds(index, func) fprintf(stderr, "ERROR: Index %llu out of bounds. At function " #func "\n", (unsigned long long) index);

#define printerr_memory_op(func) fprintf(stderr, "ERROR: At memory operation in function " #func "\n");

#define printerr_data_size(func) fprintf(stderr, "ERROR: Data size must be >= 0. In function " #func "\n");

#endif
