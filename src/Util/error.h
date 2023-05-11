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
#ifdef GDS_ENABLE_ERROR_MACROS

// If it's not already defined, define a macro for the base name of the file
// Example: If __FILE__ is "/path/to/proyect/main.c" __FILE_NAME__ is "main.c"
#ifndef __FILE_NAME__
        #include <string.h>
        #ifdef __unix__
                #define __FILE_NAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
        #else
                #define __FILE_NAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
        #endif
#endif

#define _context_str "In %s, line %d (%s)\n"
#define _context_args __FILE_NAME__, __LINE__, __func__

#define printerr(msg, ...) fprintf(stderr, "ERROR: " msg  ".\n"_context_str __VA_ARGS__, _context_args)  // C23 will add __VA_OPT__(,)

#define printerr_allocation() fprintf(stderr, "ERROR: Unable to allocate memory.\n" _context_str, _context_args)

#define printerr_null_param() fprintf(stderr, "ERROR: NULL parameter(s).\n" _context_str, _context_args)

#define printerr_out_of_bounds(index, max) fprintf(stderr, "ERROR: Index %zu out of bounds for [0, %zu).\n" _context_str, index, max, _context_args)

#define printerr_data_size() fprintf(stderr, "ERROR: Data size must be > 0.\n" _context_str, _context_args)

#endif //__GDS_ENABLE_ERROR_MACROS

#ifdef __cplusplus
}
#endif

#endif
