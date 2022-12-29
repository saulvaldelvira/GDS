#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int main(){
        List l = list_empty();
        for(int i=0; i < 1000000000; i++){
                int* tmp = calloc(1, sizeof(int));
                *tmp = i;
                list_append(&l, tmp);
        }
        
        list_free(l, FREE_ELEMENTS);
        return 1;
}
