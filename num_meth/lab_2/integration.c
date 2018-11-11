#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define EPS_4 0.0001
#define EPS_5 0.00001
#define START_STEP 1.0
#define MAX_ARRAY_SIZE 1024 * 1024

#define LOW_LIMIT 0
#define UP_LIMIT 4 

enum INTEGRATION_RULES {
	TRAPEZOIDAL = 0,
	SIMPSON
};

typedef long double float80_t;
typedef float80_t(*func_ptr)(float80_t);

float80_t lower_integration_limit = 0;
float80_t upper_integration_limit = 0;
float80_t epsilon = 0;
float80_t accuracy_coef = 1;
enum INTEGRATION_RULES RULE = 0;

float80_t trapezoidal_rule(func_ptr integrate_function, float80_t lower_limit, float80_t upper_limit, float80_t epsilon, float80_t *accuracy);
float80_t simpson_rule(func_ptr integrate_function, float80_t lower_limit, float80_t upper_limit, float80_t epsilon, float80_t *accuracy);

static int check_accuracy(func_ptr integrate_function, float80_t step, float80_t *integral, float80_t *accuracy);
static float80_t integrate(func_ptr integrate_function, float80_t lower_limit, float80_t upper_limit, float80_t eps, float80_t *accuracy);
static float80_t integrate_function(float80_t x);
static float80_t integral_calculation(func_ptr integrate_function, float80_t step);
static float80_t integral_by_trapezoidal(float80_t *y_list, int y_list_size, float80_t h);
static float80_t integral_by_simpson(float80_t *y_list, int y_list_size, float80_t h);


int main()
{
	void start(float80_t epsilon)
	{
		float80_t accuracy_trapezoidal = 0;
		float80_t integral_trapezoidal = trapezoidal_rule(integrate_function, LOW_LIMIT, UP_LIMIT, epsilon, &accuracy_trapezoidal);	
	
		float80_t accuracy_simpson = 0;
		float80_t integral_simpson = simpson_rule(integrate_function, LOW_LIMIT, UP_LIMIT, epsilon, &accuracy_simpson);
	
		printf("trapezoidal: %.15Lf =::= epsilon: %Lf =::= accuracy: %.15Lf\n", integral_trapezoidal, epsilon, accuracy_trapezoidal);
		printf("simpson    : %.15Lf =::= epsilon: %Lf =::= accuracy: %.15Lf\n", integral_simpson, epsilon, accuracy_simpson);
	}

	start(EPS_4);
	start(EPS_5);
}

float80_t trapezoidal_rule(func_ptr integrate_function, float80_t lower_limit, float80_t upper_limit, float80_t eps, float80_t *accuracy)
{
	RULE = TRAPEZOIDAL;
	accuracy_coef = 3;
	return integrate(integrate_function, lower_limit, upper_limit, eps, accuracy);
}

float80_t simpson_rule(func_ptr integrate_function, float80_t lower_limit, float80_t upper_limit, float80_t eps, float80_t *accuracy)
{
	RULE = SIMPSON;
	accuracy_coef = 15;
	return integrate(integrate_function, lower_limit, upper_limit, eps, accuracy);
}

static float80_t integrate_function(float80_t x)
{
	return 1.0/(1 + pow(x, 1.0/2));
}

static float80_t integrate(func_ptr integrate_function, float80_t lower_limit, float80_t upper_limit, float80_t eps, float80_t *accuracy)
{
	float80_t integral = 0;
	if(lower_limit == upper_limit) {
		goto out;
	}
	lower_integration_limit = lower_limit;
	upper_integration_limit = upper_limit;
	epsilon = eps;
	check_accuracy(integrate_function, START_STEP, &integral, accuracy);
 out: 
	return integral;
}

static float80_t integral_calculation(func_ptr integrate_function, float80_t step)
{
	if(lower_integration_limit == upper_integration_limit) {
		goto out;
	}
	float80_t *x_list = (float80_t*)malloc(sizeof(float80_t) * MAX_ARRAY_SIZE);
	float80_t *y_list = (float80_t*)malloc(sizeof(float80_t) * MAX_ARRAY_SIZE);
	float80_t x_value = 0;
	uint32_t arr_iter = 0;
	float80_t y_value = 0;	
	float80_t result = 0;

	for(arr_iter = 0, x_value = lower_integration_limit;
	    x_value <= upper_integration_limit && arr_iter != MAX_ARRAY_SIZE;
	    ++arr_iter, x_value += step)
		{
			x_list[arr_iter] = x_value;		
			y_list[arr_iter] = integrate_function(x_value);				
		}
	float80_t h = (x_list[arr_iter-1] - x_list[0]) / arr_iter;
	
	switch (RULE) {
	case TRAPEZOIDAL:
		result = integral_by_trapezoidal(y_list, arr_iter, h);
		break;

	case SIMPSON:
		result = integral_by_simpson(y_list, arr_iter, h);
		break;
	}

	free(x_list);
	free(y_list);
 out:	
	return result;
}

static int check_accuracy(func_ptr integrate_function, float80_t step, float80_t *integral, float80_t *accuracy)
{
	float80_t integral_tmp = 0;
	float80_t accuracy_tmp = 0;
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
	} while(epsilon <= accuracy_tmp / accuracy_coef);

	(*integral) = integral_tmp;
	if(accuracy != NULL) {
		(*accuracy) = accuracy_tmp;
	}
	
 out:
 	return ret;
}

static float80_t integral_by_trapezoidal(float80_t *y_list, int y_list_size, float80_t h)
{
	float80_t y_list_sum = 0;						
	for(int i = 1; i < y_list_size-1; ++i) {			
		y_list_sum += y_list[i];				
	}								
	return h/2 * (y_list[0] +					
		      2 * y_list_sum +					
		      y_list[y_list_size-1]);					
}

static float80_t integral_by_simpson(float80_t *y_list, int y_list_size, float80_t h)
{
	float80_t sum_by_odd_index = 0;
	float80_t sum_by_even_index = 0;
	int i;
	for(i = 1; i < y_list_size / 2; ++i) {
		sum_by_odd_index += y_list[2 * i];
	}
	for(i = 1; i < y_list_size / 2 + 1; ++i) {
		sum_by_even_index += y_list[2 * i - 1];
	}
        return h/3 * (y_list[0] +
                      y_list[y_list_size-1] +
                      2 * sum_by_odd_index +
                      4 * sum_by_even_index);
}
     
 
