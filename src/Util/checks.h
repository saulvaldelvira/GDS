#pragma once
#ifndef CHECKS_H
#define CHECKS_H

#define INDEX_NOT_FOUND     -1
#define ALLOCATION_ERROR    -2

#define CHECK_MEMORY(x, func, ret) if(!x){ \
                            fprintf(stderr, "ERROR: Unable to allocate memory for " #x ".\nOn function " #func); \
                            return ret; \
                        }

#define CHECK_NULL(expr, func) if(expr){ \
                                        fprintf(stderr, "ERROR: NULL parameters in function " #func); \
                                        return 0; \
                                    }

#define CHECK_BOUNDS(index, lower, upper, func, ret) if (index < lower || index >= upper){ \
                                                    fprintf(stderr, "ERROR: Index " #index " out of bounds\nAt function " #func); \
                                                    return ret; \
                                                }

#endif