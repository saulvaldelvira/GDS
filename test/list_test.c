#include "../src/ArrayList/array_list.h"
#include "../src/LinkedList/linked_list.h"

#define COMPARATOR_ENABLE
#define TIMESTAMP_ENABLE
#include "test.h"
#undef COMPARATOR_ENABLE
#undef TIMESTAMP_ENABLE

#include <stdint.h>

void literals(){
        size_t n = 10000;
        printf("->Testing literals...\n");
        TIMESTAMP_START
        ArrayList arr = arrlist_empty(Comparator.literal_integer);
        
        for (size_t i=0; i < n; i++){
                assert(arrlist_append(&arr, (void*) i));
                assert(i+1 == arr.n_elements);
        }
        for (size_t i=0; i < n; i++){
                assert(arrlist_exists(arr, (void*) i));
                assert(arrlist_remove(&arr, (void*) i).status);
                assert((n-i-1) == arr.n_elements);
        }

        LinkedList lnk = lnkd_list_init(Comparator.literal_integer);
        for (size_t i=0; i < n; i++){
                assert(lnkd_list_push_back(&lnk, (void*) i));
                assert(i+1 == lnk.n_elements);
        }
        for (size_t i=0; i < n; i++){
                assert(lnkd_list_exists(lnk, (void*) i));
                assert(lnkd_list_remove(&lnk, (void*) i));
                assert((n-i-1) == lnk.n_elements);
        }


        TIMESTAMP_STOP
        printf("\tDone in %.3f seconds\n", timestamp);
}

int main(){
        int n = 10000;
        double arr_time, lnkd_time;

        printf("[List Test]\n");

        printf("->ArrayList...\n");
        TIMESTAMP_START
        
        ArrayList arr = arrlist_empty(Comparator.integer);
        arrlist_configure(&arr, FreeOnDelete);
        assert(arrlist_isempty(arr));
        for(intptr_t i=0; i < n; i++){
                assert(arrlist_append(&arr, alloc_int(i)));
                assert(arr.n_elements == (size_t) i+1);
        }
        assert(!arrlist_isempty(arr));
        int menosveinte = -20, treinta = 30;
        assert(arrlist_indexof(arr, &menosveinte).status == -1);

        index_t ret = arrlist_indexof(arr, &treinta);
        assert(ret.status);
        assert(ret.value.index == (size_t) treinta);

        // Set test 1
        int temp = 50;
        int r = arrlist_set(&arr, &temp, alloc_int(-78)).value.index;
        assert(r == temp);
        temp = -78;
        arrlist_set(&arr, &temp, alloc_int(50));
        //////////////////////////////////////////////

        for(int i=n-1; i >= 0; i--){
                assert(arrlist_exists(arr, &i));
                assert(arrlist_indexof(arr, &i).value.index == (size_t) i);
                assert(i == * (int*) arrlist_get_at(arr, i));
                assert(i == * (int*) arrlist_get(arr, &i));
                assert(arrlist_remove(&arr, &i).status);
        }
        assert(arr.n_elements == 0);

        // Set test 2
        int *one = alloc_int(1);
        int *two = alloc_int(2);
        int *three = alloc_int(3);

        assert(arrlist_append(&arr, one));
        assert(arrlist_set_at(&arr, 0, two).status);
        
        assert(*two == * (int*) arrlist_get_at(arr, 0));
        
        assert(arrlist_set(&arr, two, three).status != INDEX_OUT_OF_BOUNDS);
        assert(*three == * (int*) arrlist_get_at(arr, 0));
        /////////////////////////////////////////////////////

        arrlist_free(arr);

        TIMESTAMP_STOP
        arr_time = timestamp;

        printf("\tDone in %.3f seconds\n->Linked List...\n", arr_time);

        TIMESTAMP_START

        LinkedList lnked = lnkd_list_init(Comparator.integer);
        lnkd_list_configure(&lnked, FreeOnDelete);
        assert(lnkd_list_isempty(lnked));
        
        for(int i=0; i < n; i++){
                assert(lnkd_list_push_back(&lnked, alloc_int(i)));
                assert(lnked.n_elements == (size_t) i+1);
        }

        /*PUSH FRONT*/
        assert(lnkd_list_push_front(&lnked, alloc_int(-150)));
        assert(lnked.n_elements == 1UL + n);
        int menos150 = -150;
        assert(lnkd_list_remove(&lnked, &menos150));

        // Remove the last element
        assert(!lnkd_list_isempty(lnked));
        int last = n-1;
        assert(lnkd_list_remove(&lnked, &last));
        assert(lnked.n_elements == (size_t) n-1);
        assert(lnkd_list_push_back(&lnked, alloc_int(last)));
        ///////////////////////////////////////////////////
        
        for(int i=0; i < n; i++){
                assert(lnkd_list_exists(lnked, &i));
                assert(i == * (int *) lnkd_list_get(lnked, &i));
                assert(lnkd_list_remove(&lnked, &i));
        }
        assert(lnked.n_elements == 0);
        lnkd_list_free(lnked);

        TIMESTAMP_STOP
        lnkd_time = timestamp;

        printf("\tDone in %.3f seconds\n", lnkd_time);

        double total = lnkd_time + arr_time;


        literals();

        timestamp += total;

        END_MSG(List)
        return 0;
}
