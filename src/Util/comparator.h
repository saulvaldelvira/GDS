#pragma once
#ifndef COMPARATOR_H
#define COMPARATOR_H
    #include <stdbool.h>
    #include <math.h>

    #define COMPARATOR_FUNCTION(name) int (*name) (const void*,const void*)

    // COMMON COMPARATOR FUNCTIONS
    int compare_int(const void *e_1, const void *e_2);
    int compare_char(const void *e_1, const void *e_2);
    int compare_float(const void *e_1, const void *e_2);
    int compare_double(const void *e_1, const void *e_2);

    #define DEFAULT_FLOAT_PRECISION 100000000
    #define DEFAULT_DOUBLE_PRECISION 100000000000000000

#ifndef DISABLE_COMPARATOR
    static struct {
        int (*integer)             (const void*, const void*);
        int (*character)           (const void*, const void*);
        int (*floating)            (const void*, const void*);
        int (*double_floating)     (const void*, const void*);
    } Comparator = { 
                    .integer = *compare_int,
                    .character = *compare_char,
                    .floating = *compare_float,
                    .double_floating = *compare_double,
                };
#endif

    static struct {
        long long float_precision;
        long long double_precision;
    } Precision = {
        .float_precision = DEFAULT_FLOAT_PRECISION,
        .double_precision = DEFAULT_DOUBLE_PRECISION
    };

    static inline void set_float_precision(int num_decimals){
        Precision.float_precision = pow(10, num_decimals);
    }
    
    static inline void set_double_precision(int num_decimals){
        Precision.double_precision = pow(10, num_decimals);
    }
#endif