#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "utils.h"
#include "matrix.h"


#define WHITE_COLOR color_convert(255,255,255)


int main (int argc, char **argv){
	term_info_t term_info;
	
	matrix_t transformation = {.matrix = NULL};
	matrix_t rotation = {.matrix = NULL};
	matrix_t shear = {.matrix = NULL};
	matrix_t scale = {.matrix = NULL};

	matrix_t terminal = {.matrix = NULL};
	matrix_t vectors = {.matrix = NULL};
	matrix_t out_vectors = {.matrix = NULL};

	int width, height, index;
	double r;


	term_info = get_terminal_info();
	width = term_info.width;
	height = term_info.height;
	printf("width: %d, height: %d\n", term_info.width, term_info.height);
	

	matrix_alloc(&terminal, height, width);
	matrix_alloc(&vectors, VECTOR_DIM, 800); //width*height);
	matrix_alloc(&transformation, VECTOR_DIM, VECTOR_DIM);
	matrix_alloc(&rotation, VECTOR_DIM, VECTOR_DIM);
	matrix_alloc(&scale, VECTOR_DIM, VECTOR_DIM);
	matrix_alloc(&shear, VECTOR_DIM, VECTOR_DIM);


	//identity
	matrix_set(&transformation, 0, 0, 1);   //X
	matrix_set(&transformation, 1, 1, 1);   //Y
	matrix_set(&transformation, 2, 2, 1);   //Z
	matrix_set(&transformation, 3, 3, 1);   //grayscale
	matrix_set(&transformation, 4, 4, 1);   //color

	//scale
	matrix_set(&scale, 0, 0, 0.8);
	matrix_set(&scale, 1, 1, 0.8);
	matrix_set(&scale, 2, 2, 0.8);
	matrix_set(&scale, 3, 3, 1);   //grayscale
	matrix_set(&transformation, 4, 4, 1);   //color

	

	r = (min(width, (height-1)*RATIO_INV)/2.0);
	
	index = 0;

	//head
	for(int i = 0; i < 360; i ++){
		double theta = i/180.0*PI;
    	int x = r * cos(theta);
    	int y = r * sin(theta);
		//printf("%d, %d\n", x, y);		
		matrix_add_vector(&vectors, &index, x, y, 0, GRAYSCALE_MAX, WHITE_COLOR);   //-1 = nocolor
	}
	
	//right eye
	matrix_add_vector(&vectors, &index, r/2, r/2, 0, GRAYSCALE_MAX, WHITE_COLOR);
	matrix_add_vector(&vectors, &index, r/2, r/2-1, 0, GRAYSCALE_MAX, WHITE_COLOR);
	matrix_add_vector(&vectors, &index, r/2-1, r/2, 0, GRAYSCALE_MAX, WHITE_COLOR);
	matrix_add_vector(&vectors, &index, r/2-1, r/2-1, 0, GRAYSCALE_MAX, WHITE_COLOR);
	matrix_add_vector(&vectors, &index, r/2-2, r/2, 0, GRAYSCALE_MAX, WHITE_COLOR);
	matrix_add_vector(&vectors, &index, r/2-2, r/2-1, 0, GRAYSCALE_MAX, WHITE_COLOR);

	//left eye
	matrix_add_vector(&vectors, &index, -r/2, r/2, 0, GRAYSCALE_MAX, WHITE_COLOR);
	matrix_add_vector(&vectors, &index, -r/2, r/2-1, 0, GRAYSCALE_MAX, WHITE_COLOR);
	matrix_add_vector(&vectors, &index, -r/2+1, r/2, 0, GRAYSCALE_MAX, WHITE_COLOR);
	matrix_add_vector(&vectors, &index, -r/2+1, r/2-1, 0, GRAYSCALE_MAX, WHITE_COLOR);
	matrix_add_vector(&vectors, &index, -r/2+2, r/2, 0, GRAYSCALE_MAX, WHITE_COLOR);
	matrix_add_vector(&vectors, &index, -r/2+2, r/2-1, 0, GRAYSCALE_MAX, WHITE_COLOR);

	//nose
	matrix_add_vector(&vectors, &index, 0, 0, 0, GRAYSCALE_MAX, WHITE_COLOR);
	matrix_add_vector(&vectors, &index, 0, 1, 0, GRAYSCALE_MAX, WHITE_COLOR);
	matrix_add_vector(&vectors, &index, 0, 2, 0, GRAYSCALE_MAX, WHITE_COLOR);
	matrix_add_vector(&vectors, &index, 1, 0, 0, GRAYSCALE_MAX, WHITE_COLOR);
	matrix_add_vector(&vectors, &index, 1, 1, 0, GRAYSCALE_MAX, WHITE_COLOR);
	matrix_add_vector(&vectors, &index, 1, 2, 0, GRAYSCALE_MAX, WHITE_COLOR);
	matrix_add_vector(&vectors, &index, -1, 0, 0, GRAYSCALE_MAX, WHITE_COLOR);
	matrix_add_vector(&vectors, &index, -1, 1, 0, GRAYSCALE_MAX, WHITE_COLOR);
	matrix_add_vector(&vectors, &index, -1, 2, 0, GRAYSCALE_MAX, WHITE_COLOR);

	//mouth
	for(int i = 0; i < 180; i ++){
		double theta = i/180.0*PI;
    	int x = r/2 * cos(theta);
    	int y = r/2 * sin(theta);
		//printf("%d, %d\n", x, y);
		
		matrix_add_vector(&vectors, &index, x, -y -r/4, 0, GRAYSCALE_MAX, WHITE_COLOR);
	}	

	printf("index: %d\n\n", index);
	

	matrix_mult(transformation, vectors, &out_vectors);   //transformation matrix
	matrix_copy(out_vectors, &vectors);
	matrix_mult(scale, vectors, &out_vectors);   //scale matrix
	//matrix_print(out_vectors, true);
	clear_terminal();
	
	for(int i = 0; i < 360; i ++){
		double a = (i*PI)/180.0;
		//a = 0;
		matrix_set(&rotation, 0, 0, cos(a));
		matrix_set(&rotation, 1, 0, sin(a));
		matrix_set(&rotation, 0, 1, -sin(a));
		matrix_set(&rotation, 1, 1, cos(a));
		matrix_set(&rotation, 3, 3, 1);
		matrix_mult(rotation, out_vectors, &vectors);
		
		matrix_zero(&terminal);   //clear terminal matrix
		vectors_to_terminal_matrix(vectors, &terminal, index);

		//matrix_print(rotation, true);
		matrix_draw(terminal);
		msleep(10);
	}
	
	
	int i = 0;
	int pos = 0;
	while(true){
		matrix_set(&shear, 0, 0, 1);
		//matrix_set(&shear, 1, 0, i/100.0*RATIO);
		matrix_set(&shear, 0, 1, i/100.0);
		matrix_set(&shear, 1, 1, 1);
		matrix_set(&shear, 3, 3, 1);
		matrix_mult(shear, out_vectors, &vectors);

		matrix_zero(&terminal);
		vectors_to_terminal_matrix(vectors, &terminal, index);

		matrix_draw(terminal);
		msleep(10);

		if(pos == 0) i ++;
		if(i == 100){
			pos = 1;
		}
		if(pos == 1) i --;
		if(i == -100){
			pos = 3;
		}
		if(pos == 3) i ++;
		if(pos == 3 && i == 0) break;
	}

	sleep(2);
	reset_colors();

	matrix_free(transformation);
	matrix_free(rotation);
	matrix_free(scale);
	matrix_free(shear);

	matrix_free(terminal);
	matrix_free(vectors);
	matrix_free(out_vectors);


	return 0;
}