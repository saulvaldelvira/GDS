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

#include "../src/Util/comparator.h"
#include "../src/Util/error.h"
#include "../src/Util/ptr_cast.h"
#include "../src/Util/definitions.h"

#include "color.h"

#ifndef TIMESTAMP_DISABLE
	static long timestamp;

	#define TIMESTAMP_START timestamp = get_time_millis();

	#define TIMESTAMP_STOP timestamp = get_time_millis() - timestamp;
#endif

/**
 * Returns a timestamp of the current time in milliseconds.
*/
static inline long get_time_millis(){
	return clock() * 1000.0 / CLOCKS_PER_SEC;
}

/**
 * Returns a random number between min and max
*/
static inline int rand_range(int min, int max){
	return rand() % ((max + 1) - min) + min;
}

//////////////////////////////////////

// Colored output ///////////////////
#ifndef NO_COLOR
	#define print_test_step(name) 	printf("* " Color_Yellow "%s" Color_Reset " ... ", #name); fflush(stdout);
	#define print_test_ok() 	printf(Color_Green "OK\n" Color_Reset)
	#define print_test_end(name) 	printf("[" #name " test finished in" Color_BCyan " %ld " Color_Reset "milliseconds]\n\n", timestamp);
#else
	#define print_test_step(name) 	printf("* %s ... ", #name); fflush(stdout);
	#define print_test_ok() 	printf("OK\n")
	#define print_test_end(name) 	printf("[" #name " test finished in %ld milliseconds]\n\n", timestamp);
#endif

#define print_test_start(name) 	printf("[Starting " #name " test]\n")

///// Assert  //////////////////////
#ifndef NO_COLOR
	#define assert(expr) if (!(expr)){ fprintf(stderr, Color_BRed "[%d] ASSERT FAILED: " Color_Reset #expr "\n", __LINE__); abort(); }
#else
	#define assert(expr) if (!(expr)){ fprintf(stderr, "[%d] ASSERT FAILED: " #expr "\n", __LINE__); abort(); }
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

//////////////////////////////////////

#define Clear_Line	"\033[2K\033[1A"
#define Move_Line(n)	"\33["#n"C"

#define Ignore_Error(call,n) 	do {\
				fprintf(stderr, "\n");\
				call;\
				fprintf(stderr, Clear_Line Clear_Line Clear_Line Move_Line(n));\
				fflush(stderr);\
				}while(0);

#ifdef __cplusplus
}
#endif

#endif
