format long
global LOWER_LIMIT, UPPER_LIMIT;
LOWER_LIMIT = 0;
UPPER_LIMIT = 4;
global eps_4, eps_5;
eps_4 = 0.0001;
eps_5 = 0.00001;

integrate_function = @(x) (1/(1 + x^(0.5)));

function result = trapezoid_rule(func, lower_limit, upper_limit, step)
    x_list = lower_limit : step : upper_limit;
    y_list = lower_limit : length(x_list)-1;        
    for i = 1:length(x_list)        
        y_list(i) = func(x_list(i));        
    end
    h = (upper_limit - lower_limit) / length(x_list);
    result = h/2*(y_list(1) + ...
    2 * sum(y_list(2:length(y_list)-1)) + ...
    y_list(length(y_list)));
end

function result = simpson_rule(func, lower_limit, upper_limit, step)
    x_list = lower_limit : step : upper_limit;
    y_list = lower_limit : length(x_list)-1;        
    for i = 1:length(x_list)        
        y_list(i) = func(x_list(i));        
    end
    h = (upper_limit - lower_limit) / length(x_list);
    result = h/3*(y_list(1) + ...
    4 * sum(y_list(2:2:length(y_list)-1)) + ...
    2 * sum(y_list(3:2:length(y_list)-1)) + ...
    y_list(length(y_list)));
end


function [integral, accuracy] = check_accuracy(integral_cacl, func, lower_limit, upper_limit, step, epsilon, coef)
    integral_old = integral_cacl(func, lower_limit, upper_limit, step);
    step = step / 2;
    integral_new = integral_cacl(func, lower_limit, upper_limit, step);    
    accuracy_tmp = abs(integral_new - integral_old) / coef;
    integral_old = integral_new;
    while (accuracy_tmp > epsilon)
        step = step / 2;        
        integral_new = integral_cacl(func, lower_limit, upper_limit, step);    
        accuracy_tmp = abs(integral_new - integral_old) / coef;
        integral_old = integral_new;
    end
    integral = integral_new;
    accuracy = accuracy_tmp;
end

printf("TRAPEZOIDAL\n");
[integral, accuracy] = check_accuracy(@trapezoid_rule, integrate_function, LOWER_LIMIT, UPPER_LIMIT, 1, eps_4, 3);
printf("Integral: %.10f | accuracy: %.10f | epsilon: 0.0001\n", integral, accuracy);
[integral, accuracy] = check_accuracy(@trapezoid_rule, integrate_function, LOWER_LIMIT, UPPER_LIMIT, 1, eps_5, 3);
printf("Integral: %.10f | accuracy: %.10f | epsilon: 0.00001\n", integral, accuracy);
printf("============================================\n");
printf("SIMPSON\n");
[integral, accuracy] = check_accuracy(@simpson_rule, integrate_function, LOWER_LIMIT, UPPER_LIMIT, 1, eps_4, 3);
printf("Integral: %.10f | accuracy: %.10f | epsilon: 0.0001\n", integral, accuracy);
[integral, accuracy] = check_accuracy(@simpson_rule, integrate_function, LOWER_LIMIT, UPPER_LIMIT, 1, eps_5, 3);
printf("Integral: %.10f | accuracy: %.10f | epsilon: 0.00001\n", integral, accuracy);