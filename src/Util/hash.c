/**
 *  Copyright (C) 2023 - Saúl Valdelvira
 *
 *  This program is free software. You can modify
 *  and/or redistribute it under the terms of the
 *  GNU General Public License version 2, or newer.
 *  See <https://www.gnu.org/licenses/>
 *
 *  Email: saulvaldelvira@gmail.com
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
