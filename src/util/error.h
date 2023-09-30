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
                        fprintf(stderr, "[GDS] ");\
                        switch (errcode){\
                        case INDEX_BOUNDS_ERROR:\
                                fprintf(stderr, "Index out ouf bounds."); break;\
                        case NULL_PARAMETER_ERROR:\
                                fprintf(stderr, "NULL parameter."); break;\
                        case ELEMENT_NOT_FOUND_ERROR:\
                                fprintf(stderr, "Element not found."); break;\
                        case REPEATED_ELEMENT_ERROR:\
                                fprintf(stderr, "Repeated element."); break;\
                        case INVALID_PARAMETER_ERROR:\
                                fprintf(stderr, "Invalid parameter."); break;\
                        default:\
                                fprintf(stderr, "Unknow error code: %d.", errcode); break;\
                        }\
                        fprintf(stderr, " In %s, line %d (%s)\n", __FILE_NAME__, __LINE__, __func__);\
                }while (0)

#ifdef __cplusplus
}
#endif

#endif
