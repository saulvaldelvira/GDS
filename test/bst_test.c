#include "../src/BSTree/BSTree.h"

#define COMPARATOR_ENABLE
#include "test.h"
#undef COMPARATOR_ENABLE

struct test{
    int *i;
    char *c;
};

int main(){
    int n = 1000;//, min = 0, max = 10;
    int temp;
    printf("[BSTree Test]\n");
    BSTree t = bst_init(sizeof(int), Comparator.integer);
    // Random numbers test
    assert(!bst_exists(t, &n));
    printf("Add...\n");
    for(int i=0; i<n; i++){
        assert(bst_add(&t, &i));
        assert(bst_exists(t, &i));
        bst_get(t, &i, &temp);
        assert(temp == i);
    }
    
    printf("Remove...\n");
    for(int i=0; i<n; i++){
        bst_remove(&t, &i);
    }

    // Orders test
    printf("Traversals... \n");
    bst_reset(&t);
    int nums[] = {12, 9, 7, 10, 0, 8, 11, 30, 25, 32, 31, 33};
    for(size_t i=0; i < sizeof(nums) / sizeof(nums[0]); i++){
        assert(bst_add(&t, &nums[i]));
    }

    void* inord = bst_inorder(t);
    printf("\tInorder: \t");
    for(size_t i=0; i < t.n_elements; i++){
        printf("%d-", * (int*) offset(inord, i, sizeof(int)));
    }
    printf("\n");
    printf("\tExpected result: 0-7-8-9-10-11-12-25-30-31-32-33-\n");

    free(inord);

    void* preord = bst_preorder(t);
    printf("\n\tPreorder:  \t");
    for(size_t i=0; i < t.n_elements; i++){
        printf("%d-", * (int*) offset(preord, i, sizeof(int)));
    }
    printf("\n");
    printf("\tExpected result: 12-9-7-0-8-10-11-30-25-32-31-33-\n");

    free(preord);

    void* postord = bst_postorder(t);
    printf("\n\tPostorder: \t");
    for(size_t i=0; i < t.n_elements; i++){
        printf("%d-", * (int*) offset(postord, i, sizeof(int)));
    }
    printf("\n");
    printf("\tExpected result: 0-8-7-11-10-9-25-31-33-32-30-12-\n");
    
    free(postord);


    // Remove test
    bst_reset(&t);
    int nums2[] = {12, 5, 3, 23, 30};
    for(size_t i=0; i < sizeof(nums2) / sizeof(nums2[0]); i++){
        assert(bst_add(&t, &nums2[i]));
    }
    int cinco = 5, veintitres = 23;
    assert(bst_remove(&t, &cinco));
    assert(bst_remove(&t, &veintitres));

    void* remove_inord = bst_inorder(t);
    assert(3 == * (int*) remove_inord);
    assert(12 == * (int*) offset(remove_inord, 1, sizeof(int)));
    assert(30 == * (int*) offset(remove_inord, 2, sizeof(int)));

    free(remove_inord);

    bst_free(t);

    printf("[BSTree Test Finished]\n");
    return 0;
}