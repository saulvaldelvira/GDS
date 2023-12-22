/*
 * compare.c - Functions to compare common data types.
 * Author: Saúl Valdelvira (2023)
 */
#include "compare.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// COMPARATORS
int compare_int(const void *e_1, const void *e_2){
        int i_1 = * (int *) e_1;
        int i_2 = * (int *) e_2;
        return i_1 - i_2;
}

int compare_char(const void *e_1, const void *e_2){
        char c_1 = * (char *) e_1;
        char c_2 = * (char *) e_2;
        return c_1 - c_2;
}

int compare_long(const void *e_1, const void *e_2){
        long l_1 = * (long *) e_1;
        long l_2 = * (long *) e_2;
        if (l_1 > l_2){
                return 1;
        }else if (l_2 > l_1){
                return -1;
        }else{
                return 0;
        }
}

int compare_long_long(const void *e_1, const void *e_2){
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

int compare_float(const void *e_1, const void *e_2){
        float f_1 = * (float *) e_1;
        float f_2 = * (float *) e_2;
        if (f_1 > f_2){
                return 1;
        }else if (f_2 > f_1){
                return -1;
        }else{
                return 0;
        }
}

int compare_double(const void *e_1, const void *e_2){
        double d_1 = * (double *) e_1;
        double d_2 = * (double *) e_2;
        if (d_1 > d_2){
                return 1;
        }else if (d_2 > d_1){
                return -1;
        }else{
                return 0;
        }
}

int compare_pointer(const void *e_1, const void *e_2){
        void *ptr1, *ptr2;
        memcpy(&ptr1, e_1, sizeof(void*));
        memcpy(&ptr2, e_2, sizeof(void*));
        uintptr_t i1 = (uintptr_t) ptr1;
        uintptr_t i2 = (uintptr_t) ptr2;
        if (i1 > i2) return 1;
        else if (i1 < i2) return -1;
        else return 0;
}

int compare_lesser(const void *e_1, const void *e_2){
        (void) e_1;
        (void) e_2;
        return -1;
}

int compare_equal(const void *e_1, const void *e_2){
        (void) e_1;
        (void) e_2;
        return 0;
}

int compare_greater(const void *e_1, const void *e_2){
        (void) e_1;
        (void) e_2;
        return 1;
}

/*
 * This is tricky. The vector will allways pass elements as void*.
 * Since in this case the element itself is also a void*, we could
 * commit a mistake, and directly free e. But in a vector of pointers,
 * e would be a pointer to the actual pointer element (the one we have to free).
 */
void destroy_ptr(void *e){
        if (e){
                void *ptr;
                memcpy(&ptr, e, sizeof(void*));
                free(ptr);
        }
}
