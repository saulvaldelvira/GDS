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

#include "attrs.h"

// Error codes
typedef enum gds_return {
        GDS_SUCCESS = 1,
        GDS_ERROR = 0,
        GDS_INDEX_BOUNDS_ERROR = -0xE001,
        GDS_ELEMENT_NOT_FOUND_ERROR = -0xE002,
        GDS_REPEATED_ELEMENT_ERROR  = -0xE003,
        GDS_INVALID_PARAMETER_ERROR = -0xE004,
        GDS_NOMEM_ERROR = -0xE005,
} gds_return_t;

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

/**
 * Prints to stderr a descriptive message of the given error code.
*/
#define gds_print_error_msg(errcode) do {\
                        fprintf(stderr, "[GDS] %s", gds_get_error_msg(errcode));\
                        fprintf(stderr, " In %s, line %d (%s)\n", __FILE_NAME__, __LINE__, __func__);\
                }while (0)

_const_fn
const char* gds_get_error_msg(int error_code);

_pure_fn
gds_return_t gds_last_error(void);

_pure_fn
const char* gds_last_error_msg(void);

#ifdef __cplusplus
}
#endif

#endif
