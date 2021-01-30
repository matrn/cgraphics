#ifndef __MATRIX_H__
#define __MATRIX_H__


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MATRIX_TYPE double
#define GRAYSCALE_MIN 0
#define GRAYSCALE_MAX 9

#define RATIO 7.0/15.0
#define RATIO_INV 15.0/7.0



typedef struct Matrix {
	uint16_t m;   //rows
	uint16_t n;   //cols
	MATRIX_TYPE *matrix;
} matrix_t;

typedef int8_t sbyte;


static const char grayscale_letters[] = " .:-=+*#%@";


void matrix_print(matrix_t input, bool print_size);
void matrix_draw(matrix_t input);
void matrix_alloc(matrix_t *input, int m, int n);
void matrix_free(matrix_t input);


void matrix_zero(matrix_t *input);

sbyte matrix_scale_and_add(int scalar1, matrix_t m1, int scalar2, matrix_t m2, matrix_t *matrix_out);

sbyte matrix_add(matrix_t m1, matrix_t m2, matrix_t *matrix_out);
sbyte matrix_sub(matrix_t m1, matrix_t m2, matrix_t *matrix_out);
sbyte matrix_mult(matrix_t matrix1, matrix_t matrix2, matrix_t *matrix_out);
sbyte vectors_to_terminal_matrix(matrix_t vectors, matrix_t *terminal, int max_index);
sbyte matrix_vectorize(matrix_t matrix, matrix_t *vectors);

sbyte matrix_set(matrix_t *matrix, int x, int y, MATRIX_TYPE value);

sbyte matrix_copy(matrix_t matrix1, matrix_t *matrix_out);
bool matrix_add_vector(matrix_t *vectors, int *index, int x, int y, int z, int grayscale);

char grayscale_to_char(int input, int minv, int maxv);


#endif