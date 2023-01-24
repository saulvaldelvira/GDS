#define QUIET_DISABLE
#include "test.h"
#undef QUIET_DISABLE

#include "../src/BTree/BTree.h"

#define ARR_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

int main(){
        /*BTree *tree2 = btree_init(sizeof(int), 5, compare_int);
        int nums[] = {190, 57, 89, 90, 121, 170, 35, 48, 91, 22, 126, 132, 80};
        for (int i=0; i < (int) ARR_SIZE(nums); i++){
                assert(btree_add(tree2, &nums[i]));
        }
        for (int i=0; i < (int) ARR_SIZE(nums); i++){
                assert(btree_exists(tree2, &nums[i]));
        }

        btree_free(tree2);
        printf("oK\n");
        return 0;*/
        printf("[BTree Test]\n");
        TIMESTAMP_START

        BTree *tree = btree_init(sizeof(int), 3, compare_int);

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

        for (int i=1; i < 1020; i++){
                printf("%d\n", i);
                assert(btree_add(tree, &i));
                assert(btree_exists(tree, &i));
        }

        btree_free(tree);

        TIMESTAMP_STOP
        END_MSG(BTree)
        return 0;
}