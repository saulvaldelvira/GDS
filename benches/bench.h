#ifndef __BENCH_H__
#define __BENCH_H__

#define _POSIX_C_SOURCE 199309L

#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>

typedef void (*void_func_t)(void);
typedef void (*void_func_t)(void);

static int N_ITER = 1000;

#define BILLION  1000000000L

static inline long get_time_nanos(void){
        struct timespec t;
        clock_gettime( CLOCK_REALTIME, &t);
        long ns = (t.tv_sec * BILLION)
                  + t.tv_nsec;
        return ns;
}


static inline long bench(void_func_t body, void_func_t init, void_func_t cleanup) {
        long t = 0;

        for (int i = 0; i < N_ITER; i++) {
                init();
                long curr = get_time_nanos();
                body();
                t += get_time_nanos() - curr;
                cleanup();
        }

        return t / N_ITER;
}

#include <stdio.h>

void printfpunct2(long n, char sep) {
    if (n < 1000) {
        printf ("%ld", n);
        return;
    }
    printfpunct2 (n/1000, sep);
    printf ("%c%03ld", sep, n%1000);
}

void printfpunct(long n, char sep) {
    if (n < 0) {
        printf ("-");
        n = -n;
    }
    printfpunct2(n, sep);
}

#define do_bench(name, init, cleanup) { \
        long t = bench(name, init, cleanup); \
        printf(#name ": "); \
        printfpunct(t, ','); \
        printf(" ns/it\n"); }

#endif /* __BENCH_H__ */
