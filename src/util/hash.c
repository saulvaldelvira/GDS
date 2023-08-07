/*
 * Implements functions to hash different data types.
 *
 *  Copyright (C) 2023 - Saúl Valdelvira
 *  License: BSD 3-Clause
 *  Email: saul@saulv.es
 */
#include "hash.h"

int64_t hash_int(const void *arg){
        return (int64_t) * (int*) arg;
}

int64_t hash_char(const void *arg){
        return (int64_t) * (char*) arg;
}

int64_t hash_long(const void *arg){
        return (int64_t) * (long*) arg;
}

/**
 * djb2 algorithm
 * see: <http://www.cse.yorku.ca/~oz/hash.html>
*/
int64_t hash_string(const void *arg){
        int64_t hash = 5381;
        char *str = (char*) arg;
        int c;
        while ((c = *str++)){
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }
        return hash;
}
