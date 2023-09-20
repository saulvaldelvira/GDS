/*
 * error.h - Defines error codes and macros to print error mesages.
 * Author: Saúl Valdelvira (2023)
 */
#pragma once
#ifndef ERROR_H
#define ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

// Error codes
#define SUCCESS		         1
#define ERROR                    0
#define INDEX_BOUNDS_ERROR      -0xE001
#define NULL_PARAMETER_ERROR    -0xE002
#define ELEMENT_NOT_FOUND_ERROR -0xE003
#define REPEATED_ELEMENT_ERROR  -0xE004
#define INVALID_PARAMETER_ERROR -0xE005

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

#define printerr_null_param() fprintf(stderr, "ERROR: NULL parameter(s).\n" _context_str, _context_args)

#define printerr_out_of_bounds(index, min, max) fprintf(stderr, "ERROR: Index %lld out of bounds for [%lld, %lld).\n" _context_str, (long long)index, (long long)min, (long long)max, _context_args)

#define printerr_data_size() fprintf(stderr, "ERROR: Data size must be > 0.\n" _context_str, _context_args)

#endif //__GDS_ENABLE_ERROR_MACROS

#ifdef __cplusplus
}
#endif

#endif
