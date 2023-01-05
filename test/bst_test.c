#include "test.h"
#include "../src/BSTree/BSTree.h"

struct test{
    int *i;
    char *c;
};

int main(){
    int n = 50000, min = 0, max = 10;
    int nbuf[n];
    int ncount;
    printf("[List Test]\n");
    BSTree t = bst_init(Comparators.integer);
    bst_configure(&t, FREE_ON_DELETE);
    printf("Add...\n");
    for(ncount=0; ncount<n; ncount+=2){
        nbuf[ncount] = rand_range(min, max);
        assert(bst_add(&t, alloc_int(ncount)));
    }
    for(ncount=1; ncount<n; ncount+=2){
        nbuf[ncount] = rand_range(min, max);
        assert(bst_add(&t, alloc_int(ncount)));
    }
    printf("Remove...\n");
    for(ncount=0; ncount<n; ncount++){
        assert(bst_remove(&t, &ncount));
    }

    // test serio
    bst_reset(&t);

    assert(bst_add(&t, alloc_int(12)));
    assert(bst_add(&t, alloc_int(9)));
    assert(bst_add(&t, alloc_int(7)));
    assert(bst_add(&t, alloc_int(10)));
    assert(bst_add(&t, alloc_int(0)));
    assert(bst_add(&t, alloc_int(8)));
    assert(bst_add(&t, alloc_int(11)));
    assert(bst_add(&t, alloc_int(30)));
    assert(bst_add(&t, alloc_int(25)));
    assert(bst_add(&t, alloc_int(32)));
    assert(bst_add(&t, alloc_int(31)));
    assert(bst_add(&t, alloc_int(33)));

    

    
    printf("[List Test Finished]\n");
    return 1;
}