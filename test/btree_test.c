#define QUIET_DISABLE
#include "test.h"
#undef QUIET_DISABLE

#include "../src/BTree/BTree.h"

int main(){
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

        assert(btree_exists(tree, cast_int(4)));
        assert(btree_exists(tree, cast_int(10)));
        assert(btree_exists(tree, cast_int(7)));
        assert(btree_exists(tree, cast_int(6)));
        assert(btree_exists(tree, cast_int(20)));
        assert(btree_exists(tree, cast_int(15)));
        assert(btree_exists(tree, cast_int(5)));


        btree_free(tree);

        TIMESTAMP_STOP
        END_MSG(BTree)
        return 0;
}