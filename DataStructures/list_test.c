#include <stdio.h>
#include <stdlib.h>
#include "List/array_list.h"
#include "List/linked_list.h"
#include <assert.h>
#include "comparator.h"

void list_test(){
        ArrayList l = arrlist_empty();
        for(int i=0; i < 1000000000; i++){
                int* tmp = calloc(1, sizeof(int));
                *tmp = i;
                arrlist_append(&l, tmp);
        }
        
        arrlist_free(l, FREE_ELEMENTS);
}

void int_float(){
        /// INTEGER
        LinkedList l = lnkd_list_init(Comparators.integer);
        // Because we use the values of numbers (wich are NOT allocated thus do not need to be freed) we have
        // to configure the list to NOT free elements in node remove neither in list remove
        lnkd_list_configure(&l, DONT_FREE_ON_NODE_REMOVE, DONT_FREE_ELEMENTS_ON_LIST_REMOVE);
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
        
        set_float_precision(0);
         // FLOAT
        LinkedList lfloat = lnkd_list_init(Comparators.floating);
        /*double floating[100];
        int  n = 0;
        for(double f=0; f<10.0f; f+=0.1f){
                floating[n] = f;
                assert(lnkd_list_append(&lfloat, &floating[n]));
                n++;
        }*/
        float f = 1.2f;
        float f2 = 1.1f;
        lnkd_list_append(&lfloat, &f);
        printf("EXPECTED 1: %d\n", lnkd_list_exists(lfloat, &f2));
        return 1;
}
