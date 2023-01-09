/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 08-01-2023
 * Copyright: Saúl Valdelvira Iglesias (2023)
 * Licensed under the GNU GPL V.3. See /LICENSE file for more info
*/
#pragma once
#ifndef ALLOCATE_H
#define ALLOCATE_H

static inline void* cast_int(int n){
    static int _int;
    _int = n;
    return (void*) &_int;
}

static inline void* cast_char(char c){
    static char _char;
    _char = c;
    return (void*) &_char;
}

static inline void* cast_float(float f){
    static float _float;
    _float = f;
    return (void*) &_float;
}

static inline void* cast_double(double d){
    static double _double;
    _double = d;
    return (void*) &_double;
}

static inline void* cast_long(long l){
    static long _long;
    _long = l;
    return (void*) &_long;
}

static inline void* cast_long_long(long long l){
    static long long _longlong;
    _longlong = l;
    return (void*) &_longlong;
}

#endif