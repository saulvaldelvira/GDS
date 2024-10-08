/* Silly demo program for the dictionary.
   Track people's age by their name.

   $ gcc dict_demo.c ../src/dictionary.c ../src/hash.c \
         ../src/vector.c ../src/compare.c -o dict_demo
   $ ./dict_demo
   */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "../include/dictionary.h"

int main(void){
	dictionary_t *dict = dict_init(sizeof(char*), sizeof(int), hash_string);
	for (;;){
		char option;
		printf("Options: (a)dd person, (r)emove person, (s)earch person, (e)xit.\n"
			"Select an option: ");
		do{
			option = getchar();
		}while (isspace(option));
		if (option == 'e') break;

		int age;
		char namebuf[1024];
		char *name = namebuf;
		printf("Name: ");
		scanf("%1024s", name);

		switch (option){
		case 'a':
			printf("Age: ");
			scanf("%d", &age);
                        char *k = malloc(1024);
                        strcpy(k,name);
			dict_put(dict, &k, &age);
			break;
		case 'r':
			dict_remove(dict, &name);
			break;
		case 's':
			if (dict_get(dict, &name, &age) == NULL)
				printf("The person [%s] doesn't exist\n", name);
			else
				printf("%s's age is %d\n", name, age);
			break;
		}
	}
	dict_free(dict);
}
