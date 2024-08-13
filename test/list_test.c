#include "../include/linked_list.h"
#include "test.h"
#include <stdio.h>

void joins_test(void){
	linked_list_t *l1 = list_init(sizeof(int), compare_int);
	linked_list_t *l2 = list_init(sizeof(int), compare_int);

	for (int i = 0; i < 10; i++)
		list_append(l1, &i);

	for (int i = 10; i < 20; i++)
		list_append(l2, &i);

	linked_list_t *lnkd_joint = list_join(l1, l2);
	assert(lnkd_joint != NULL);
	for (int i = 0; i < 20; i++)
		assert(list_exists(lnkd_joint, &i));

	list_free(l1, l2, lnkd_joint);
}

void destructor_test(void){
	linked_list_t *list = list_init(sizeof(int*), compare_pointer);
	list_set_destructor(list, destroy_ptr);
	int *ptr;
	for (int i = 0; i < 1024; i++){
		ptr = malloc(sizeof(int));
		assert(list_append(list, &ptr) == GDS_SUCCESS);
	}

	int *ptr2 = malloc(sizeof(int));
	list_set(list, &ptr, &ptr2);
	list_free(list);
}

/* Make sure that, when removing elements from the
   back and front, the edge case in which the head
   and tail are the same is correctly handled */
void remove_back_and_front(void){
        int tmpdest;
	linked_list_t *list = list_init(sizeof(int), compare_pointer);
        list_append(list, &(int){1});
        list_remove_front(list);
        assert(list_get_back(list, &tmpdest) == NULL);

        list_append(list, &(int){1});
        list_remove_back(list);
        assert(list_get_front(list, &tmpdest) == NULL);

        list_append(list, &(int){1});
        list_pop_front(list, NULL);
        assert(list_get_back(list, &tmpdest) == NULL);

        list_append(list, &(int){1});
        list_pop_back(list, NULL);
        assert(list_get_front(list, &tmpdest) == NULL);
        list_free(list);
}

void iterator(void) {
        const int N = 1024;
	linked_list_t *l = list_init(sizeof(int), compare_int);

	for (int i = 0; i < N; i++)
		list_append(l, &i);

        list_iterator_t it = list_iterator(l);
        assert(!list_it_has_prev(&it));
        assert(list_it_has_next(&it));
        int tmp;
        assert(list_it_prev(&it, &tmp) == NULL);
        for (int i = 0; i < N; i++) {
                assert(list_it_has_next(&it));
                list_it_peek_next(&it, &tmp);
                assert(tmp == i);
                list_it_next(&it, &tmp);
                assert(tmp == i);

                assert(list_it_has_prev(&it));
                list_it_peek_prev(&it, &tmp);
                assert(tmp == i);
        }
        assert(list_it_has_prev(&it));
        assert(!list_it_has_next(&it));
        assert(list_it_next(&it, &tmp) == NULL);
        assert(list_it_prev(&it, &tmp) != NULL);
        assert(tmp == N - 1);

        it = list_iterator_from_back(l);
        assert(list_it_has_prev(&it));
        assert(!list_it_has_next(&it));
        assert(list_it_next(&it, &tmp) == NULL);
        for (int i = N - 1; i >= 0; i--) {
                assert(list_it_has_prev(&it));
                list_it_peek_prev(&it, &tmp);
                assert(tmp == i);
                list_it_prev(&it, &tmp);
                assert(tmp == i);

                assert(list_it_has_next(&it));
                list_it_peek_next(&it, &tmp);
                assert(tmp == i);
        }
        assert(!list_it_has_prev(&it));
        assert(list_it_has_next(&it));
        assert(list_it_prev(&it, &tmp) == NULL);
        assert(list_it_next(&it, &tmp) != NULL);
        assert(tmp == 0);

	list_free(l);
}

int main(void){
	int n = 2400;
	int tmp;
	test_start("linked_list.c");


	linked_list_t *lnked = list_init(sizeof(int), compare_int);

	assert(list_isempty(lnked));
	assert(list_get_front(lnked, &tmp) == NULL);
	assert(list_get_back(lnked, &tmp) == NULL);

	for(int i=0; i < n; i++){
		assert(list_append(lnked, &i));
		assert(list_size(lnked) == (size_t) i+1);
	}
	assert(n-1 == * (int*) list_get_back(lnked, &tmp));
	int* get_arr = list_get_array(lnked, list_size(lnked));
	int* get_into = malloc(n * sizeof(int));
	assert(get_into != NULL);
	list_get_into_array(lnked, get_into, list_size(lnked));
	for (int i = 0; i < n; i++){
		assert(i == get_arr[i]);
		assert(i == get_into[i]);
	}


	free(get_arr);
	free(get_into);

	/*PUSH FRONT*/
	assert(list_push_front(lnked, &(int){-150}));
	assert(list_size(lnked) == 1UL + n);
	assert(-150 == * (int*) list_get_front(lnked, &tmp));
	assert(list_remove_front(lnked));
	assert(!list_exists(lnked, &(int){-150}));

	assert(list_append(lnked, &(int){4525}));
	assert(list_remove_back(lnked));

	/// confign tests
	list_set_comparator(lnked, compare_equal);
	assert(list_exists(lnked, &(int){-684765}));
	list_set_comparator(lnked, compare_int);
	///////////////

	// Remove the last element
	assert(!list_isempty(lnked));
	int last = n-1;
	assert(list_remove(lnked, &last));
	assert(list_size(lnked) == (size_t) n-1);
	assert(list_append(lnked, &last));
	assert(list_size(lnked) == (size_t) n);
	///////////////////////////////////////////////////

	for(int i=0; i < n; i++){
		assert(list_exists(lnked, &i));
		assert(i == * (int *) list_get(lnked, &i, &tmp));
		assert(list_remove(lnked, &i));
	}
	assert(list_size(lnked) == 0);

        list_clear(lnked);

	int nums[] = {1, 2, 3, 4, 5};
	assert(list_append_array(lnked, nums, 5UL));
	assert(5UL == list_size(lnked));
	assert(list_remove_array(lnked, nums, 5UL));
	assert(list_isempty(lnked));
	assert(list_push_front_array(lnked, nums, 5UL));
	assert(5UL == list_size(lnked));

	list_free(lnked);

        remove_back_and_front();
	joins_test();
	destructor_test();
        iterator();

	test_end("linked_list.c");
	return 0;
}
