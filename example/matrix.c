/**
 * Prints a matrix filled with random numbers.
 * The dimension can be given as a command line parameter (default: 10)
 * $ gcc matrix.c ../src/Vector.c ../src/util/compare.c -o matrix
 * $ ./matrix 7
 * Output:
 *      83 86 77 15 93 35 86
 *      92 49 21 62 27 90 59
 *      63 26 40 26 72 36 11
 *      68 67 29 82 30 62 23
 *      67 35 29  2 22 58 69
 *      67 93 56 11 42 29 73
 *      21 19 84 37 98 24 15
*/
#include "../src/Vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void destroy_vec(void *arg){
        Vector *v = * (Vector**) arg;
        vector_free(v);
}

void matrix_init(void *element, void *args){
        Vector **v = (Vector**) element;
        *v = vector_init(sizeof(int), compare_int);
        int size = * (int*) args;
        vector_reserve(*v, size);
}

int main(int argc, char *argv[]){
        int dim = 10;
        if (argc > 1)
                dim = atoi(argv[1]);
        srand(time(0));

        Vector *matrix = vector_init(sizeof(Vector*), compare_equal);
        vector_set_destructor(matrix, destroy_vec);
        vector_reserve(matrix, dim);
        vector_map(matrix, matrix_init, &dim);

        Vector *row;
        for (int i = 0; i < dim; i++){
                vector_at(matrix, i, &row);
                for (int j = 0; j < dim; j++){
                        int value = rand() % 100;
                        vector_set_at(row, j, &value);
                }
        }

        int *values = malloc(dim * sizeof(int));
        for (int i = 0; i < dim; i++){
                vector_at(matrix, i, &row);
                vector_get_into_array(row, values, dim);
                for (int j = 0; j < dim; j++)
                        printf("%2d ", values[j]);
                printf("\n");
        }
        free(values);
        vector_free(matrix);
}
