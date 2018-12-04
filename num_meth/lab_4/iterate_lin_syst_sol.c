#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SYSTEM_ORDER 3

int jacobi(double **matrix, double *extension, int order, double eps, double *solve);
int gauss_seidel(double **matrix, double *extension, int order, double eps, double *solve);

static int readEquations(FILE *fd, double **matrix, double *extension, int order);
static int resid(double **matrix, double *extension, double *solve, int order, double *resid);
static int resid_norm(double *resid, int order, double *norm);
static double accuracy(double *solv_curr, double *solv_prev, int order);

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
	double *residial = (double*)malloc(sizeof(double) * order);
	double residial_norm = 0;

	int ret = readEquations(NULL, matrix, extension, order);
	if (ret != 0) {
		printf("Matrix read error: %d\n", ret);
		return ret;
	}
	printf("JACOBI METHOD\n--------------\n");
	ret = jacobi(matrix, extension, order, 0.0001, solve);
	if (ret != 0) {
		printf("JACOBI: Equation calculatoin error: %d\n", ret);
		return ret;
	}

	printf("========\n");
	for (i = 0; i < order; ++i) {
		printf("X%d = %lf\n", i+1, solve[i]);
	}
	printf("========\n");

	ret = resid(matrix, extension, solve, order, residial);
	if (ret != 0) {
		printf("Resid calculatoin error: %d\n", ret);
		return ret;
	}
	for (i = 0; i < order; ++i) {
		printf("F%d = %lf\n", i+1, residial[i]);
	}

	ret = resid_norm(residial, order, &residial_norm);
	if (ret != 0) {
		printf("Resid norm calculatoin error: %d\n", ret);
		return ret;
	}	
	printf("Resid norm: %lf\n", residial_norm);

	printf("\n******************\nGAUSS_SEIDEL METHOD\n--------------\n");
	ret = gauss_seidel(matrix, extension, order, 0.0001, solve);
	if (ret != 0) {
		printf("JACOBI: Equation calculatoin error: %d\n", ret);
		return ret;
	}

	printf("========\n");
	for (i = 0; i < order; ++i) {
		printf("X%d = %lf\n", i+1, solve[i]);
	}
	printf("========\n");

	ret = resid(matrix, extension, solve, order, residial);
	if (ret != 0) {
		printf("Resid calculatoin error: %d\n", ret);
		return ret;
	}
	for (i = 0; i < order; ++i) {
		printf("F%d = %lf\n", i+1, residial[i]);
	}

	ret = resid_norm(residial, order, &residial_norm);
	if (ret != 0) {
		printf("Resid norm calculatoin error: %d\n", ret);
		return ret;
	}	
	printf("Resid norm: %lf\n", residial_norm);
	
	return 0;
}

int jacobi(double **matrix, double *extension, int order, double eps, double *solve)
{
	int i, j;	
	int iter = 0;
	double sum = 0;
	double *solve_prev = (double*)calloc(order, sizeof(double));
	do {		
		for (i = 0; i < order; ++i) {
			sum = 0;
			solve_prev[i] = solve[i];
			for (j = 0; j < order; ++j) {
				sum += (i != j ? (- matrix[i][j] * solve[j]) : 0);
			}
			if (matrix[i][i] == 0) {
				return -1;
			}
			solve[i] = (sum + extension[i]) / matrix[i][i];
		}		
	} while (accuracy(solve, solve_prev, order) > eps);
	return 0;
}

int gauss_seidel(double **matrix, double *extension, int order, double eps, double *solve)
{
	int i, j;	
	int iter = 0;
	double sum_l = 0;
	double sum_r = 0;
	double *solve_prev = (double*)calloc(order, sizeof(double));
	do {
		for (i = 0; i < order; ++i) {
			solve_prev[i] = solve[i];
		}
		for (i = 0; i < order; ++i) {			
			sum_l = 0;
			sum_r = 0;			
			for (j = 0; j < i; ++j) {
				sum_l += (- matrix[i][j] * solve[j]);
			}
			for (j = i+1; j < order; ++j) {
				sum_r += (- matrix[i][j] * solve_prev[j]);
			}			
			if (matrix[i][i] == 0) {
				return -1;
			}
			solve[i] = (sum_l + sum_r + extension[i]) / matrix[i][i];
		}
	} while (accuracy(solve, solve_prev, order) > eps);
	return 0;
}


static double accuracy(double *solv_curr, double *solv_prev, int order)
{
	int i;
	double max_abs_sub = 0;
	double max_abs_curr = 0;
	for (i = 0; i < order; ++i) {
		solv_prev[i] = solv_curr[i] - solv_prev[i];
	}
	for (i = 0; i < order; ++i) {
		if (fabs(solv_curr[i]) > max_abs_curr) {
			max_abs_curr = fabs(solv_curr[i]);
		}
		if (fabs(solv_prev[i]) > max_abs_sub) {
			max_abs_sub = fabs(solv_prev[i]);
		}
	}
	if (max_abs_curr != 0) {
		return max_abs_sub / max_abs_curr;
	}
	else {
		return 0;
	}
}

	
static int readEquations(FILE *fd, double **matrix, double *extension, int order)
{
	double matrix_tmp[SYSTEM_ORDER][SYSTEM_ORDER] = {{5,-3,1},
							 {1,-10,3},
							 {-2,1,4}};/* {{5.0, 1.0, 0.0, -1.0}, */
							/*  {3.0, -3.0, 1.0, 4.0}, */
							/*  {3.0, 0.0, -2.0, 1.0}, */
							/*  {1.0, -4.0, 0.0, 1.0}}; */
	double extension_tmp[SYSTEM_ORDER] = /* {-9.0,-7.0,-16.0,0.0}; */{1,2,-8};
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

static int resid(double **matrix, double *extension, double *solve, int order, double *resid)
{
	int i,j;
	if (matrix == NULL ||
	    extension == NULL ||
	    solve == NULL ||
	    resid == NULL)
	{
		printf("Error (resid): NULL pointer passed\n");
		return -1;		
	}
	for (i = 0; i < order; ++i) {
		int res = 0;
		for (j = 0; j < order; ++j) {
			resid[i] += matrix[i][j] * solve[j];
		}
		resid[i] -= extension[i];
	}
	return 0;
}

static int resid_norm(double *resid, int order, double *norm)
{
	int i;
	double max_abs = 0;
	if (resid == NULL || norm == NULL) {
		printf("Error (resid): NULL pointer passed\n");
		return -1;				
	}
	for (i = 0; i < order; ++i) {
		if (fabs(resid[i]) > max_abs) {
			max_abs = fabs(resid[i]);
		}
	}
	*norm = max_abs;
	return 0;
}
