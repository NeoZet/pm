#include <stdio.h>
#include <math.h>

#include "gauss.h"

static int prepare_main_elem(double **matrix, double *extension, int order);

int gauss(double **matrix, double *extension, int order, double *solve)
{
	int ret = 0;
	int i,j,k;
	if (matrix == NULL || extension == NULL || solve == NULL) {
		ret = -1;
		goto out;
	}
	
	if ((ret = prepare_main_elem(matrix, extension, order)) != 0) {
		goto out;
	}
		
	for (i = 0; i < order; ++i){
		if (matrix[i][i] == 0){				
			ret = -2;
			goto out;
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

out:	
	return ret;
}

int resid(double **matrix, double *extension, double *solve, int order, double *resid)
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

int resid_norm(double *resid, int order, double *norm)
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
