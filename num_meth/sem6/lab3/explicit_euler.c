#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

#define NUM_OF_EQ 2

static const double E_DOP = 0.00001;
double _u_0[2] = {0, -0.412};
double u_0[2] = {3, 0};
int Om = 40;
double tau_max = 0.01;
double T = 1;

#define a (2.5 + Om/40)

typedef int (*ftype)(double*, int, double**);

int euler();

static int f(double *x, double t, double result [][NUM_OF_EQ]);

static double step(double *f);

int main()
{
	euler();
}

int euler()
{
	double t_k = 0.00000001;
	double y_k[NUM_OF_EQ];
	memcpy(y_k, u_0, sizeof(double) * NUM_OF_EQ);
	double f_vec[NUM_OF_EQ];
	int k = 0;
	while (t_k < T) {
		f(y_k, t_k, &f_vec);
		double tau_k = step(f_vec);
		for (int i = 0; i < NUM_OF_EQ; ++i) {
			y_k[i] = y_k[i] + tau_k * f_vec[i];
		}
		t_k += tau_k;

		printf("K: [%d]\nY: [\n", k);
		for (int i = 0; i < NUM_OF_EQ; ++i) {
			printf("\tU%d: %lf\n", i, y_k[i]);
		}
		printf("]\n\nT: [%lf]\n", t_k);
		printf("---------------------\n");
		k++;
	}
	
	return 0;
}

static int f(double *u, double t, double result[][NUM_OF_EQ])
{
	int ret = 0;
	if (u == NULL || (*result) == NULL) {
		ret = -1;
		goto out;
	}       

	(*result)[0] = -2*u[0] + 4*u[1];
	(*result)[1] = -u[0] + 3*u[1];

	/* (*result)[0] = -u[0]*u[1] + sin(t) / t; */
	/* (*result)[1] = -(u[1]*u[1]) + a*t/(1+t*t); */
		
out:
	return ret;
}


static double step(double *f)
{
	double min = DBL_MAX;
	double tau[NUM_OF_EQ]; 
	for(int i = 0; i < NUM_OF_EQ; ++i) {
		tau[i] = E_DOP / fabs(f[i]) + E_DOP / tau_max;
		if (tau[i] < min) {
			min = tau[i];
		}
	}
	return min;
}

