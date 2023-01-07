#pragma once
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

typedef enum free_on_delete {FreeOnDelete=1, DontFreeOnDelete=0} free_on_delete_t;

#define OFFSET(arr, index, size) (void*)((char*) arr) + (index * size)

#endif