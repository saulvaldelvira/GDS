/*
 * hash.c - Implements functions to hash common data types.
 * Author: Saúl Valdelvira (2023)
 */
#include "hash.h"
#include <stdint.h>
#include <string.h>

int64_t hash_int(const void *arg){
        return (int64_t) * (int*) arg;
}

int64_t hash_short(const void *arg){
        return (int64_t) * (short*) arg;
}

int64_t hash_char(const void *arg){
        return (int64_t) * (char*) arg;
}

int64_t hash_long(const void *arg){
        return (int64_t) * (long*) arg;
}

/*
 * djb2 algorithm
 * see: <http://www.cse.yorku.ca/~oz/hash.html>
*/
int64_t hash_string(const void *arg){
        int64_t hash = 5381;
        const char *str = * (char**) arg;
        int c;
        while ((c = *str++)){
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }
        return hash;
}

int64_t hash_ptr(const void *arg) {
        void *ptr;
        memcpy(&ptr, arg, sizeof(void*));
        return (int64_t)ptr;
}
