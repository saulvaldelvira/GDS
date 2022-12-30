#include <stdio.h>
#include <stdlib.h>
#include "List/list.h"
#include "List/linked_list.h"
#include <assert.h>

void list_test(){
        List l = list_empty();
        for(int i=0; i < 1000000000; i++){
                int* tmp = calloc(1, sizeof(int));
                *tmp = i;
                list_append(&l, tmp);
        }
        
        list_free(l, FREE_ELEMENTS);
}

void int_float(){
        /// INTEGER

        LinkedList l = linked_list_init(Comparators.integer);
        // Because we use the values of numbers (wich are NOT allocated thus do not need to be freed) we have
        // to configure the list to NOT free elements in node remove neither in list remove
        linked_list_configure(&l, DONT_FREE_ON_NODE_REMOVE, DONT_FREE_ELEMENTS_ON_LIST_REMOVE);
        int numbers[300];
        assert(l.n_elements == 0);
        for(int i=0; i < 300; i++){
                numbers[i] = i;
                assert(linked_list_append(&l, &numbers[i]));
                assert(l.n_elements == i+1);
        }
        for(int i=0; i < 300; i++){
                assert(linked_list_exists(l, &numbers[i]));
        }
        for(int i=0; i < 300; i++){
                assert(linked_list_remove(&l, &numbers[i]));
                assert(l.n_elements == 299 - i);
        }
        for(int i=0; i < 300; i++){
                assert(!linked_list_exists(l, &numbers[i]));
        }

        linked_list_free(l);


}

int main(){
         // FLOAT
        LinkedList lfloat = linked_list_init(Comparators.floating);
        float floating[100];
        int  n = 0;
        for(float f=0; f<10.0; f+=0.1){
                floating[n] = f;
                printf("Adding: %2.1f at %d\n", floating[n], n);
                assert(linked_list_append(&lfloat, &floating[n]));
                n++;
        }
        float f = 1.2;
        printf("(%2.1f) EXPECTED 1: %d\n",floating[12], linked_list_exists(lfloat, &floating[12]));
        return 1;
}
