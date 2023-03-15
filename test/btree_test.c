#define QUIET_DISABLE
#include "test.h"
#undef QUIET_DISABLE

#include "../src/BTree/BTree.h"

#define ARR_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

int main(){
	BTree *tree;
	printf("[BTree Test]\n");
	TIMESTAMP_START
	tree = btree_init(sizeof(int), 5, compare_int);
	// Test case 1
	int nums[] = {60, 40, 80, 20, 55, 65, 63, 51, 75, 2, 4, 90, 95, 100, 41, 42, 50, 22, 30, 25, 31, 32, 33, 36, 38, 39};
	assert(btree_add_array(tree, nums, 26) == SUCCESS);

	int rem[] = {100, 60, 65, 63};
	assert(btree_remove_array(tree, rem, 4));

	assert(!btree_exists(tree, cast_int(100)));
	assert(!btree_exists(tree, cast_int(60)));
	assert(!btree_exists(tree, cast_int(65)));
	assert(!btree_exists(tree, cast_int(63)));

	assert(btree_add(tree, cast_int(100)));
	assert(btree_add(tree, cast_int(60)));
	assert(btree_add(tree, cast_int(65)));
	assert(btree_add(tree, cast_int(63)));
	for (int i=0; i < (int) ARR_SIZE(nums); i++){
		assert(btree_exists(tree, &nums[i]));
	}

	/// Config test
	btree_configure(tree, compare_allways_true);
	assert(btree_exists(tree, cast_int(-897987)));
	btree_configure(tree, compare_int);
	/////////////////////////////////

	btree_free(tree);

	printf("[BTree Test]\n");
	return 0;
	// Test case 2
	tree = btree_init(sizeof(int), 3, compare_int);

	assert(btree_add(tree, cast_int(4)));
	assert(btree_add(tree, cast_int(10)));

	assert(btree_add(tree, cast_int(7)));

	assert(btree_add(tree, cast_int(6)));

	assert(btree_add(tree, cast_int(20)));

	assert(btree_add(tree, cast_int(15)));
	assert(btree_add(tree, cast_int(5)));

	assert(btree_add(tree, cast_int(19)));
	assert(btree_add(tree, cast_int(16)));


	assert(btree_add(tree, cast_int(13)));
	assert(btree_add(tree, cast_int(12)));

	assert(btree_exists(tree, cast_int(4)));
	assert(btree_exists(tree, cast_int(10)));
	assert(btree_exists(tree, cast_int(7)));
	assert(btree_exists(tree, cast_int(6)));
	assert(btree_exists(tree, cast_int(20)));
	assert(btree_exists(tree, cast_int(15)));
	assert(btree_exists(tree, cast_int(5)));
	assert(btree_exists(tree, cast_int(19)));
	assert(btree_exists(tree, cast_int(16)));
	assert(btree_exists(tree, cast_int(13)));
	assert(btree_exists(tree, cast_int(12)));
	int n;
	assert(4 == * (int*) btree_get(tree, cast_int(4), &n));

	tree = btree_reset(tree);

	// Test case 3
	for (int i=1; i < 1020; i++){
		assert(btree_add(tree, &i));
		assert(btree_exists(tree, &i));
	}


	// Test case 3
	for (int i=1; i < 1020; i++){
		assert(btree_remove(tree, &i));
		assert(!btree_exists(tree, &i));
	}

	btree_free(tree);

	TIMESTAMP_STOP
	END_MSG(BTree)
	return 0;
}
