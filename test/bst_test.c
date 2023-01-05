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
    printf("[BSTree Test]\n");
    BSTree t = bst_init(Comparators.integer);
    // Random numbers test
    bst_configure(&t, DONT_FREE_ON_DELETE);
    assert(!bst_exists(t, &n));
    printf("Add...\n");
    for(ncount=0; ncount<n; ncount++){
        nbuf[ncount] = rand_range(min, max);
        assert(bst_add(&t, &nbuf[ncount]));
        assert(bst_exists(t, &nbuf[ncount]));
    }
    printf("Remove...\n");
    for(ncount=0; ncount<n; ncount++){
        bst_remove(&t, &nbuf[ncount]);
    }

    // Orders test
    bst_reset(&t);
    bst_configure(&t, FREE_ON_DELETE);
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

    void* *inord = bst_inorder(t);
    printf("Inorder: \t");
    for(int i=0; i < t.n_elements; i++){
        printf("%d-", * (int*) inord[i]);
    }
    printf("\n");
    printf("Expected result: 0-7-8-9-10-11-12-25-30-31-32-33-\n");

    free(inord);

    void* *preord = bst_preorder(t);
    printf("Preorder:  \t");
    for(int i=0; i < t.n_elements; i++){
        printf("%d-", * (int*) preord[i]);
    }
    printf("\n");
    printf("Expected result: 12-9-7-0-8-10-11-30-25-32-31-33-\n");

    free(preord);

    void* *postord = bst_postorder(t);
    printf("Postorder: \t");
    for(int i=0; i < t.n_elements; i++){
        printf("%d-", * (int*) postord[i]);
    }
    printf("\n");
    printf("Expected result: 0-8-7-11-10-9-25-31-33-32-30-12-\n");
    
    free(postord);


    // Remove test
    bst_reset(&t);
    t.free_on_delete = FREE_ON_DELETE;
    assert(bst_add(&t, alloc_int(12)));
    assert(bst_add(&t, alloc_int(5)));
    assert(bst_add(&t, alloc_int(3)));
    assert(bst_add(&t, alloc_int(23)));
    assert(bst_add(&t, alloc_int(30)));
    int cinco = 5, veintitres = 23;
    assert(bst_remove(&t, &cinco));
    assert(bst_remove(&t, &veintitres));

    void* *remove_inord = bst_inorder(t);
    assert(3 == * (int*) remove_inord[0]);
    assert(12 == * (int*) remove_inord[1]);
    assert(30 == * (int*) remove_inord[2]);

    printf("[BSTree Test Finished]\n");
    return 0;
}