#include "test.h"
#include "../src/BSTree/BSTree.h"

struct test{
    int *i;
    char *c;
};

int main(){
    int n = 10000, min = 0, max = 10;
    int nbuf[n];
    int ncount;
    printf("[List Test]\n");
    BSTree t = bst_init(Comparators.integer);
    bst_configure(&t, FREE_ON_DELETE);
    for(ncount=0; ncount<n; ncount+=2){
        nbuf[ncount] = rand_range(min, max);
        assert(bst_add(&t, alloc_int(ncount)));
    }
    for(ncount=1; ncount<n; ncount+=2){
        nbuf[ncount] = rand_range(min, max);
        assert(bst_add(&t, alloc_int(ncount)));
    }
    for(ncount=0; ncount<n; ncount++){
        assert(bst_remove(&t, &ncount));
    }
    /*
    for(ncount=0; ncount<n; ncount++){
        assert(bst_remove(&t, &nbuf[ncount]));
    }*/
    
    printf("[List Test Finished]\n");
    return 1;
}