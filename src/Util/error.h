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

#ifdef __cplusplus
extern "C" {
#endif

// Error codes
#define SUCCESS		         1
#define ERROR                    0
#define INDEX_BOUNDS_ERROR      -0xE001
#define ALLOCATION_ERROR        -0xE002
#define NULL_PARAMETER_ERROR    -0xE003
#define MEMORY_OP_ERROR         -0xE004
#define ELEMENT_NOT_FOUND_ERROR -0xE005
#define REPEATED_ELEMENT_ERROR  -0xE006

// Print error macros
#define printerr(func, msg, ...) fprintf(stderr, "ERROR: " msg  ". Function: " #func "\n" __VA_ARGS__)  // C23 will add __VA_OPT__(,)

#define printerr_allocation(func) fprintf(stderr, "ERROR: Unable to allocate memory. Function: " #func "\n");

#define printerr_null_param(func) fprintf(stderr, "ERROR: NULL parameter(s). Function: " #func "\n");

#define printerr_out_of_bounds(index, max, func) fprintf(stderr, "ERROR: Index %llu out of bounds for [0, %llu). Function: " #func "\n", (unsigned long long) index, (unsigned long long) max);

#define printerr_memory_op(func) fprintf(stderr, "ERROR: In memory operation. Function: " #func "\n");

#define printerr_data_size(func) fprintf(stderr, "ERROR: Data size must be > 0. Function: " #func "\n");

#ifdef __cplusplus
}
#endif

#endif
