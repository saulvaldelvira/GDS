#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../src/Util/allocate.h"
#include "../src/Util/comparator.h"
#include "../src/Util/checks.h"

#include <time.h>

static double timestamp;
static double timestamp_diff;

#define TIMESTAMP_START timestamp = get_time();

#define TIMESTAMP_STOP timestamp_diff = get_time() - timestamp;

double get_time(){
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec + now.tv_nsec*1e-9;
}