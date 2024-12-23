#include "../include/hash_map.h"
#include "test.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_simple(void){
        hash_map_t *dic = hashmap_init(sizeof(int), sizeof(char), hash_int, compare_int);
        int itmp = 1;
        char ctmp = 'A';
        hashmap_put(dic, &itmp, &ctmp);

        assert(hashmap_exists(dic, &itmp));
        char c;
        assert(hashmap_get(dic, &itmp, &c) != NULL);
        assert(c == ctmp);

        assert(hashmap_remove(dic, &itmp));

        assert(!hashmap_exists(dic, &itmp));
        assert(hashmap_get(dic, &itmp, &c) == NULL);
        hashmap_free(dic);
}

void brute(void){
        test_step("Brute");
        int n = 1000;
        hash_map_t *dic = hashmap_init(sizeof(int), sizeof(int), hash_int, compare_int);
        assert(dic);
        for (int i = 0; i < n; i++){
                int s = hashmap_put(dic, &i, &i);
                assert(s == GDS_SUCCESS);
        }
        int tmp;
        for (int i = 0; i < n; i++){
                assert(i == * (int*) hashmap_get(dic, &i, &tmp));
                assert(hashmap_exists(dic, &i));
                assert(hashmap_remove(dic, &i));
                assert(!hashmap_exists(dic, &i));
        }
        hashmap_free(dic);
        test_ok();
}

void config(void){
        hash_map_t *dic = hashmap_init(sizeof(int), sizeof(int), hash_int, compare_int);

        assert(hashmap_configure(dic, DOUBLE_HASHING, 0.05, 0.15));

        hashmap_free(dic);

}

void random_test(void){
        test_step("Random");
        int n = 2048;
        srand(time(0));

        // Create 2 arrays for the keys and
        // values, and shuffle them randomly
        int *keys = int_array(0,n);
        int *values = int_array(0,n);
        shuffle_array(keys, n);
        shuffle_array(values, n);

        hash_map_t *dic = hashmap_init(sizeof(int), sizeof(int), hash_int, compare_int);
        int *exp = malloc(n * sizeof(int));
        int *exp2 = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++){
                int k = keys[i];
                int v = values[i];
                exp[i] = k;
                exp2[i] = v;
                assert(hashmap_put(dic, &k, &v) == GDS_SUCCESS);
        }
        int tmp;
        for (int i = 0; i < n; i++){
                assert(hashmap_exists(dic, &exp[i]) == GDS_SUCCESS);
                assert(hashmap_get(dic, &exp[i], &tmp) != NULL);
                assert(tmp == exp2[i]);
        }
        free(exp);
        free(exp2);
        free(keys);
        free(values);
        hashmap_free(dic);
        test_ok();
}

void string_test(void){
        test_step("String");
        hash_map_t *map = hashmap_init(sizeof(char*), sizeof(int), hash_string, compare_string);

        const int n = 100;
        char *strs[n];

        for (int i = 0; i < n; i++) {
                char *str = malloc(1000);
                strs[i] = str;
                sprintf(str, "%d", i);
                hashmap_put(map, &str, &i);
        }

        for (int i = 0; i < n; i++) {
                int tmp;
                assert(hashmap_exists(map,&strs[i]));
                assert(i == * (int*) hashmap_get(map,&strs[i],&tmp));
        }
        for (int i = 0; i < n; i++) {
                free(strs[i]);
        }
        hashmap_free(map);
        test_ok();
}

// Struct test

struct key{
        int i;
        char c;
};

struct person{
        char name[30];
        int age;
};

int64_t hash_structs(const void *e_1){
        struct key *k = (struct key*) e_1;
        int64_t hash = (int64_t)k->i << sizeof(k->c);
        hash += k->c;
        return hash;
}

int compare_structs(const void *e_1, const void *e_2){
        struct key *k1 = (struct key*) e_1;
        struct key *k2 = (struct key*) e_2;
        if (k1->i == k2->i && k1->c == k2->c)
                return 0;
        return 1;
}

void struct_test(void){
        test_step("Struct");
        struct key k = {
                .i = 12,
                .c = 2
        };
        struct person p = {
                .age = 23
        };
        strcpy(p.name, "alejandro");

        hash_map_t *d = hashmap_init(sizeof(struct key), sizeof(struct person), hash_structs, compare_structs);
        assert(hashmap_put(d, &k, &p) == GDS_SUCCESS);
        assert(hashmap_exists(d, &k));
        struct person per;
        assert(hashmap_get(d, &k, &per));
        assert(per.age == p.age);
        assert(strcmp(per.name, p.name) == 0);

        hashmap_free(d);
        test_ok();
}

void destructor_test(void){
	hash_map_t *map = hashmap_init(sizeof(int), sizeof(char*), hash_int, compare_int);
	hashmap_set_destructor(map, destroy_ptr);

        char *tmp = NULL;
        assert(hashmap_get(map, &(int){12}, &tmp) == NULL);

	for (int i = 0; i < 120; i++){
		char *str = malloc(sizeof(char[12]));
		assert(str);
		sprintf(str, "%d", i);
		hashmap_put(map, &i, &str);
	}

	// Make sure the pointers are also
	// freed when we overwrite the values
	char *str = malloc(sizeof(char[12]));
	hashmap_put(map, &(int){30}, &str);

	hashmap_free(map);
}

int main(void){
	test_start("hash_map.c");


        test_simple();
        brute();
        config();
        random_test();
        string_test();
        struct_test();
	destructor_test();


	test_end("hash_map.c");
        return 0;
}
