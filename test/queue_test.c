#include "../src/Queue.h"

#define QUIET_DISABLE
#include "test.h"
#undef QUIET_DISABLE

int main(){
	int n = 10000, tmp;

	print_test_start(Queue);
	TIMESTAMP_START

	Queue *q = queue_init(sizeof(int), compare_int);
	assert(queue_isempty(q));

	for(int i=0; i < n; i++){
		assert(queue_enqueue(q, &i));
		assert(queue_size(q) == (size_t) i + 1);
	}
	assert(!queue_isempty(q));
	assert(queue_size(q) == (size_t) n);

	/// config test
	queue_configure(q, compare_always_equal);
	assert(queue_exists(q, cast_int(-854654)));
	queue_configure(q, compare_int);
	//////////

	for(int i=0; i<n; i++){
		assert(queue_size(q) == (size_t) n - i);
		assert(i == * (int*) queue_peek(q, &tmp));
		assert(i == * (int*) queue_dequeue(q, &tmp));
		assert(queue_size(q) == (size_t) n - i - 1);
	}
	assert(queue_isempty(q));

	q  = queue_reset(q);
	int nums[] = {1, 2, 3, 4, 5, 6, 7, 8};
	int dest[8];
	assert(queue_enqueue_array(q, nums, 8));
	assert(8UL == queue_size(q));
	assert(queue_dequeue_array(q, dest, 8));
	assert(queue_isempty(q));
	for (int i = 0; i < 8; i++){
		assert(nums[i] == dest[i]);
	}

	queue_free(q);

	TIMESTAMP_STOP
	print_test_end(Queue);
}