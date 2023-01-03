#include "../src/ArrayList/array_list.h"
#include "../src/LinkedList/linked_list.h"
#include "test.h"

/**
 * test the lists with dynamic memory, wich means the free_on_delete option is set to 1.
*/
void dynamic_test(){
        int n = 1000;
        double tmp, arr_time, lnkd_time;

        printf("->Starting Dynamic test\n->ArrayList...\n");
        tmp = get_time();
        
        ArrayList arr = arrlist_empty(Comparators.integer);
        arrlist_configure(&arr, FREE_ON_DELETE);
        assert(arrlist_isempty(arr));
        for(int i=0; i < n; i++){
                assert(arrlist_append(&arr, alloc_int(i)));
                assert(arr.n_elements == i+1);
        }
        assert(!arrlist_isempty(arr));
        // Set test 1
        int temp = 50;
        int r = arrlist_set(&arr, &temp, alloc_int(-78));
        assert(r == temp);
        temp = -78;
        arrlist_set(&arr, &temp, alloc_int(50));
        //////////////////////////////////////////////

        for(int i=n-1; i >= 0; i--){
                assert(arrlist_exists(arr, &i));
                assert(arrlist_indexof(arr, &i) == i);
                assert(i == * (int*) arrlist_get_at(arr, i));
                assert(i == * (int*) arrlist_get(arr, &i));
                assert(arrlist_remove(&arr, &i));
        }
        assert(arr.n_elements == 0);

        // Set test 2
        int *one = alloc_int(1);
        int *two = alloc_int(2);
        int *three = alloc_int(3);

        assert(arrlist_append(&arr, one));
        assert(arrlist_set_at(&arr, 0, two));
        
        assert(*two == * (int*) arrlist_get_at(arr, 0));
        
        assert(arrlist_set(&arr, two, three) != INDEX_NOT_FOUND);
        assert(*three == * (int*) arrlist_get_at(arr, 0));
        /////////////////////////////////////////////////////

        arrlist_free(arr);

        arr_time = get_time() - tmp;

        printf("\tDone in %.3f seconds\n->Linked List...\n", arr_time);

        tmp = get_time();
        LinkedList lnked = lnkd_list_init(Comparators.integer);
        lnkd_list_configure(&lnked, FREE_ON_DELETE);
        assert(lnkd_list_isempty(lnked));
        
        for(int i=0; i < n; i++){
                assert(lnkd_list_append(&lnked, alloc_int(i)));
                assert(lnked.n_elements == i+1);
        }
        assert(!lnkd_list_isempty(lnked));
        for(int i=0; i < n; i++){
                assert(lnkd_list_exists(lnked, &i));
                assert(i == * (int *) lnkd_list_get(lnked, &i));
                assert(lnkd_list_remove(&lnked, &i));
        }
        assert(lnked.n_elements == 0);
        lnkd_list_free(lnked);

        lnkd_time = get_time() - tmp;

        printf("\tDone in %.3f seconds\n", lnkd_time);
        printf("->Dynamic test complete: Linked list is %.2f%% faster\n", 100.0 * (arr_time / lnkd_time));
}

int main(){
        printf("[List Test]\n");
        dynamic_test();
        printf("[List Test Finished]\n\n");
}
