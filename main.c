#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "utils.h"
#include "matrix.h"


#define RATIO 7.0/15.0
#define RATIO_INV 15.0/7.0
#define PI 3.141592654


int main (int argc, char **argv){
    //DEBUG_PRINT("Debug level: %d", (int) DEBUG);

	term_info_t term_info = get_terminal_info();
	printf("width: %d, height: %d\n", term_info.width, term_info.height);

	matrix_t matrix;
	matrix_t out_vectors;
	matrix_t transformation;
	matrix_t rotation;

	matrix_t terminal;
	matrix_t vectors;

	//int image_size = min(term_info.width, term_info.height);
	//int n = image_size;

	int width = term_info.width;
	int height = term_info.height;

	matrix_alloc(&terminal, height, width);
	matrix_alloc(&vectors, 3, width*height);
	matrix_alloc(&transformation, 3, 3);
	matrix_alloc(&rotation, 3, 3);

	matrix_set(&transformation, 0, 0, 1);
	matrix_set(&transformation, 1, 1, 0.5);
	matrix_set(&transformation, 2, 2, 1);

	double a = PI/4;
	matrix_set(&rotation, 0, 0, cos(a));
	matrix_set(&rotation, 1, 0, sin(a));
	matrix_set(&rotation, 0, 1, -sin(a));
	matrix_set(&rotation, 1, 1, cos(a));

	//printf(">%d<\n", grayscale_to_char(100, -100, 100));

	double r = min(width, (height-1)*RATIO_INV)/2.0;
	
	int index = 0;
	for(double x = -r; x <= r; x += 0.5){
		int y = sqrt(r*r - x*x)*RATIO;
		//y = x*x;
		//if(y > r) y = r;
		//if(y < r) y = -r;
		//printf("%f %d\n", r*r - x*x, y);
		vectors.matrix[0*vectors.n + index] = x;
		vectors.matrix[1*vectors.n + index] = y;
		index ++;
		vectors.matrix[0*vectors.n + index] = x;
		vectors.matrix[1*vectors.n + index] = -y;

		index ++;
	}
	printf("index: %d\n\n", index);
	
	//sleep(2);
	//vectors.matrix[0*vectors.n + 0] = -2;
	//vectors.matrix[1*vectors.n + 0] = -2;
/*
	for(int i = 0; i < m1.m*m1.n; i ++){
		m1.matrix[i] = i+1;
		m2.matrix[i] = i;
	}

	matrix_sub(m1, m2, &matrix);
	matrix_print(matrix, true);
*/
	matrix_print(transformation, true);
	matrix_mult(transformation, vectors, &out_vectors);
	matrix_print(out_vectors, true);
	matrix_mult(rotation, out_vectors, &vectors);
	matrix_print(vectors, true);

	vectors_to_terminal_matrix(vectors, &terminal);
	clear_terminal();
	matrix_draw(terminal);
	sleep(3);


	matrix_free(transformation);
	matrix_free(rotation);
	matrix_free(vectors);
	matrix_free(out_vectors);
	matrix_free(terminal);
	matrix_free(matrix);
	//matrix_free(m1);
	//matrix_free(m2);

	return 0;
}