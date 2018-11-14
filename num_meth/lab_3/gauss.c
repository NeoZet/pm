#include <stdio.h>
#include <stdlib.h>

#define SYSTEM_ORDER 3

int gauss(double **matrix, double *extension, int order, double *solve);

static int readEquations(FILE *fd, double **matrix, double *extension, int order);
static int prepare_main_elem(double **matrix, double *extension, int order);

int main()
{
	int order = SYSTEM_ORDER;
	int i;		
	double **matrix = (double**)malloc(sizeof(double*) * order);
	for (i = 0; i < order; ++i) {
		matrix[i] = (double*)malloc(sizeof(double) * order);
	}
	double *extension = (double*)malloc(sizeof(double) * order);
	double *solve = (double*)malloc(sizeof(double) * order);

	int ret = readEquations(NULL, matrix, extension, order);
	if (ret != 0) {
		printf("Matrix read error: %d\n", ret);
		return ret;
	}
	ret = prepare_main_elem(matrix, extension, order);
	if (ret != 0) {
		printf("Preparing main elem error: %d\n", ret);
		return ret;
	}
	ret = gauss(matrix, extension, order, solve);
	if (ret != 0) {
		printf("Equation calculatoin error: %d\n", ret);
		return ret;
	}

	printf("========\n");
	for (i = 0; i < order; ++i) {
		printf("X%d = %lf\n", i+1, solve[i]);
	}
	printf("========\n");

	return 0;
}

int gauss(double **matrix, double *extension, int order, double *solve)
{
	int i,j,k;
	if (matrix == NULL || extension == NULL || solve == NULL) {
		return -1;
	}
	for (i = 0; i < order; ++i){
		if (matrix[i][i] == 0){				
			return -2;
		}
		for(j = i + 1; j < order; ++j){
			double tmp = matrix[j][i] / matrix[i][i];
			for(k = i; k < order; ++k){
				matrix[j][k] -= tmp * matrix[i][k];
			}
			extension[j] -= tmp * extension[i];
		}
	}
	for (i = 0; i < order; ++i) {
		solve[i] = 0;
	}
	for(i = order - 1; i >= 0; --i){
		double sum_known_elems = 0;
		for(j = i; j < order; ++j){
			sum_known_elems += matrix[i][j] * solve[j];
		}
		solve[i] = (extension[i] - sum_known_elems) / matrix[i][i];
	}
	return 0;
}

static int readEquations(FILE *fd, double **matrix, double *extension, int order)
{
	double matrix_tmp[SYSTEM_ORDER][SYSTEM_ORDER] = {{1.80, 2.50, 4.60},
							 {3.10, 2.30, -1.20},
							 {4.51,-1.80,3.60}};/* {{5.0, 1.0, 0.0, -1.0}, */
							/*  {3.0, -3.0, 1.0, 4.0}, */
							/*  {3.0, 0.0, -2.0, 1.0}, */
							/*  {1.0, -4.0, 0.0, 1.0}}; */
	double extension_tmp[SYSTEM_ORDER] = /* {-9.0,-7.0,-16.0,0.0}; */{2.20,3.60,-1.70};
	if (matrix == NULL || extension == NULL) {		
		return -1;
	}
	if (fd == NULL) {
		//TODO: read by stdio
		int i, j;
		for (i = 0; i < order; ++i) {
			for (j = 0; j < order; ++j) {
				matrix[i][j] = matrix_tmp[i][j];
			}
			extension[i] = extension_tmp[i];
		}
		return 0;
	}
	//TODO: read from fs
	return 0;
}


static int prepare_main_elem(double **matrix, double *extension, int order)
{
	int i,j,k;
	double tmp = 0;
	for (i = 0; i < order; ++i) {
		for (j = 0; j < order; ++j) {
			if (matrix[i][j] != 0 &&
			    matrix[j][i] != 0 &&
			    matrix[i][i] == 0 &&
			    i != j)
			{
				for (k = 0; k < order; ++k) {
					tmp = matrix[j][k];
					matrix[j][k] = matrix[i][k];
					matrix[i][k] = tmp;
				}
				tmp = extension[j];
				extension[j] = extension[i];
				extension[i] = tmp;
				break;
			}
		}
	}
	return 0;
}


