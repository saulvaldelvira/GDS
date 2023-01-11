/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 08-01-2023
 * Copyright: Saúl Valdelvira Iglesias (2023)
 * Licensed under the GNU GPL V.3. See /LICENSE file for more info
*/
#pragma once
#ifndef CHECKS_H
#define CHECKS_H

#define SUCCESS		             1
#define ERROR                    0
#define INDEX_BOUNDS_ERROR      -1
#define ALLOCATION_ERROR        -2
#define NULL_PARAMETER_ERROR    -3
#define MEMORY_OP_ERROR         -4
#define ELEMENT_NOT_FOUND_ERROR -5

#define printerr_allocation(func) fprintf(stderr, "ERROR: Unable to allocate memory for on function " #func "\n");

#define printerr_null_param(func) fprintf(stderr, "ERROR: NULL parameter(s) in function " #func "\n");

#define printerr_out_of_bounds(index, func) fprintf(stderr, "ERROR: Index %lu out of bounds. At function " #func "\n", index);

#define printerr_memory_op(func) fprintf(stderr, "ERROR: At memory operation in function " #func "\n");

#define printerr_data_size(func) fprintf(stderr, "ERROR: Data size must be >= 0. In function " #func "\n");

#endif