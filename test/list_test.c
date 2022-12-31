#include <stdio.h>
#include <stdlib.h>
#include "../src/ArrayList/array_list.h"
#include "../src/LinkedList/linked_list.h"
#include <assert.h>
#include "../src/Util/allocate.h"
#include "../src/Util/comparator.h"

void dynamic_test(){
        int n = 10000;
        printf("[Starting Dynamic test]\nArrayList...");
        ArrayList arr = arrlist_empty(Comparators.integer);
        arrlist_configure(&arr, FREE_ON_DELETE);
        for(int i=0; i < n; i++){
                assert(arrlist_append(&arr, alloc_integer(i)));
                assert(arr.n_elements == i+1);
        }
        for(int i=n-1; i >= 0; i--){
                assert(arrlist_exists(arr, &i));
                assert(arrlist_indexof(arr, &i) == i);
                assert(i == * (int*) arrlist_get_at(arr, i));
                assert(i == * (int*) arrlist_get(arr, &i));
                assert(arrlist_remove(&arr, &i));
        }
        assert(arr.n_elements == 0);

        
        int *one = alloc_integer(1);
        int *two = alloc_integer(2);
        int *three = alloc_integer(3);

        assert(arrlist_append(&arr, one));
        assert(arrlist_set_at(&arr, 0, two));
        
        assert(*two == * (int*) arrlist_get_at(arr, 0));
        
        assert(arrlist_set(&arr, two, three) != INDEX_NOT_FOUND);
        assert(*three == * (int*) arrlist_get_at(arr, 0));

        arrlist_free(arr);

        printf("Done\nLinked List...");

        LinkedList lnked = lnkd_list_init(Comparators.integer);
        lnkd_list_configure(&lnked, FREE_ON_DELETE);
        for(int i=0; i < n; i++){
                int* tmp = calloc(1, sizeof(int));
                *tmp = i;
                assert(lnkd_list_append(&lnked, tmp));
                assert(lnked.n_elements == i+1);
        }
        for(int i=0; i < n; i++){
                assert(lnkd_list_exists(lnked, &i));
        }
        for(int i=0; i < n; i++){
                assert(lnkd_list_remove(&lnked, &i));
        }
        assert(lnked.n_elements == 0);
        lnkd_list_free(lnked);
        printf("Done\n");
        printf("[Dynamic test complete]\n");
}

void int_test(){
        /// INTEGER
        LinkedList l = lnkd_list_init(Comparators.integer);
        // Because we use the values of numbers (wich are NOT allocated thus do not need to be freed) we have
        // to configure the list to NOT free elements in node remove neither in list remove
        lnkd_list_configure(&l, DONT_FREE_ON_DELETE);
        int numbers[300];
        assert(l.n_elements == 0);
        for(int i=0; i < 300; i++){
                numbers[i] = i;
                assert(lnkd_list_append(&l, &numbers[i]));
                assert(l.n_elements == i+1);
        }
        for(int i=0; i < 300; i++){
                assert(lnkd_list_exists(l, &numbers[i]));
        }
        for(int i=0; i < 300; i++){
                assert(lnkd_list_remove(&l, &numbers[i]));
                assert(l.n_elements == 299 - i);
        }
        for(int i=0; i < 300; i++){
                assert(!lnkd_list_exists(l, &numbers[i]));
        }
        lnkd_list_free(l);
}

int main(){
        dynamic_test();
}
