#include <stdio.h>
#include <stdlib.h>
#include <types.h>
#include <math.h>

#define EPS 0.0001
#define START_STEP 1
#define MAX_ARRAY_SIZE 1024

typedef double(*func_ptr)(double);

const double lower_intergration_limit = 0;
const double upper_intergration_limit = 0;

double trapezoidal_rule(func_ptr integrate_function, double lower_limit, double upper_limit, double epsilon, double *accuracy);
double simpson_rule(func_ptr integrate_function, double lower_limit, double upper_limit, double epsilon, double *accuracy);

static double integrate_function(double x);
static double integral_calculation(double step);
static int check_accuracy(double epsilon, double step, double *integral, double *accuracy)

int main()
{

}

double trapezoidal_rule(func_ptr integrate_function, double lower_limit, double upper_limit, double epsilon, double *accuracy)
{
	double integral = 0;
	if(lower_limit == upper_limit) {
		goto out;
	}
	lower_intergration_limit = lower_limit;
	upper_intergration_limit = upper_limit;
	check_accuracy(epsilon, START_STEP, &integral, accuracy);

 out:
	return integral;
}

static double integrate_function(double x)
{
	return 1/(1 + pow(x, 1/2));
}

static double integral_calculation(func_ptr integrate_function, double step)
{
	double x_list[MAX_ARRAY_SIZE];
	double y_list[MAX_ARRAY_SIZE];
	double x_value;
	uint_t arr_iter;
	double y_value;	
	int result = 0;

	if(lower_integration_limit == upper_integration_limit) {
		goto out;
	}
	
	for(arr_iter = 0, x_value = lower_integration_limit;
	    x_value <= upper_integration_limit;
	    ++arr_iter, x_value += step)
	{
		if(arr_iter == MAX_ARRAY_SIZE) {
			fprintf(stderr, "WARNING! TOO LITTLE STEP!\n");
			break;
		}
		x_list[arr_iter] = x_value;
		y_list[arr_iter] = integrate_function(x_value);
	}
	double h = (x_list[arr_iter] - x_list[0]) / arr_iter+1;
        double y_list_sum = 0;
	uint_t i;
	for(i = 0; i < arr_iter; ++i) {
		y_list_sum += y_list[i];
	}
	result =  h/2 * (y_list[0] +
			 2 * y_list_sum +
			 y_list[arr_iter]);

 out:
	return result;
}

static int check_accuracy(func_ptr integrate_function, double step, double coeff, double *integral, double *accuracy)
{
	double integral_tmp = 0;
	double accuracy_tmp = 0;
	int ret = 0;
	if (integral == NULL) {
		fprintf(stderr, "ERROR! NULL POINTER PASSED!\n");
		ret = -1;
		goto out;
	}
	
	do {
		integral_tmp = integral_calculation(integrate_function, step);
		accuracy_tmp = fabs(integral_calculation(integrate_function, step / 2) - integral_tmp);
		step /= 10;
	} while(epsilon <= accuracy_tmp / coeff);

	(*integral) = integral_tmp;
	if(accuracy != NULL) {
		(*accuracy) = accuracy_tmp;
	}
	
 out:
	return ret;
}

