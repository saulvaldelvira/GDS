/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 08-01-2023
 * Copyright (C) 2022-2023 Saúl Valdelvira
 * Licensed under the GNU GPL V.3. See /LICENSE file for more info
*/
#pragma once
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define void_offset(ptr, bytes) (void*)(((unsigned char*)ptr) + (bytes))
#define matrix_offset(arr, dim, rows, cols) (arr+((dim)*(rows) + (cols)))

#endif
