#include "../src/ArrayList/array_list.h"
#include "../src/LinkedList/linked_list.h"

#define COMPARATOR_ENABLE
#define TIMESTAMP_ENABLE
#include "test.h"
#undef COMPARATOR_ENABLE
#undef TIMESTAMP_ENABLE

#include <stdint.h>

#include <memory.h>
struct test {
        int a;
        int b;
        char ch[];
};
int main(){
        int n = 2400;
        double arr_time, lnkd_time;

        printf("[List Test]\n");
        
        printf("->ArrayList...\n");
        TIMESTAMP_START
        
        ArrayList *arr = arrlist_empty(sizeof(int), compare_int);

        assert(arrlist_isempty(arr));
        for(int i=0; i < n; i++){
                assert(arrlist_append(arr, &i));
                assert(arrlist_n_elements(arr) == (size_t) i+1);
        }
        assert(!arrlist_isempty(arr));
        int menosveinte = -20, treinta = 30;
        assert(arrlist_indexof(arr, &menosveinte).status == -1);

        index_t ret = arrlist_indexof(arr, &treinta);

        assert(ret.status);
        assert(ret.value == (size_t) treinta);
        int tmp;
        for(int i=n-1; i >= 0; i--){
                assert(arrlist_exists(arr, &i));
                assert(arrlist_indexof(arr, &i).value == (size_t) i);
                assert(i == * (int*) arrlist_get_at(arr, i, &tmp));
                assert(i == * (int*) arrlist_get(arr, &i, &tmp));
                assert(arrlist_remove(arr, &i));
        }
        assert(arrlist_n_elements(arr) == 0);

        // Set test 2
        int one = 1;
        int two = 2;
        int three = 3;

        assert(arrlist_append(arr, &one));
        assert(arrlist_set_at(arr, 0, &two));
        
        assert(two == * (int*) arrlist_get_at(arr, 0, &tmp));
        
        assert(arrlist_set(arr, &two, &three) != INDEX_OUT_OF_BOUNDS);
        assert(three == * (int*) arrlist_get_at(arr, 0, &tmp));
        /////////////////////////////////////////////////////

        arrlist_free(arr);

        TIMESTAMP_STOP
        arr_time = timestamp;

        printf("\tDone in %.3f seconds\n->Linked List...\n", arr_time);

        TIMESTAMP_START

        LinkedList *lnked = lnkd_list_init(sizeof(int), compare_int);
        
        assert(lnkd_list_isempty(lnked));
        
        for(int i=0; i < n; i++){
                assert(lnkd_list_push_back(lnked, &i));
                assert(lnkd_list_n_elements(lnked) == (size_t) i+1);
        }

        /*PUSH FRONT*/
        int menos150 = -150;
        assert(lnkd_list_push_front(lnked, &menos150));
        assert(lnkd_list_n_elements(lnked) == 1UL + n);
        assert(lnkd_list_remove(lnked, &menos150));

        // Remove the last element
        assert(!lnkd_list_isempty(lnked));
        int last = n-1;
        assert(lnkd_list_remove(lnked, &last));
        assert(lnkd_list_n_elements(lnked) == (size_t) n-1);
        assert(lnkd_list_push_back(lnked, &last));
        ///////////////////////////////////////////////////
        
        for(int i=0; i < n; i++){
                assert(lnkd_list_exists(lnked, &i));
                assert(i == * (int *) lnkd_list_get(lnked, &i, &tmp));
                assert(lnkd_list_remove(lnked, &i));
        }
        assert(lnkd_list_n_elements(lnked) == 0);
        lnkd_list_free(lnked);

        TIMESTAMP_STOP
        lnkd_time = timestamp;

        printf("\tDone in %.3f seconds\n", lnkd_time);

        double total = lnkd_time + arr_time;

        timestamp += total;



        END_MSG(List)
        return 0;
}
