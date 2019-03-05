#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "utils/gauss.h"

#define NUM_OF_EQ 2
#define MAX_ITER 50
static const double E_DOP = 0.00001;

typedef int (*ftype)(double*, int, double**);

int euler();

static int f(double *x, int num_of_eq, double **result);
static double step(ftype eq_system, double *xk, int num_of_eq);

int main()
{

}

int euler()
{
	return 0;
}

static int f(double *x, int num_of_eq, double **result)
{
	int ret = 0;
	if (x == NULL || (*result) == NULL) {
		ret = -1;
		goto out;
	}       
	
out:
	return ret;
}


static double step(ftype eq_system, double *xk, int num_of_eq)
{

}

