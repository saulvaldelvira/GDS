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

        for (int i = 0; i < N; i++) {
                deque_pop_front(q, &tmp);
                assert(tmp == i);
        }

        deque_free(q);
}

int main(void) {
        test_start("deque.c");
        push_back();
        test_end("deque.c");
}
