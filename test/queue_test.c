#include "test.h"
#include "../src/Queue/queue.h"

int main(){
    int n = 10000000;

    printf("[Starting Queue test]\n Workload: %d\n", n);
    TIMESTAMP_START

    Queue q = queue_init(Comparators.integer);
    queue_configure(&q, FREE_ON_DELETE);
    assert(queue_isempty(q));

    for(int i=0; i < n; i++){
        queue_enqueue(&q, alloc_int(i));
    }
    assert(!queue_isempty(q));

    for(int i=0; i<n; i++){
        assert(i == * (int*) queue_peek(q));
        assert(i == * (int*) queue_dequeue(&q));
    }
    assert(queue_isempty(q));

    TIMESTAMP_STOP

    printf("[Queue test finished in %.3f seconds]\n\n", timestamp_diff);
}