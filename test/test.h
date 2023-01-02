#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../src/Util/allocate.h"
#include "../src/Util/comparator.h"

#include <time.h>

double get_time(){
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec + now.tv_nsec*1e-9;
}