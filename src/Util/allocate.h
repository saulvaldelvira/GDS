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
void* alloc_int(int n);
void* alloc_char(char c);
void* alloc_float(float f);
void* alloc_double(double d);
void* alloc_long(long l);
void* alloc_long_long(long long l);
#endif