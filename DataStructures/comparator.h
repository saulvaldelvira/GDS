#pragma once
#ifndef COMPARATOR_H
#define COMPARATOR_H
    #include <stdbool.h>
    #include <math.h>

    // COMMON COMPARATOR FUNCTIONS
    bool compare_int(void *e_1, void *e_2);
    bool compare_char(void *e_1, void *e_2);
    bool compare_float(void *e_1, void *e_2);
    bool compare_double(void *e_1, void *e_2);

    #define DEFAULT_FLOAT_PRECISION 100000000
    #define DEFAULT_DOUBLE_PRECISION 100000000000000000

    static struct {
        bool (*integer)             (void*, void*);
        bool (*character)           (void*, void*);
        bool (*floating)            (void*, void*);
        long long float_precision;
        bool (*double_floating)    (void*, void*);
        long long double_precision;
    } Comparators = {
        *compare_int,
        *compare_char,
        *compare_float,
        DEFAULT_FLOAT_PRECISION,
        *compare_double,
        DEFAULT_FLOAT_PRECISION
    };

    void set_float_precision(int num_decimals);
    void set_double_precision(int num_decimals);
#endif