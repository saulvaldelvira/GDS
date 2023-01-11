#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifdef COMPARATOR_ENABLE
	#include "../src/Util/comparator.h"
#endif

#include "../src/Util/error.h"
#include <stdint.h>
#include <time.h>
#include "../src/Util/void_cast.h"
#include "../src/Util/definitions.h"

#ifndef __linux__ // Needed for use of CLOCK_REALTIME outside Linux
	#define _GNU_SOURCE
	#define _POSIX_C_SOURCE 1999309L
#endif

#ifdef TIMESTAMP_ENABLE 
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

#endif
