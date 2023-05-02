/**
 *  Defines error codes and macros to print error mesages.
 *
 *  Copyright (C) 2023 - Saúl Valdelvira
 *
 *  This library is free software. You can modify
 *  and/or redistribute it under the terms of the
 *  GNU Lesser General Public License version 2.1
 *  See <https://www.gnu.org/licenses/>
 *
 *  Email: saulvaldelvira@gmail.com
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
#define ELEMENT_NOT_FOUND_ERROR -0xE004
#define REPEATED_ELEMENT_ERROR  -0xE005
#define INVALID_PARAMETER_ERROR -0xE006

// Print error macros
#ifdef __GDS_ENABLE_ERROR_MACROS

#define printerr(func, msg, ...) fprintf(stderr, "ERROR: " msg  ". Function: " #func "\n" __VA_ARGS__)  // C23 will add __VA_OPT__(,)

#define printerr_allocation(func) fprintf(stderr, "ERROR: Unable to allocate memory. Function: " #func "\n");

#define printerr_null_param(func) fprintf(stderr, "ERROR: NULL parameter(s). Function: " #func "\n");

#define printerr_out_of_bounds(func, index, max) fprintf(stderr, "ERROR: Index %zu out of bounds for [0, %zu). Function: " #func "\n", index, max);

#define printerr_data_size(func) fprintf(stderr, "ERROR: Data size must be > 0. Function: " #func "\n");

#endif //__GDS_ENABLE_ERROR_MACROS

#ifdef __cplusplus
}
#endif

#endif
