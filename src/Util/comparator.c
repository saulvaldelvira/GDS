#include "comparator.h"


// COMPARATORS
int compare_int(void *e_1, void *e_2){
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

int compare_char(void *e_1, void *e_2){
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

int compare_float(void *e_1, void *e_2){
    float f_1 = * (float *) e_1;
    int   i_1 = f_1 * Precision.float_precision;

    float f_2 = * (float *) e_2;
    int   i_2 = f_2 * Precision.float_precision;
    if (i_1 > i_2){
        return 1;
    }else if (i_2 > i_1){
        return -1;
    }else{
        return 0;
    }
}

int compare_double(void *e_1, void *e_2){
    double d_1 = * (double *) e_1;
    int i_1 = d_1 * Precision.double_precision;

    double d_2 = * (double *) e_2;
    int i_2 = d_2 * Precision.double_precision;

    if (i_1 > i_2){
        return 1;
    }else if (i_2 > i_1){
        return -1;
    }else{
        return 0;
    }
}

/*void set_float_precision(int num_decimals){
        
}

void set_double_precision(int num_decimals){
        
}*/