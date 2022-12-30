#include <stdio.h>
#include <stdlib.h>
#include "List/list.h"
#include "List/linked_list.h"

void list_test(){
        List l = list_empty();
        for(int i=0; i < 1000000000; i++){
                int* tmp = calloc(1, sizeof(int));
                *tmp = i;
                list_append(&l, tmp);
        }
        
        list_free(l, FREE_ELEMENTS);
}

/**
 * Returns 1 if e_1 is greater than e_2, 
 *        -1 if e_2 is greater than e_1,
 *         0 if they are equal
*/
bool compare_int(void *e_1, void *e_2){
        int n_1 = * (int *) e_1;
        int n_2 = * (int *) e_2;
        if (n_1 > n_2){
                return 1;
        }else if (n_2 > n_1){
                return -1;
        }else{
                return 0;
        }
}

int main(){
        LinkedList l = linked_list_init(*compare_int);
       
        int n = 12;
        linked_list_append(&l, &n);
        printf("TEST 1 -> %d\n", linked_list_exists(l, &n));
}
