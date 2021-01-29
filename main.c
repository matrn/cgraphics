#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "utils.h"
#include "matrix.h"



int main (int argc, char **argv){
    //DEBUG_PRINT("Debug level: %d", (int) DEBUG);

	term_info_t term_info = get_terminal_info();
	printf("width: %d, height: %d\n", term_info.width, term_info.height);

	matrix_t matrix;
	matrix_t m1;
	matrix_t m2;

	matrix_t terminal;
	matrix_t vectors;

	//int image_size = min(term_info.width, term_info.height);
	//int n = image_size;


	matrix_alloc(&terminal, term_info.height, term_info.width);
	matrix_alloc(&vectors, 3, term_info.width*term_info.height);


	//printf(">%d<\n", grayscale_to_char(100, -100, 100));

	double r = min(term_info.width, term_info.height)/2.0;

	for(int x = -r; x < r; x ++){
		int y = sqrt(r*r - x*x);
		vectors.matrix[0*vectors.n + x] = x;
		vectors.matrix[1*vectors.n + x] = y;
	}
/*
	for(int i = 0; i < m1.m*m1.n; i ++){
		m1.matrix[i] = i+1;
		m2.matrix[i] = i;
	}

	matrix_sub(m1, m2, &matrix);
	matrix_print(matrix, true);
*/
	clear_terminal();
	vectors_to_terminal_matrix(vectors, &terminal);
	matrix_draw(terminal);


	matrix_free(matrix);
	matrix_free(m1);
	matrix_free(m2);

	return 0;
}