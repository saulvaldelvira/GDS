#include "test.h"
#include "../src/BSTree/BSTree.h"

int main(){
    int n = 10000, min = -1200, max = 1200;
    int nbuf[2*1200];
    int ncount;
    printf("[List Test]\n");
    BSTree t = bst_init(Comparators.integer);

    for(ncount=0; ncount<n; ncount++){
        nbuf[ncount] = rand_range(min, max);
        bst_add(&t, &nbuf[ncount]);
    }
    for(ncount=0; ncount<n; ncount++){
        assert(bst_remove(&t, &nbuf[ncount]));
    }

    return 1;
}