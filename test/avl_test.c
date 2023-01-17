#include "test.h"
#include "../src/AVLTree/AVLTree.h"

void print_preord(AVLTree *tree){
        void *preord = avl_preorder(tree);
        for (size_t i = 0; i < avl_size(tree); i++){
                void *tmp = void_offset(preord, i * sizeof(int));
                printf("%d-",* (int*) tmp);
        }
        printf("\n");
        free(preord);
}

void assert_preord(AVLTree *tree, int exp[]){
        void *preord = avl_preorder(tree);
        for (size_t i = 0; i < avl_size(tree); i++){
                void *tmp = void_offset(preord, i * sizeof(int));
                assert(exp[i] == * (int*) tmp);
        }
        free(preord);
}

int main(){
        int n = 1200;
        printf("[AVLTree Test]\n");
        AVLTree *t = avl_init(sizeof(int), compare_int);
        assert(avl_size(t) == 0UL);
        assert(avl_isempty(t));

        /*for (int i = 0; i < 1200; i++){
                assert(avl_add(t, &i) == SUCCESS);
                assert(avl_exists(t, &i));
                assert(avl_size(t) == 1UL + i);
        }*/
        (void) n;

        int nums[] = {10, 6, 15, 3, 9, 14, 20, 2, 4, 7, 12};

        int i;
        for (i = 0; i < 7; i++){
                assert(avl_add(t, &nums[i]));
        }

        int exp1 [] = {10, 6, 3, 9, 15, 14, 20};
        assert_preord(t, exp1);

        assert(!avl_isempty(t));

        for (; i < 11; i++){
                assert(avl_add(t, &nums[i]));
        }
        int exp2 [] = {10, 6, 3, 2, 4, 9, 7, 15, 14, 12, 20};
        assert_preord(t, exp2);

        assert(avl_add(t, cast_int(1)));
        int exp3 [] = {10, 6, 3, 2, 1, 4, 9, 7, 15, 14, 12, 20};
        assert_preord(t, exp3);

        assert(avl_remove(t, cast_int(20)));
        int exp4 [] = {6, 3, 2, 1, 4, 10, 9, 7, 14, 12, 15};
        assert_preord(t, exp4);

        assert(avl_remove(t, cast_int(4)));
        int exp5 [] = {6, 2, 1, 3, 10, 9, 7, 14, 12, 15};
        assert_preord(t, exp5);

        assert(avl_remove(t, cast_int(10)));
        int exp6 [] = {6, 2, 1, 3, 9, 7, 14, 12, 15};
        assert_preord(t, exp6);

        assert(avl_remove(t, cast_int(9)));
        int exp7 [] = {6, 2, 1, 3, 14, 7, 12, 15};
        assert_preord(t, exp7);

        assert(avl_remove(t, cast_int(6)));
        int exp8 [] = {3, 2, 1, 14, 7, 12, 15};
        assert_preord(t, exp8);

        assert(avl_remove(t, cast_int(3)));
        int exp9 [] = {7, 2, 1, 14, 12, 15};
        assert_preord(t, exp9);

        avl_free(t);

        printf("[AVLTree Test Finshed]\n");
        return 0;
}