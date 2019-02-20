#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "utils/gauss.h"

#define NUM_OF_EQ 2
#define MAX_ITER 50
static const double EPS_1 = 1.0e-9;
static const double EPS_2 = 1.0e-9;

typedef int (*ftype)(double*, int, double**);

int jacobian(ftype system, double *x, int num_of_eq, double ***result);
int newton(ftype system, double *x, int num_of_eq, const double eps_1, const double eps_2, \
	   double **result, int *iter, double *delta_1, double *delta_2);

static int f(double *x, int num_of_eq, double **result);
static double delta_1(ftype eq_system, double *xk, int num_of_eq);
static double delta_2(double *xk, double *xk_next, int num_of_eq);


int main()
{
	double *x = (double*)malloc(NUM_OF_EQ * sizeof(double));
	double *res = NULL;
	int iter;
	double d1[MAX_ITER], d2[MAX_ITER];
	
	memset(x, 0, NUM_OF_EQ);
	int ret = newton(f, x, NUM_OF_EQ, EPS_1, EPS_2, &res, &iter, d1, d2);
	printf("Iter  |     delta_1      |     delta_2\n");
	printf("--------------------------------------------\n");
	for (int i = 0; i < iter; ++i) {
		printf(" %d    |   %.10lf   |     %.10lf\n", i, d1[i], d2[i]);
	}
	printf("--------------------------------------------\n");
	printf("Solution:\n");
	for (int i = 0; i < NUM_OF_EQ; ++i) {
		printf("X%d = %.10lf\n", i+1, res[i]);
	}
}


int jacobian(ftype eq_system, double *x, int num_of_eq, double ***result)
{
	int ret = 0;
	double h = 1.0e-4;
	
	if (x == NULL) {
		ret = -1;
		goto out;
	}

	double *f0 = (double*)malloc(num_of_eq * sizeof(double));
	if (f0 == NULL) {
		ret = -2;
		goto out;
	}

	double *f_x = (double*)malloc(num_of_eq * sizeof(double));
	if (f_x == NULL) {
		ret = -2;
		goto free_f0;
	}

	*result = (double**)malloc((num_of_eq+1) * sizeof(double*));
	for (int i = 0; i < num_of_eq + 1; ++i) {
		(*result)[i] = (double*)malloc(num_of_eq * sizeof(double));
	}

	if ((ret = eq_system(x, num_of_eq, &f0)) != 0) {
		goto free_fx;
	}

	double tmp;
	
	for (int i = 0; i < num_of_eq; ++i) {
		tmp = x[i];
		x[i] = tmp + h;

		if ((ret = eq_system(x, num_of_eq, &f_x)) != 0) {
			goto free_fx;
		}

		x[i] = tmp;
		for (int j = 0; j < num_of_eq; ++j) {
			(*result)[j][i] = (f_x[j] - f0[j]) / h;
			(*result)[num_of_eq][j] = f0[j];
		}
	}

free_fx:
	free(f_x);
	
free_f0:
	free(f0);
	
out:
	return ret;
}


int newton(ftype eq_system, double *x, int num_of_eq, const double eps_1, const double eps_2, \
	   double **result, int *iter, double *arr_delta_1, double *arr_delta_2)
{
	int ret = 0;
	const int iter_max = MAX_ITER;
	double d1[iter_max], d2[iter_max];

	double *x_copy = (double*)malloc(num_of_eq * sizeof(double));
	double *x_prev = (double*)malloc(num_of_eq * sizeof(double));
	memcpy(x_copy, x, num_of_eq);
	
	double **jac = NULL;
	double *dx = (double*)malloc(num_of_eq * sizeof(double));

	*result = (double*)malloc(num_of_eq * sizeof(double));
	
	for (int i = 0; i < iter_max; ++i) {
		if ((ret = jacobian(eq_system, x_copy, num_of_eq, &jac)) != 0) {
			goto free_tmp_x;
		}
	  	if ((ret = gauss(jac, jac[num_of_eq], num_of_eq, dx)) != 0) {
			goto free_jac;
		}

		memmove(x_prev, x_copy, num_of_eq * sizeof(double));
		for (int j = 0; j < num_of_eq; ++j) {			
			x_copy[j] -= dx[j];
		}
		d1[i] = delta_1(eq_system, x_copy, num_of_eq);
		d2[i] = delta_2(x_prev, x_copy, num_of_eq);
		if (d1[i] < eps_1 && d2[i] < eps_2) {
			memcpy(*result, x_copy, num_of_eq * sizeof(double));
			*iter = i;
			memcpy(arr_delta_1, d1, num_of_eq * sizeof(double));
			memcpy(arr_delta_2, d2, num_of_eq * sizeof(double));
			goto out;
		}		
	}
	printf("Слишком много итераций!\n");
	ret = -1;
	
free_jac:
	for (int i = 0; i < num_of_eq + 1; ++i) {
		free(jac[i]);
	}
	free(jac);
	
free_tmp_x:
	free(x_copy);
	free(x_prev);
	free(dx);
out:
	return ret;
}



static int f(double *x, int num_of_eq, double **result)
{
	int ret = 0;
	if (x == NULL || (*result) == NULL) {
		ret = -1;
		goto out;
	}       

	/*
	(*result)[0] = 2 * x[1] - cos(x[0] + 1);
	(*result)[1] = x[0] + sin(x[1]) + 0.4;
	*/
	
	(*result)[0] = powf(x[0], 2) - x[1] + 1;
	(*result)[1] = x[0] - cos(M_PI/2 * x[1]);

out:
	return ret;
}


static double delta_1(ftype eq_system, double *xk, int num_of_eq)
{
	double max = 0;
	double *res = (double*)malloc(num_of_eq * sizeof(double));
	eq_system(xk, num_of_eq, &res);
	for (int i = 0; i < num_of_eq; ++i) {
		if (fabs(res[i]) > max) {
			max = fabs(res[i]);
		}
	}
	free(res);
	return max;
}

static double delta_2(double *xk, double *xk_next, int num_of_eq)
{
	double max = 0;
	for (int i = 0; i < num_of_eq; ++i) {
		double abs_dx = fabs(xk_next[i] - xk[i]);
		if (abs_dx > max) {
			max = abs_dx;
		}
	}
	return max;
}


