/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 08-01-2023
 * Copyright: Saúl Valdelvira Iglesias (2023)
 * Licensed under the GNU GPL V.3. See /LICENSE file for more info
*/
#pragma once
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

typedef enum free_on_delete {FreeOnDelete=1, DontFreeOnDelete=0} free_on_delete_t;

#define offset(ptr,index,size) (void*)(((unsigned char*)ptr) + index * size)

#endif