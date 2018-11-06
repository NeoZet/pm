#include <stdio.h>
#include <stdlib.h>
#include <types.h>
#include <math.h>

#define EPS 0.0001

typedef int(*func_ptr)(int64_t);

int trapezoidal_rule(func_ptr integrate_function, lower_limit, upper_limit, epsilon);
int trapezoidal_rule(func_ptr integrate_function, lower_limit, upper_limit, epsilon);

static int integrate_function(int64_t x);
static int integral(double step);
static int check_accuracy(double epsilon, double step);

int main()
{

}

static int integrate_function(int64_t x)
{
	return 1/(1 + pow(x, 1/2));
}

static int integral(func_ptr integrate_function, double lower_limit, double upper_limit, double step)
{

}

static int check_accuracy(double epsilon, double step);

