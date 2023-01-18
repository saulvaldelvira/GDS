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

#ifndef __linux__ // Needed for use of CLOCK_REALTIME outside Linux
	#define _GNU_SOURCE
	#define _POSIX_C_SOURCE 1999309L
#endif

#ifndef TIMESTAMP_DISABLE 
	static double timestamp;

	#define TIMESTAMP_START timestamp = get_time();

	#define TIMESTAMP_STOP timestamp = get_time() - timestamp;
#endif

static inline double get_time(){
	struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);
	return now.tv_sec + now.tv_nsec*1e-9;
}

/**
 * @return a random number between min and max
*/
static inline int rand_range(int min, int max){
	return rand()%((max+1)-min) + min;
}

#define END_MSG(name) printf("[" #name " test finished in "); \
		      if(timestamp>=1.0) printf("%.3f seconds]\n\n", timestamp); \
		      else printf("%.2f miliseconds]\n\n", timestamp * 1000);

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

#endif
