#include "../src/BSTree/BSTree.h"
#define QUIET_DISABLE
#include "test.h"
#undef QUIET_DISABLE

struct test{
	int *i;
	char *c;
};

void join_test(){
        BSTree *t1 = bst_init(sizeof(int), compare_int);
        BSTree *t2 = bst_init(sizeof(int), compare_int);

        for (int i = 0; i < 10; i++){
                bst_add(t1, &i);
        }

        for (int i = 10; i < 20; i++){
                bst_add(t2, &i);
        }

        BSTree *joint = bst_join(t1, t2);

        for (int i = 0; i < 20; i++){
                assert(bst_exists(joint, &i));
        }

        bst_free(t1);
        bst_free(t2);
        bst_free(joint);
}

int main(){


	int n = 1000;//, min = 0, max = 10;
	int temp;
	printf("[BSTree Test]\n");
	TIMESTAMP_START
	BSTree *t = bst_init(sizeof(int), compare_int);
	// Random numbers test
	assert(!bst_exists(t, &n));
	printf("Add...\n");
	for(int i=0; i<n; i++){
		assert(bst_add(t, &i));
		assert(bst_exists(t, &i));
		bst_get(t, &i, &temp);
		assert(temp == i);
	}
	
	printf("Remove...\n");
	for(int i=0; i<n; i++){
		bst_remove(t, &i);
	}
	// Orders test
	printf("Traversals... \n");
	t = bst_reset(t);
	int nums[] = {12, 9, 7, 10, 0, 8, 11, 30, 25, 32, 31, 33};
	assert(bst_add_array(t, nums, 12UL));
	int max, min;
	assert(33 == * (int*) bst_max(t, &max));
	assert(0 == * (int*) bst_min(t, &min));


	void* inord = bst_inorder(t);
	printf("\tInorder: \t");
	for(size_t i=0; i < bst_size(t); i++){
		printf("%d-", * (int*) void_offset(inord, i * sizeof(int)));
	}
	printf("\n");
	printf("\tExpected result: 0-7-8-9-10-11-12-25-30-31-32-33-\n");

	free(inord);

	void* preord = bst_preorder(t);
	printf("\n\tPreorder:  \t");
	for(size_t i=0; i < bst_size(t); i++){
		printf("%d-", * (int*) void_offset(preord, i * sizeof(int)));
	}
	printf("\n");
	printf("\tExpected result: 12-9-7-0-8-10-11-30-25-32-31-33-\n");

	free(preord);

	void* postord = bst_postorder(t);
	printf("\n\tPostorder: \t");
	for(size_t i=0; i < bst_size(t); i++){
		printf("%d-", * (int*) void_offset(postord, i * sizeof(int)));
	}
	printf("\n");
	printf("\tExpected result: 0-8-7-11-10-9-25-31-33-32-30-12-\n");
	
	free(postord);

	// Remove test
	t = bst_reset(t);
	assert(t != NULL);

	int nums2[] = {12, 5, 3, 23, 30};
	assert(bst_add_array(t, nums2, 5UL));

	//int cinco = 5, veintitres = 23;
	assert(bst_remove(t, cast_int(5)));
	assert(bst_remove(t, cast_int(23)));

	void* remove_inord = bst_inorder(t);
	assert(3 == * (int*) remove_inord);
	assert(12 == * (int*) void_offset(remove_inord, 1 * sizeof(int)));
	assert(30 == * (int*) void_offset(remove_inord, 2 * sizeof(int)));

	free(remove_inord);

	t = bst_reset(t);

	assert(bst_add_array(t, nums, 12UL));
	assert(bst_remove_array(t, nums, 12UL));
	assert(bst_size(t) == 0UL);

	bst_free(t);

	join_test();

	TIMESTAMP_STOP
	END_MSG(BSTree);
	
	return 0;
}