#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "utils.h"
#include "matrix.h"
#include <imago2.h>


#define PI 3.141592654

#define VECTOR_DIM 5


int rgbToAnsi256(int r, int g, int b) {
    // we use the extended greyscale palette here, with the exception of
    // black and white. normal palette only has 4 greyscale shades.
    if (r == g && g == b) {
        if (r < 8) {
            return 16;
        }

        if (r > 248) {
            return 231;
        }

        return round(((r - 8.0) / 247.0) * 24.0) + 232;
    }

    int ansi = 16
        + (36.0 * round(r / 255.0 * 5.0))
        + (6.0 * round(g / 255.0 * 5.0))
        + round(b / 255.0 * 5.0);

    return ansi;
}


int main (int argc, char **argv){
    //DEBUG_PRINT("Debug level: %d", (int) DEBUG);
	
	printf("ansi: %d\n", rgbToAnsi256(253,13,0));

	/*
	union int16_to_bytes conv;
	conv.integer = -1;

	union color_conv fin;
	fin.b[0] = 10;
	fin.b[1] = conv.b[0];
	fin.b[2] = conv.b[1];

	union color_conv fin2;
	fin2.vec = fin.vec;

	union int16_to_bytes color;
	color.b[0] = fin2.b[1];
	color.b[1] = fin2.b[2];

	printf("grayscale:%d, color: %d\n", fin2.b[0], color.integer);
	puts("gg");
	sleep(2);
	*/

	term_info_t term_info = get_terminal_info();
	printf("width: %d, height: %d\n", term_info.width, term_info.height);

	matrix_t matrix = {.matrix = NULL};
	matrix_t out_vectors = {.matrix = NULL};
	matrix_t transformation = {.matrix = NULL};
	matrix_t rotation = {.matrix = NULL};
	matrix_t shear = {.matrix = NULL};
	matrix_t scale = {.matrix = NULL};


	matrix_t terminal = {.matrix = NULL};
	matrix_t vectors = {.matrix = NULL};

	//int image_size = min(term_info.width, term_info.height);
	//int n = image_size;

	int width = term_info.width;
	int height = term_info.height;

	matrix_alloc(&terminal, height, width);
	matrix_alloc(&vectors, VECTOR_DIM, 80000); //width*height);
	matrix_alloc(&transformation, VECTOR_DIM, VECTOR_DIM);
	matrix_alloc(&rotation, VECTOR_DIM, VECTOR_DIM);
	matrix_alloc(&scale, VECTOR_DIM, VECTOR_DIM);
	matrix_alloc(&shear, VECTOR_DIM, VECTOR_DIM);


	matrix_set(&transformation, 0, 0, 1);   //X
	matrix_set(&transformation, 1, 1, 1);   //Y
	matrix_set(&transformation, 2, 2, 1);   //Z
	matrix_set(&transformation, 3, 3, 1);   //grayscale
	matrix_set(&transformation, 4, 4, 1);   //ansi color

	
	matrix_set(&scale, 0, 0, 0.8);
	matrix_set(&scale, 1, 1, 0.8);
	matrix_set(&scale, 2, 2, 1);
	matrix_set(&scale, 3, 3, 1);   //grayscale
	matrix_set(&scale, 4, 4, 1);   //ansi color

	//printf(">%d<\n", grayscale_to_char(100, -100, 100));

	
	int index = 0;
	

	{
		int width, height;
		int r,g,b,a;

		struct img_pixmap img;
		img_init(&img);
		img_load(&img, "images/rainbow.jpeg");
		
		width = img.width;
		height = img.height;
		
		float img_scale = 4;

		if(height > term_info.height) img_scale = height/term_info.height/2;

		for(int x = 0; x < width; x += 1){
			for(int y = 0; y < height; y += 1){
				img_getpixel4i(&img, x, y, &r, &g, &b, &a);
				matrix_add_vector(&vectors, &index, (x-width/2)/img_scale, (height-y-height/2)/img_scale, 0, GRAYSCALE_MAX, rgbToAnsi256(r,g,b));
				printf("index: %d. %d %d %d color: %d\n", index, r,g,b, rgbToAnsi256(r,g,b));
				//if(index > 5) exit(0);
			}
		}
		
		img_destroy(&img);



		printf("scale: %f, term w: %d, term h: %d, width: %d, height: %d, RGBA %d %d %d %d\n", img_scale, term_info.width, term_info.height, width, height, r, g, b, a);
	}
	

	printf("index: %d\n\n", index);
	sleep(2);
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

	vectors_to_terminal_matrix(out_vectors, &terminal, index);
	//matrix_print(rotation, true);
	matrix_draw(terminal);
	sleep(3);

	for(int i = 0; i < 360; i ++){
		double a = (i*PI)/180.0;
		//a = 0;
		matrix_set(&rotation, 0, 0, cos(a));
		matrix_set(&rotation, 1, 0, sin(a));
		matrix_set(&rotation, 0, 1, -sin(a));
		matrix_set(&rotation, 1, 1, cos(a));
		matrix_set(&rotation, 3, 3, 1);
		matrix_set(&rotation, 4, 4, 1);
		matrix_mult(rotation, out_vectors, &vectors);
		//matrix_print(vectors, true);
		matrix_zero(&terminal);
		vectors_to_terminal_matrix(vectors, &terminal, index);

		//matrix_print(rotation, true);
		matrix_draw(terminal);
		usleep(10*1000);
	}
	
	
	int i = 0;
	int pos = 0;
	while(true){
		matrix_set(&shear, 0, 0, 1);
		//matrix_set(&shear, 1, 0, i/100.0*RATIO);
		matrix_set(&shear, 0, 1, i/100.0);
		matrix_set(&shear, 1, 1, 1);
		matrix_set(&shear, 3, 3, 1);
		matrix_set(&shear, 4, 4, 1);
		matrix_mult(shear, out_vectors, &vectors);

		matrix_zero(&terminal);
		vectors_to_terminal_matrix(vectors, &terminal, index);

		matrix_draw(terminal);
		usleep(10*1000);

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
	matrix_free(transformation);
	matrix_free(rotation);
	matrix_free(scale);
	matrix_free(shear);


	matrix_free(vectors);
	matrix_free(out_vectors);
	matrix_free(terminal);
	matrix_free(matrix);
	//matrix_free(m1);
	//matrix_free(m2);

	return 0;
}