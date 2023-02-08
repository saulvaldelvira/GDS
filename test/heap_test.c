#define QUIET_DISABLE
#include "test.h"
#undef QUIET_DISABLE

#include "../src/BinaryHeap/binary_heap_min.h"



void filter_up(){
        MinHeap *min = minheap_init(sizeof(int), compare_int);
        minheap_add(min, cast_int(10));
        minheap_add(min, cast_int(9));
        minheap_add(min, cast_int(8));

        /// Config test
        minheap_configure(min, compare_allways_true);
        assert(minheap_exists(min, cast_int(-15454)));
        minheap_configure(min, compare_int);
        ////////////

        int exp1[] = {8, 10, 9};
        int *res = minheap_get_array(min, 3);
        assert_array_int(res, exp1, 3);
        free(res);

        minheap_add(min, cast_int(7));
        int exp2[] = {7, 8, 9, 10};
        res = minheap_get_array(min, 4);
        assert_array_int(res, exp2, 4);
        free(res);

        minheap_add(min, cast_int(6));
        int exp3[] = {6, 7, 9, 10, 8};
        res = minheap_get_array(min, 5);
        assert_array_int(res, exp3, 5);
        free(res);

        minheap_add(min, cast_int(5));
        int exp4[] = {5, 7, 6, 10, 8, 9};
        res = minheap_get_array(min, 6);
        assert_array_int(res, exp4, 6);
        free(res);

        minheap_add(min, cast_int(4));
        int exp5[] = {4, 7, 5, 10, 8, 9, 6};
        res = minheap_get_array(min, 7);
        assert_array_int(res, exp5, 7);
        free(res);

        minheap_add(min, cast_int(3));
        int exp6[] = {3, 4, 5, 7, 8, 9, 6, 10};
        res = minheap_get_array(min, 8);
        assert_array_int(res, exp6, 8);
        free(res);

        minheap_add(min, cast_int(2));
        int exp7[] = {2, 3, 5, 4, 8, 9, 6, 10, 7};
        res = minheap_get_array(min, 9);
        assert_array_int(res, exp7, 9);
        free(res);

        minheap_add(min, cast_int(1));
        int exp8[] = {1, 2, 5, 4, 3, 9, 6, 10, 7, 8};
        res = minheap_get_array(min, 10);
        assert_array_int(res, exp8, 10);
        free(res);

        minheap_free(min);

        // CHAR TEST
        min = minheap_init(sizeof(char), compare_char);
        
        char input[] = {'f', 'g', 'a', 'z', 'd'};
        minheap_add_array(min, input, 5);
        char expchar[] = {'a', 'd', 'f', 'z', 'g'};
        char* result = minheap_get_array(min, GET_ALL_ELEMENTS);
        assert_array_char(result, expchar, 5);
        free(result);
        char dest;
        assert('a' == * (char*) minheap_pop_min(min, &dest));
        char expchar2[] = {'d', 'g', 'f', 'z'};
        result = minheap_get_array(min, GET_ALL_ELEMENTS);
        assert_array_char(result, expchar2, 4);
        free(result);
        
        minheap_free(min);
}

void pop_min(){
        MinHeap *min = minheap_init(sizeof(int), compare_int);
        minheap_add(min, cast_int(200));
        minheap_add(min, cast_int(105));
        minheap_add(min, cast_int(1));

        int exp[] = {1, 105, 200};
        int index = 0;
        int tmp;
        while (!minheap_isempty(min)){
                int value = * (int*) minheap_pop_min(min, &tmp);
                assert(exp[index++] == value);
        }

        minheap_free(min);

        min = minheap_init(sizeof(char), compare_char);
        char input[] = {'Z', 'X', 'R', 'P', 'O', 'G', 'E', 'D', 'B', 'A'};
        minheap_add_array(min, input, 10);
        char exp2[] = {'A','B','D','E','G','O','P','R','X','Z'};
        index = 0;
        char c;
        while (!minheap_isempty(min)){
                char value = * (char*) minheap_pop_min(min, &c);
                assert(exp2[index++] == value);
        }

        minheap_free(min);
}

int main(){
        printf("[Binary Heap Test]\n");
        TIMESTAMP_START

        MinHeap *min = minheap_init(sizeof(int), compare_int);
        assert(minheap_isempty(min));

        int nums[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
        minheap_add_array(min, nums, 10);

        int *res = minheap_get_array(min, 10);
        int exp1[] = {1, 2, 4, 3, 6, 5, 8, 10, 7, 9};
        assert_array_int(res, exp1, 10);
        free(res);

        min = minheap_reset(min);

        int nums2[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
        minheap_add_array(min, nums2, 11);

        int exp2[] = {0, 1, 4, 2, 6, 5, 8, 3, 7, 9, 10};
        res = minheap_get_array(min, 11);
        assert_array_int(res, exp2, 11);
        free(res);

        minheap_free(min);

        filter_up();

        pop_min();


        TIMESTAMP_STOP
        END_MSG(BinaryHeap)
        return 0;
}