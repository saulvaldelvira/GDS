/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 08-01-2023
 * Copyright: Saúl Valdelvira Iglesias (2023)
 * Licensed under the GNU GPL V.3. See /LICENSE file for more info
*/
#define DISABLE_COMPARATOR
#include "comparator.h"
#undef DISABLE_COMPARATOR
#include <stdint.h>

#define NOT_NULL(a,b) if(a == NULL){if(b == NULL){return 0;}return -1;}else if(b == NULL){return 1;}

// COMPARATORS
int compare_int(const void *e_1, const void *e_2){
    NOT_NULL(e_1, e_2)
    int n_1 = * (int *) e_1;
    int n_2 = * (int *) e_2;
    if (n_1 > n_2){
        return 1;
    }else if (n_2 > n_1){
        return -1;
    }else{
        return 0;
    }
}

int compare_char(const void *e_1, const void *e_2){
    NOT_NULL(e_1, e_2)
    char c_1 = * (char *) e_1;
    char c_2 = * (char *) e_2;
    if (c_1 > c_2){
        return 1;
    }else if (c_2 > c_1){
        return -1;
    }else{
        return 0;
    }
}

int compare_float(const void *e_1, const void *e_2){
    NOT_NULL(e_1, e_2)
    float f_1 = * (float *) e_1;
    long long   i_1 = f_1 * Precision.float_precision;

    float f_2 = * (float *) e_2;
    long long   i_2 = f_2 * Precision.float_precision;
    if (i_1 > i_2){
        return 1;
    }else if (i_2 > i_1){
        return -1;
    }else{
        return 0;
    }
}

int compare_double(const void *e_1, const void *e_2){
    NOT_NULL(e_1, e_2)
    double d_1 = * (double *) e_1;
    long long i_1 = d_1 * Precision.double_precision;

    double d_2 = * (double *) e_2;
    long long i_2 = d_2 * Precision.double_precision;

    if (i_1 > i_2){
        return 1;
    }else if (i_2 > i_1){
        return -1;
    }else{
        return 0;
    }
}

int compare_long_long(const void *e_1, const void *e_2){
    NOT_NULL(e_1, e_2)
    long long l_1 = * (long long *) e_1;
    long long l_2 = * (long long *) e_2;

    if (l_1 > l_2){
        return 1;
    }else if (l_2 > l_1){
        return -1;
    }else{
        return 0;
    }
}

int compare_literal_integer(const void *e_1, const void *e_2) {
    intptr_t l_1 = (intptr_t) e_1;
    intptr_t l_2 = (intptr_t) e_2;

    if (l_1 > l_2){
        return 1;
    }else if (l_2 > l_1){
        return -1;
    }else{
        return 0;
    }
}
