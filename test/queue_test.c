#include "../src/Queue/queue.h"

#define COMPARATOR_ENABLE
#define TIMESTAMP_ENABLE
#include "test.h"
#undef COMPARATOR_ENABLE
#undef TIMESTAMP_ENABLE

int main(){
    int n = 10000000;

    printf("[Starting Queue test]\n Workload: %d\n", n);
    TIMESTAMP_START

    Queue q = queue_init(Comparator.integer);
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

    END_MSG(Queue)
}