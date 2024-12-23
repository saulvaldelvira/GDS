/* Silly demo program for the hash_map.
   Track people's age by their name.

   $ gcc hash_map_demo.c ../src/hash_map.c ../src/hash.c \
         ../src/vector.c ../src/compare.c -o hash_map_demo
   $ ./hash_map_demo
   */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "../include/hash_map.h"

int main(void){
	hash_map_t *map = hashmap_init(sizeof(char*), sizeof(int), hash_string, compare_string);
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
			hashmap_put(map, &k, &age);
			break;
		case 'r':
			hashmap_remove(map, &name);
			break;
		case 's':
			if (hashmap_get(map, &name, &age) == NULL)
				printf("The person [%s] doesn't exist\n", name);
			else
				printf("%s's age is %d\n", name, age);
			break;
		}
	}
	hashmap_free(map);
}
