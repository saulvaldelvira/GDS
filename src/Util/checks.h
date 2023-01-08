/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 08-01-2023
 * Copyright: Saúl Valdelvira Iglesias (2023)
 * Licensed under the GNU GPL V.3. See /LICENSE file for more info
*/
#pragma once
#ifndef CHECKS_H
#define CHECKS_H

#define INDEX_OUT_OF_BOUNDS -1
#define ALLOCATION_ERROR    -2
#define NULL_PARAMETER      -3

/**
 * If x is NULL, prints and error and returns the ret parameter
*/
#define CHECK_MEMORY(x, func, ret) if(!x){ \
                            fprintf(stderr, "ERROR: Unable to allocate memory for " #x ".\nOn function " #func); \
                            return ret; \
                        }

/**
 * If the expresion is true, prints and error and returns the ret parameter
*/
#define CHECK_NULL(expr, func, ret) if(expr){ \
                                        fprintf(stderr, "ERROR: NULL parameters in function " #func); \
                                        return ret; \
                                    }

/**
 * If index is >= upper, prints and error and returns the ret parameter
*/
#define CHECK_BOUNDS(index, upper, func, ret) if (index >= upper){ \
                                                    fprintf(stderr, "ERROR: Index " #index " out of bounds\nAt function " #func); \
                                                    return ret; \
                                                }

#endif