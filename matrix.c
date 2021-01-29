#include "matrix.h"


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
	printf("\x1b[H");   //home cursor

	/*
	for(int i = 0; i < input.m; i ++){
		printf("\033[1A\033[K");   //go to line up and clear the line
	}
	*/

	for(int row = 0; row < input.m; row ++){
		for(int col = 0; col < input.n; col ++){
			printf("%c", grayscale_to_char(input.matrix[row*input.n + col], 0, 9));
		}
		usleep(10*1000);
		//if(row < n-1) printf("\n");
		if(row < input.m - 1) printf("\n");
	}
}

void matrix_alloc(matrix_t *input, int m, int n){   //
	input->matrix = (MATRIX_TYPE *)calloc(m*n, sizeof(MATRIX_TYPE));
	input->m = m;
	input->n = n;
}

void matrix_free(matrix_t input){
	if(input.matrix != NULL) free(input.matrix);
}





byte matrix_scale_and_add(int scalar1, matrix_t m1, int scalar2, matrix_t m2, matrix_t *matrix_out){   //a*A + a*B
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


byte matrix_add(matrix_t m1, matrix_t m2, matrix_t *matrix_out){
	return matrix_scale_and_add(1, m1, 1, m2, matrix_out);
}

byte matrix_sub(matrix_t m1, matrix_t m2, matrix_t *matrix_out){
	return matrix_scale_and_add(1, m1, -1, m2, matrix_out);
}


char grayscale_to_char(int input, int minv, int maxv){
	if(input > maxv) input = maxv;
	if(input < minv) input = minv;

	int grayscale_size = strlen(grayscale_letters);   //grayscale letters count
	int converted_size = (input - minv)*(grayscale_size-1)/(maxv-minv);

	return grayscale_letters[converted_size];
}


byte matrix_mult(matrix_t matrix1, matrix_t matrix2, matrix_t *matrix_out){   //matrices multiplication
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

byte matrix_set(matrix_t *matrix, int m, int n, MATRIX_TYPE value){
	if(m >= matrix->m || n >= matrix->n) return -1;
	matrix->matrix[m*matrix->n + n] = value;

	return 0;
}

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

byte vectors_to_terminal_matrix(matrix_t vectors, matrix_t *terminal){
	printf(">%d %d\n", terminal->m, terminal->n);
	for(int col = 0; col < vectors.n; col ++){
		int x = vectors.matrix[0*vectors.n + col] + terminal->n/2;
		int y = vectors.matrix[1*vectors.n + col] + terminal->m/2 + 1;
		//int z = 2*vectors.n + col;
		//printf("%d %d\n", x, y);
		//int pos = y*terminal->n + x;   //rows*total_columns + columns
		//printf("%d ", pos);
		matrix_set(terminal, terminal->m-y, x, 10);
		//terminal->matrix[pos] = vectors.matrix[pos];
	}

	return 0;
}


/*
uint8_t m_copy(Matrix *matrix_out, Matrix matrix1){   //destination, source
	int n = matrix1.n;
	int m = matrix1.m;
	
	if(matrix_out->n != 0 && matrix_out->m != 0 && (matrix_out->n != n || matrix_out->m != m)){
		free(matrix_out->matrix);
		matrix_out->matrix = NULL;
	}
	if(matrix_out->matrix == NULL){
		matrix_out->matrix = (int *)calloc(n*m, sizeof(int));
		matrix_out->n = n;
		matrix_out->m = m;
	}

	for(int rows = 0; rows < n; rows ++){
		for(int cols = 0; cols < m; cols ++){
			int pos = rows*m + cols;
			matrix_out->matrix[pos] = matrix1.matrix[pos];
		}
	}
	return 0;
}





int main(){
	//int n1=0, m1=0;
	//int n2=0, m2=0;
	int rtn = 0;
	//int *matrix1, *matrix2;
	Matrix matrix_sum = { .matrix = NULL, .n = 0, .m = 0 };
	Matrix matrix1 = { .matrix = NULL };
	Matrix matrix2 = { .matrix = NULL };
	Matrix matrix3 = { .matrix = NULL };
	Matrix matrix_out = { .matrix = NULL };


	char operation = 0;

	uint8_t pos = 0;

	bool operation_or_matrix = true;
	char first_operation = 0;
	char second_operation = 0;



		}
		else{
			//#ifdef DEBUG
			//	puts("operation");
			//#endif
			while(!check_operation(operation)){
				int rtn = 0;
				//printf(">%c<\n", operation);
				if((rtn = scanf("%c\n", &operation)) != 1){
					if (rtn == EOF) break;
					rtn = err_input();
					goto clean;
				}
			}
			//printf(">%c<\n", operation);
			if(check_operation(operation)){
				if(first_operation == 0) first_operation = operation;
				else if(second_operation == 0) second_operation = operation;
			}
			operation = 0;
		}
		operation_or_matrix = !operation_or_matrix;
	}
		//print_matrix(matrix1);
		//printf("\n");

	//printf("%c,%c\n", first_operation, second_operation);
	if(second_operation != 0){
		//puts("sec");
		#ifdef DEBUG
			puts("end - sec");
		#endif
		if((rtn = matrices_compute(matrix2, matrix3, &matrix_out, second_operation)) != 0) goto clean;
		m_copy(&matrix2, matrix_out);
	}
	if(first_operation != 0){
		//puts("frst");
		#ifdef DEBUG
			puts("end - frst");
		#endif
		if((rtn = matrices_compute(matrix1, matrix2, &matrix_out, first_operation)) != 0) goto clean;
		m_copy(&matrix_sum, matrix_out);
	}
	print_matrix(matrix_sum, true);

	clean:
		if(matrix_sum.matrix != NULL) free(matrix_sum.matrix);
		if(matrix1.matrix != NULL) free(matrix1.matrix);
		if(matrix2.matrix != NULL) free(matrix2.matrix);
		if(matrix3.matrix != NULL) free(matrix3.matrix);
		if(matrix_out.matrix != NULL) free(matrix_out.matrix);

	if(rtn < 0) rtn = 0;
	return rtn;
}
*/