#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "../src/Util/comparator.h"
#include "../src/Util/error.h"
#include "../src/Util/void_cast.h"
#include "../src/Util/definitions.h"

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

#define END_MSG(name) printf("[" #name " test finished in %ld milliseconds]\n\n", timestamp);

#ifndef QUIET_DISABLE
	#ifdef QUIET
	static bool quiet = true;
	#else
	static bool quiet = false;
	#endif
#endif

#define LOG(x) if (!quiet){x;}

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

int compare_allways_true(const void* e1, const void *e2){
	(void) e1; (void) e2;
	return 0;
}

#endif
