#include "../src/Dictionary.h"

#define QUIET_DISABLE
#include "test.h"
#undef QUIET_DISABLE

#include <string.h>

void test_simple(){

        Dictionary *dic = dict_init(sizeof(int), sizeof(char), hash_int);
        int itmp = 1;
        char ctmp = 'A';
        dict_put(dic, &itmp, &ctmp);

        assert(dict_exists(dic, &itmp));
        char c;
        assert(dict_get(dic, &itmp, &c) != NULL);
        assert(c == ctmp);

        assert(dict_remove(dic, &itmp));

        assert(!dict_exists(dic, &itmp));
        assert(dict_get(dic, &itmp, &c) == NULL);
        dict_free(dic);
}

void brute(){
        print_test_step(Brute);
        int n = 1000;
        Dictionary *dic = dict_init(sizeof(int), sizeof(int), hash_int);
        assert(dic);
        for (int i = 0; i < n; i++){
                assert(dict_put(dic, &i, &i) == SUCCESS);
        }
        int tmp;
        for (int i = 0; i < n; i++){
                assert(i == * (int*) dict_get(dic, &i, &tmp));
                assert(dict_exists(dic, &i));
                assert(dict_remove(dic, &i));
                assert(!dict_exists(dic, &i));
        }
        for (int i = n; i < 2*n; i++){
                assert(dict_put(dic, &i, &i) == SUCCESS);
        }
        for (int i = n; i < 2*n; i++){
                assert(i == * (int*) dict_get(dic, &i, &tmp));
                assert(dict_exists(dic, &i));
                assert(dict_remove(dic, &i));
                assert(!dict_exists(dic, &i));
        }
        dict_free(dic);
        print_test_ok();
}

void config(){
        Dictionary *dic = dict_init(sizeof(int), sizeof(int), hash_int);

        assert(dict_configure(dic, DOUBLE_HASHING, 0.05, 0.15, NULL));

        dict_free(dic);

}

void random_test(){
        print_test_step(Random);
        int n = 100;
        srand(time(NULL));
        Dictionary *dic = dict_init(sizeof(int), sizeof(int), hash_int);
        int *exp = malloc(n * sizeof(int));
        int *exp2 = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++){
                int k = rand();
                int v = rand();
                exp[i] = k;
                exp2[i] = v;
                assert(dict_put(dic, &k, &v) == SUCCESS);
        }
        int tmp;
        for (int i = 0; i < n; i++){
                assert(dict_exists(dic, &exp[i]) == SUCCESS);
                assert(dict_get(dic, &exp[i], &tmp) != NULL);
                assert(tmp == exp2[i]);
        }
        free(exp);
        free(exp2);
        dict_free(dic);
        print_test_ok();
}

void string_test(){
        print_test_step(String);
        Dictionary *dic = dict_init(sizeof(char*), sizeof(int), hash_string);
        char *str[] = {"Hello world!", "this is a dictionary", ":p"};
        int strc = 3;
        for (int i = 0; i < strc; ++i){
                dict_put(dic, &str[i], &i);
        }
        int tmp;
        for (int i = 0; i < strc; ++i){
                dict_get(dic, &str[i], &tmp);
                assert(strcmp(str[i], str[tmp]) == 0);
                assert(tmp == i);
        }
        dict_free(dic);
        print_test_ok();
}

// Struct test

struct key{
        int i;
        char c;
        struct {
                float f;
                long l;
        } s;
};

struct person{
        char name[30];
        int age;
};

int64_t hash_structs(const void *e_1){
        struct key *k = (struct key*) e_1;
        int64_t hash = k->i << sizeof(k->c);
        hash += k->c;
        return hash;
}

void struct_test(){
        print_test_step(Struct);
        struct key k = {
                .i = 12,
                .c = 2
        };
        struct person p = {
                .age = 23
        };
        strcpy(p.name, "alejandro");

        Dictionary *d = dict_init(sizeof(struct key), sizeof(struct person), hash_structs);
        assert(dict_put(d, &k, &p) == SUCCESS);
        assert(dict_exists(d, &k));
        struct person per;
        assert(dict_get(d, &k, &per));
        assert(per.age == p.age);
        assert(strcmp(per.name, p.name) == 0);

        dict_free(d);
        print_test_ok();
}

int main(){
	print_test_start(Dictionary);
        TIMESTAMP_START

        test_simple();
        brute();
        config();
        random_test();
        string_test();
        struct_test();

	TIMESTAMP_STOP
	print_test_end(Dictionary);
        return 0;
}