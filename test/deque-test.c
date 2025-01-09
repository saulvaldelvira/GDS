#include "test.h"
#include <deque.h>

void push_back(void) {
        deque_t *q = deque_init(sizeof(int), compare_int);
        const int N = 10000;
        int tmp;

        for (int i = 0; i < N; i++)
                deque_push_back(q, &i);

        deque_at(q, 2, &tmp);
        assert(tmp == 2);
        assert((int)deque_size(q) == N);

        for (int i = 0; i < N; i++) {
                deque_pop_front(q, &tmp);
                assert(tmp == i);
        }

        assert((int)deque_size(q) == 0);

        deque_free(q);
}

void destructor(void) {
        deque_t *q = deque_init(sizeof(void*), compare_pointer);
        deque_set_destructor(q, destroy_ptr);

        deque_t *q2 = deque_init(sizeof(void*), compare_pointer);
        deque_set_destructor(q2, destroy_ptr);

        const int N = 10000;

        for (int i = 0; i < N; i++) {
                void *ptr = malloc(1024);
                deque_push_back(q, &ptr);
        }

        assert((int)deque_size(q) == N);
        assert((int)deque_size(q2) == 0);

        for (int i = 0; i < N; i++) {
                void *ptr;
                deque_pop_back(q, &ptr);
                deque_push_back(q2, &ptr);
        }

        assert((int)deque_size(q2) == N);
        assert((int)deque_size(q) == 0);

        deque_free(q, q2);
}

void remove_range(void) {
        deque_t *q = deque_init(sizeof(int), compare_int);
        const int N = 10000;

        for (int i = 0; i < N; i++)
                deque_push_back(q, &i);

        deque_remove_at(q, 900);
        assert(!deque_exists(q, &(int){900}));

        deque_free(q);
}

int main(void) {
        test_start("deque.c");
        push_back();
        destructor();
        remove_range();
        test_end("deque.c");
}
