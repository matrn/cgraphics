#include "matrix.h"




void matrix_alloc(matrix_t *input, int m, int n){   //allocate matrix
	input->matrix = (MATRIX_TYPE *)calloc(m*n, sizeof(MATRIX_TYPE));
	input->m = m;
	input->n = n;
}


void matrix_free(matrix_t input){
	if(input.matrix != NULL) free(input.matrix);
}


void matrix_zero(matrix_t *input){
	//printf("%d\n", sizeof(*input->matrix));
	memset(input->matrix, 0, input->m*input->n*sizeof(*input->matrix));
}


sbyte matrix_copy(matrix_t matrix1, matrix_t *matrix_out){   //source, destination
	int m = matrix1.m;
	int n = matrix1.n;
	
	if(matrix_out->m != 0 && matrix_out->n != 0 && (matrix_out->m != m || matrix_out->n != n)){
		free(matrix_out->matrix);
		matrix_out->matrix = NULL;
	}

	if(matrix_out->matrix == NULL){
		matrix_out->matrix = (MATRIX_TYPE *)calloc(m*n, sizeof(MATRIX_TYPE));
		matrix_out->m = m;
		matrix_out->n = n;
	}

	for(int rows = 0; rows < m; rows ++){
		for(int cols = 0; cols < n; cols ++){
			int pos = rows*n + cols;
			matrix_out->matrix[pos] = matrix1.matrix[pos];
		}
	}
	return 0;
}



MATRIX_TYPE color_convert(int r, int g, int b){
	//printf("RGB: %d %d %d\n", r,g,b);
	union color_conv_u color;
	color.b[0] = r;
	color.b[1] = g;
	color.b[2] = b;

	return color.vec;
}

char grayscale_to_char(int input, int minv, int maxv){
	if(input > maxv) input = maxv;
	if(input < minv) input = minv;

	int grayscale_size = strlen(grayscale_letters);   //grayscale letters count
	int converted_size = (input - minv)*(grayscale_size-1)/(maxv-minv);

	return grayscale_letters[converted_size];
}



void matrix_print(matrix_t input, bool print_size){ 
	if(print_size){
		printf("%d x %d\n", input.m, input.n);
	}

	for(int row = 0; row < input.m; row ++){
		for(int col = 0; col < input.n; col ++){
			printf("%f", input.matrix[row*input.n + col]);
			if(col < input.n-1) printf(" ");
		}
		//if(row < n-1) printf("\n");
		printf("\n");
	}
}


void matrix_draw(matrix_t input){
	//printf("\033[10A");
	printf("\x1b[H"); //\033[0m");   //home cursor

	/*
	for(int i = 0; i < input.m; i ++){
		printf("\033[1A\033[K");   //go to line up and clear the line
	}
	*/

	for(int row = 0; row < input.m; row ++){
		for(int col = 0; col < input.n; col ++){
			

			union sat_and_color_u fin;
			fin.vec = input.matrix[row*input.n + col];

			//union int16_to_bytes color;
			//color.b[0] = fin.b[1];
			//color.b[1] = fin.b[2];
			//printf("color: %d, %d, %d\n", fin.b[1], fin.b[2], fin.b[3]);
			//printf "\x1b[38;2;255;100;0mTRUECOLOR\x1b[0m\n"
			//if((fin.b[1] == 255 && fin.b[2] == 255 && fin.b[3] == 255) || (fin.b[1] == 0 && fin.b[2] == 0 && fin.b[3] == 0)) printf("%c", grayscale_to_char(fin.b[0], GRAYSCALE_MIN, GRAYSCALE_MAX));
			printf("\x1b[38;2;%d;%d;%dm%c", fin.b[1], fin.b[2], fin.b[3], grayscale_to_char(fin.b[0], GRAYSCALE_MIN, GRAYSCALE_MAX));
			//38/48
		}
		//usleep(10*1000);
		//if(row < n-1) printf("\n");
		if(row < input.m - 1) printf("\n");
	}
}



sbyte matrix_set(matrix_t *matrix, int m, int n, MATRIX_TYPE value){
	if(m < 0 || n < 0) return -1;
	if(m >= matrix->m || n >= matrix->n) return -1;
	matrix->matrix[m*matrix->n + n] = value;

	return 0;
}



//------------math operations with matrices----------
sbyte matrix_scale_and_add(int scalar1, matrix_t m1, int scalar2, matrix_t m2, matrix_t *matrix_out){   //a*A + a*B
	int m = m1.m;
	int n = m1.n;
	
	printf("%d\n", matrix_out->m);

	if(m1.m == m2.m && m1.n == m2.n){
		if(matrix_out->m != m || matrix_out->n != n){
			if(matrix_out->matrix != NULL) free(matrix_out->matrix);
			matrix_out->matrix = NULL;
		}
		if(matrix_out->matrix == NULL){
			matrix_out->matrix = (MATRIX_TYPE *)calloc(m*n, sizeof(MATRIX_TYPE));
			matrix_out->m = m;
			matrix_out->n = n;
		}

		for(int rows = 0; rows < m; rows ++){
			for(int cols = 0; cols < n; cols ++){
				int pos = rows*n + cols;   //rows*total_columns + columns
				matrix_out->matrix[pos] = scalar1*m1.matrix[pos] + scalar2*m2.matrix[pos];
			}
		}
	}
	else return -1;

	return 0;
}


sbyte matrix_add(matrix_t m1, matrix_t m2, matrix_t *matrix_out){
	return matrix_scale_and_add(1, m1, 1, m2, matrix_out);
}


sbyte matrix_sub(matrix_t m1, matrix_t m2, matrix_t *matrix_out){
	return matrix_scale_and_add(1, m1, -1, m2, matrix_out);
}


sbyte matrix_mult(matrix_t matrix1, matrix_t matrix2, matrix_t *matrix_out){   //matrices multiplication
	if(matrix1.n == matrix2.m){
		if(matrix_out->m != matrix1.m || matrix_out->n != matrix2.n){
			if(matrix_out->matrix != NULL) free(matrix_out->matrix);
			matrix_out->matrix = NULL;
		}
		if(matrix_out->matrix == NULL){
			matrix_out->matrix = (MATRIX_TYPE *)calloc(matrix1.m*matrix2.n, sizeof(MATRIX_TYPE));
			matrix_out->m = matrix1.m;   //rows of first
			matrix_out->n = matrix2.n;   //cols or second matrix
		}

		for(int rows = 0; rows < matrix1.m; rows ++){
			for(int cols = 0; cols < matrix2.n; cols ++){
				MATRIX_TYPE sum = 0;
				for(int m2_rows = 0; m2_rows < matrix2.m; m2_rows ++){
					sum += matrix1.matrix[rows*matrix1.n + m2_rows] * matrix2.matrix[m2_rows*matrix2.n + cols];
				}
				matrix_out->matrix[rows*matrix2.n + cols] = sum;
			}
		}
	}
	else return -1;

	return 0;
}
//------------math operations with matrices----------



/*
byte matrix_vectorize(matrix_t matrix, matrix_t *vectors){
	int index = 0;
	for(int row = 0; row < matrix.m; row ++){
		for(int col = 0; col < matrix.n; col ++){
			if(matrix.matrix[row*matrix.n + col] > 0){
				vectors->matrix[0*vectors->n + index] = col;
				vectors->matrix[0*vectors->n + index] = matrix.m - row;
				index ++;
			}
		}
	}
}
*/


bool matrix_add_vector(matrix_t *vectors, int *index, int x, int y, int z, int grayscale, MATRIX_TYPE color){
	if(grayscale == GRAYSCALE_MIN) return false;

	for(int q = 0; q < *index; q ++){
		if(vectors->matrix[0*vectors->n + q] == x && vectors->matrix[1*vectors->n + q] == y){
			return false;
		}
	}

	//printf("adding %d, %d, %d, color: %d\n", x, y, z, color);
	//union color_conv_u conv;
	//conv.vec = color;
	//printf("color: %d %d %d\n", conv.b[0], conv.b[1], conv.b[2]);

	vectors->matrix[0*vectors->n + *index] = x;
	vectors->matrix[1*vectors->n + *index] = y;
	vectors->matrix[2*vectors->n + *index] = z;
	vectors->matrix[3*vectors->n + *index] = grayscale;
	vectors->matrix[4*vectors->n + *index] = color;

	(*index) ++;
	
	return true; //vector added
}


sbyte vectors_to_terminal_matrix(matrix_t vectors, matrix_t *terminal, int max_index){
	//printf(">%d %d\n", terminal->m, terminal->n);
	for(int col = 0; col < max_index; col ++){
		int x = vectors.matrix[0*vectors.n + col] + terminal->n/2 + 0.4;
		int y = vectors.matrix[1*vectors.n + col]*RATIO + terminal->m/2 + 1 + 0.4;
		//int z = vectors.matrix[2*vectors.n + col];
		int grayscale = vectors.matrix[3*vectors.n + col];
		MATRIX_TYPE color = vectors.matrix[4*vectors.n + col];
		//int z = 2*vectors.n + col;
		//printf("%d %d\n", x, y);
		//int pos = y*terminal->n + x;   //rows*total_columns + columns
		//printf("%d ", pos);

		union color_conv_u conv;
		conv.vec = color;
		
		union sat_and_color_u fin;
		fin.b[0] = grayscale;
		/*if((int)color <= 0){
			fin.b[1] = 255;
			fin.b[2] = 255;
			fin.b[3] = 255;
		}
		else{*/
			fin.b[1] = conv.b[0];
			fin.b[2] = conv.b[1];
			fin.b[3] = conv.b[2];
		//}
		/*printf("gray: %d, color: %d %d %d\n", grayscale, conv.b[0], conv.b[1], conv.b[2]);
		if(color != 0) sleep(2);
		*/
		matrix_set(terminal, terminal->m-y, x, fin.vec); // == -1) perror("no");
		//terminal->matrix[pos] = vectors.matrix[pos];
	}

	return 0;
}