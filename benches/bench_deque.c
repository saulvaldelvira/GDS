#include "bench.h"
#include "deque.h"
#include <stdio.h>

const int N = 10000;

deque_t *deque;

void init_deque(void) {
        deque = deque_init(sizeof(int), compare_int);
}

void init_with_elems(void) {
        init_deque();
        for (int i = 0; i < N; i++)
                deque_push_back(deque, &i);
}

void cleanup(void) {
        deque_free(deque);
}

void bench_append(void) {
        for (int i = 0; i < N; i++)
                deque_push_back(deque, &i);
}

void bench_remove_first(void) {
        for (int i = 0; i < N; i++)
                deque_remove_front(deque);
}

int main(void) {
        printf("==== BENCHING deque.c ====\n");
        do_bench(bench_append, init_deque, cleanup);
        do_bench(bench_remove_first, init_with_elems, cleanup);
        printf("==========================\n");
}
