/**
 *  Defines functions to return void pointers, given the most 
 *  common data types. Used to avoid declaring temporary variables
 *  in order to add infomation to the structures.
 * 
 *  Copyright (C) 2022-2023  Saúl Valdelvira Iglesias
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *  Email: saulvaldelvira@gmail.com
 *  Version: 11-01-2023
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