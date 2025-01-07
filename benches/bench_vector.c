#include "bench.h"
#include "vector.h"

int N;

vector_t *vec;

void init_vec(void) {
        vec = vector_init(sizeof(int), compare_int);
}

void init_with_elems(void) {
        init_vec();
        for (int i = 0; i < N; i++)
                vector_append(vec, &i);
}

void cleanup(void) {
        vector_free(vec);
}

void bench_append(void) {
        for (int i = 0; i < N; i++)
                vector_append(vec, &i);
}

void bench_remove_first(void) {
        for (int i = 0; i < N; i++)
                vector_remove_at(vec, 0);
}

int main(void) {
        printf("==== Benching vector.c ====\n");
        N = 10000;
        do_bench(bench_append, init_vec, cleanup);
        N = 2000;
        do_bench(bench_remove_first, init_with_elems, cleanup);
        printf("==========================\n");
}
