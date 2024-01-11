#ifndef TEST_H
#define TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include "../src/util/compare.h"
#include "../src/util/error.h"
#include "../src/util/definitions.h"

/*
 * Returns a timestamp of the current time in milliseconds.
*/
static inline long get_time_millis(void){
	return clock() * 1000.0 / CLOCKS_PER_SEC;
}

/*
 * Returns a random number between min and max
*/
static inline int rand_range(int min, int max){
	return rand() % ((max + 1) - min) + min;
}

//////////////////////////////////////

// Colored output ///////////////////
#ifndef NO_COLOR
#define Color_Reset  "\033[0m"
#define Color_Green  "\033[0;32m"
#define Color_Yellow "\033[0;33m"
#define Color_BRed   "\033[1;31m"
#define Color_BCyan  "\033[1;36m"

#define test_step(name) do { printf("* " Color_Yellow "%s" Color_Reset " ... ", name); fflush(stdout); } while(0)
#define test_ok() 	printf(Color_Green "OK\n" Color_Reset)
#define test_end(name) 	printf("[" #name " test finished in" Color_BCyan " %ld " Color_Reset "milliseconds]\n\n", get_time_millis() - timestamp)
#else
#define test_step(name) do { printf("* %s ... ", name); fflush(stdout); } while(0)
#define test_ok() 	printf("OK\n")
#define test_end(name) 	printf("[" #name " test finished in %ld milliseconds]\n\n", get_time_millis() - timestamp)
#endif

#define test_start(name) long timestamp = get_time_millis() ; printf("[Starting %s test]\n", name);

///// Assert  //////////////////////
#ifndef NO_COLOR
#define assert(expr) do { if (!(expr)){ fprintf(stderr, Color_BRed "[%d] ASSERT FAILED: %s" Color_Reset "\n", __LINE__, #expr); abort(); } } while(0)
#else
#define assert(expr) do { if (!(expr)){ fprintf(stderr, "[%d] ASSERT FAILED: %s\n", __LINE__, #expr); abort(); } } while(0)
#endif

void assert_array_int(int *arr, int *exp, int size){
        for (int i = 0; i < size; i++){
                assert(arr[i] == exp[i]);
        }
}

void assert_array_char(char *arr, char *exp, int size){
        for (int i = 0; i < size; i++){
                assert(arr[i] == exp[i]);
        }
}

int* int_array(int start, int end){
	int *arr = malloc((end - start) * sizeof(int));
        for (int i = start; i < end; i++){
                arr[i] = i;
        }
	return arr;
}

void shuffle_array(int *arr, int length){
	for (int i = 0; i < length; i++){
                int tmp = arr[i];
                int pos = rand() % length;
                arr[i] = arr[pos];
                arr[pos] = tmp;
        }
}

//////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif
