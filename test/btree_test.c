#define QUIET_DISABLE
#include "test.h"
#undef QUIET_DISABLE

#include "../src/BTree/BTree.h"

int main(){
        /*BTree *tree2 = btree_init(sizeof(int), 4, compare_int);
        assert(btree_add(tree2, cast_int(1)));
        assert(btree_add(tree2, cast_int(5)));
        assert(btree_add(tree2, cast_int(9)));
        assert(btree_add(tree2, cast_int(12)));

        assert(btree_add(tree2, cast_int(19)));
        assert(btree_add(tree2, cast_int(20)));

        assert(btree_add(tree2, cast_int(27)));
        assert(btree_add(tree2, cast_int(35)));
        assert(btree_add(tree2, cast_int(28)));
        assert(btree_add(tree2, cast_int(22)));


        assert(btree_exists(tree2, cast_int(1)));
        assert(btree_exists(tree2, cast_int(5)));
        assert(btree_exists(tree2, cast_int(9)));
        assert(btree_exists(tree2, cast_int(12)));
        assert(btree_exists(tree2, cast_int(19)));
        assert(btree_exists(tree2, cast_int(20)));

        assert(btree_exists(tree2, cast_int(28)));
        assert(btree_exists(tree2, cast_int(22)));
        
        assert(btree_exists(tree2, cast_int(27)));
        assert(btree_exists(tree2, cast_int(35)));
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