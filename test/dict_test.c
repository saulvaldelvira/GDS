#include "../src/Dictionary/dictionary.h"

#define QUIET_DISABLE
#include "test.h"
#undef QUIET_DISABLE

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
}

void config(){
        Dictionary *dic = dict_init(sizeof(int), sizeof(int), hash_int);

        assert(dict_configure(dic, DOUBLE_HASHING, 0.05, 0.15, NULL));

        dict_free(dic);

}

void random_test(){
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
}

void string_test(){
        Dictionary *dic = dict_init(sizeof(char*), sizeof(int), hash_string);
        char *str[] = {"Hello world!", "this is a dictionary", ":p"};
        int strc = 3;
        for (int i = 0; i < strc; ++i){
                dict_put(dic, &str[i], &i);
        }
        int tmp;
        for (int i = 0; i < strc; ++i){
                dict_get(dic, &str[i], &tmp);
                printf("%-22s -> %d\n", str[i], tmp);
                assert(tmp == i);
        }
        dict_free(dic);
}

int main(){

	printf("[Starting Dictionary test]\n");

        TIMESTAMP_START

        test_simple();
        brute();
        config();
        random_test();
        string_test();

	TIMESTAMP_STOP
	END_MSG(Dictionary)
        return 0;
}