/*
 * hash.c - Implements functions to hash common data types.
 * Author: Saúl Valdelvira (2023)
 */
#include "hash.h"
#include <stdint.h>
#include <string.h>

hashcode_t hash_int(const void *arg){
        return (hashcode_t) * (int*) arg;
}

hashcode_t hash_short(const void *arg){
        return (hashcode_t) * (short*) arg;
}

hashcode_t hash_char(const void *arg){
        return (hashcode_t) * (char*) arg;
}

hashcode_t hash_long(const void *arg){
        return (hashcode_t) * (long*) arg;
}

/*
 * djb2 algorithm
 * see: <http://www.cse.yorku.ca/~oz/hash.html>
*/
hashcode_t hash_string(const void *arg){
        hashcode_t hash = 5381;
        const char *str = * (char**) arg;
        int c;
        while ((c = *str++)){
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }
        return hash;
}

hashcode_t hash_ptr(const void *arg) {
        void *ptr;
        memcpy(&ptr, arg, sizeof(void*));
        return (hashcode_t)ptr;
}

hashcode_t hash_combine(hashcode_t h1, hashcode_t h2) {
        return h1 ^ (h2 + 0x9e3779b9U + (h1 << 6) + (h1 >> 2));
}
