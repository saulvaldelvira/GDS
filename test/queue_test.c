#include "../src/Queue/queue.h"

#define COMPARATOR_ENABLE
#define TIMESTAMP_ENABLE
#include "test.h"
#undef COMPARATOR_ENABLE
#undef TIMESTAMP_ENABLE

int main(){
	int n = 10000, tmp;

	printf("[Starting Queue test]\n Workload: %d\n", n);
	TIMESTAMP_START

	Queue *q = queue_init(sizeof(int), compare_int);
	assert(queue_isempty(q));

	for(int i=0; i < n; i++){
		assert(queue_enqueue(q, &i));
		assert(queue_size(q) == (size_t) i + 1);
	}
	assert(!queue_isempty(q));
	assert(queue_size(q) == (size_t) n);
	for(int i=0; i<n; i++){
		assert(queue_size(q) == (size_t) n - i);
		assert(i == * (int*) queue_peek(q, &tmp));
		assert(i == * (int*) queue_dequeue(q, &tmp));
		assert(queue_size(q) == (size_t) n - i - 1);
	}
	assert(queue_isempty(q));
	queue_free(q);

	TIMESTAMP_STOP
	END_MSG(Queue)
}