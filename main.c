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

	matrix_t matrix = {.matrix = NULL};
	matrix_t out_vectors = {.matrix = NULL};
	matrix_t transformation = {.matrix = NULL};
	matrix_t rotation = {.matrix = NULL};
	matrix_t scale = {.matrix = NULL};


	matrix_t terminal = {.matrix = NULL};
	matrix_t vectors = {.matrix = NULL};

	//int image_size = min(term_info.width, term_info.height);
	//int n = image_size;

	int width = term_info.width;
	int height = term_info.height;

	matrix_alloc(&terminal, height, width);
	matrix_alloc(&vectors, 3, 360); //width*height);
	matrix_alloc(&transformation, 3, 3);
	matrix_alloc(&rotation, 3, 3);
	matrix_alloc(&scale, 3, 3);

	matrix_set(&transformation, 0, 0, 1);
	matrix_set(&transformation, 1, 1, 1);
	matrix_set(&transformation, 2, 2, 1);

	
	matrix_set(&scale, 0, 0, 1);
	matrix_set(&scale, 1, 1, 1);
	matrix_set(&scale, 2, 2, 1);

	//printf(">%d<\n", grayscale_to_char(100, -100, 100));

	double r = (min(width, (height-1)*RATIO_INV)/2.0);
	
	int index = 0;
	/*
	for(double x = -r; x <= r; x += 0.5){
		int y = sqrt(r*r - x*x);//*RATIO;
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
	*/
	for(int i = 0; i < 360; i ++){
		double theta = i/180.0*PI;
    	int x = r * cos(theta);
    	int y = r * sin(theta);
		//printf("%d, %d\n", x, y);
		
		matrix_add_vector(&vectors, &index, x, y, 0);
	}
	printf("index: %d\n\n", index);
	
	matrix_add_vector(&vectors, &index, r/2, r/2, 0);
	matrix_add_vector(&vectors, &index, r/2, r/2-1, 0);
	matrix_add_vector(&vectors, &index, r/2-1, r/2, 0);
	matrix_add_vector(&vectors, &index, r/2-1, r/2-1, 0);
	matrix_add_vector(&vectors, &index, r/2-2, r/2, 0);
	matrix_add_vector(&vectors, &index, r/2-2, r/2-1, 0);

	matrix_add_vector(&vectors, &index, -r/2, r/2, 0);
	matrix_add_vector(&vectors, &index, -r/2, r/2-1, 0);
	matrix_add_vector(&vectors, &index, -r/2+1, r/2, 0);
	matrix_add_vector(&vectors, &index, -r/2+1, r/2-1, 0);
	matrix_add_vector(&vectors, &index, -r/2+2, r/2, 0);
	matrix_add_vector(&vectors, &index, -r/2+2, r/2-1, 0);

	//sleep(2);
	//vectors.matrix[0*vectors.n + 0] = 10;
	//vectors.matrix[1*vectors.n + 0] = 0;
/*
	for(int i = 0; i < m1.m*m1.n; i ++){
		m1.matrix[i] = i+1;
		m2.matrix[i] = i;
	}

	matrix_sub(m1, m2, &matrix);
	matrix_print(matrix, true);
*/
	//matrix_print(transformation, true);
	matrix_mult(transformation, vectors, &out_vectors);
	matrix_copy(out_vectors, &vectors);
	matrix_mult(scale, vectors, &out_vectors);
	//matrix_print(out_vectors, true);
	clear_terminal();

	/*
	double a = PI/180;
	matrix_set(&rotation, 0, 0, cos(a));
		matrix_set(&rotation, 1, 0, sin(a)*RATIO);
		matrix_set(&rotation, 0, 1, -sin(a));
		matrix_set(&rotation, 1, 1, cos(a)*RATIO);
		matrix_mult(rotation, out_vectors, &vectors);
		matrix_print(rotation, true);
		//sleep(2);
	vectors_to_terminal_matrix(vectors, &terminal);
	matrix_draw(terminal);
	//sleep(3);
	*/

	for(int i = 0; i < 720; i ++){
		double a = (i*PI)/180.0;
		a = 0;
		matrix_set(&rotation, 0, 0, cos(a));
		matrix_set(&rotation, 1, 0, sin(a)*RATIO);
		matrix_set(&rotation, 0, 1, -sin(a));
		matrix_set(&rotation, 1, 1, cos(a)*RATIO);
		matrix_mult(rotation, out_vectors, &vectors);
		//matrix_print(vectors, true);
		matrix_zero(&terminal);
		vectors_to_terminal_matrix(vectors, &terminal, index);

		//matrix_print(rotation, true);
		matrix_draw(terminal);
		usleep(10*1000);
	}
	sleep(2);


	matrix_free(transformation);
	matrix_free(rotation);
	matrix_free(scale);


	matrix_free(vectors);
	matrix_free(out_vectors);
	matrix_free(terminal);
	matrix_free(matrix);
	//matrix_free(m1);
	//matrix_free(m2);

	return 0;
}