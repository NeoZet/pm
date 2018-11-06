import numpy as np

def trapezoidal_rule(integrate_function, lower_limit, upper_limit, epsilon):
    def _integral(step):
        x_list = np.arange(lower_limit, upper_limit+step, step)
        y_list = [integrate_function(x_list[i]) for i in range(len(x_list))]
        h = (x_list[-1] - x_list[0]) / len(x_list)
        return h/2 * (y_list[0] +
                      2 * sum([y_list[i] for i in range(1, len(y_list)-1)]) +
                      y_list[-1])

    def _check_accuracy(epsilon, step):
        integral = _integral(step)
        accuracy = abs(_integral(step / 2) - integral)
        if accuracy / 3 < epsilon:
            return [integral, accuracy]
        return _check_accuracy(epsilon, step / 10)

    return _check_accuracy(epsilon, 1)


def simpson_rule(integrate_function, lower_limit, upper_limit, epsilon):
    def _integral(step):
        x_list = np.arange(lower_limit, upper_limit+step, step)
        y_list = [integrate_function(x_list[i]) for i in range(len(x_list))]
        num = len(x_list)
        h = (x_list[-1] - x_list[0]) / num
        return h/3 * (y_list[0] +
                      y_list[-1] +
                      2 * sum([y_list[2 * i] for i in range(1, num // 2)]) +
                      4 * sum([y_list[2 * i - 1] for i in range(1, num // 2 + 1)]))

    def _check_accuracy(epsilon, step):
        integral = _integral(step)
        accuracy = abs(_integral(step / 2) - integral) / 15
        if accuracy < epsilon:
            return [integral, accuracy]
        return _check_accuracy(epsilon, step / 10)
                    
    return _check_accuracy(epsilon, 1)
    

def main():
    func = lambda x: 1/(1 + x ** (1/2))
    lower_limit = 0
    upper_limit = 4
    eps = 10 ** (-5)
    integral, accuracy = trapezoidal_rule(func, lower_limit, upper_limit, eps)
    integral_1, accuracy_1 = simpson_rule(func, lower_limit, upper_limit, eps)
    print("###")
    print('trapezoidal: {0} -::- accuracy: {1}'.format(integral, accuracy))
    print('------------------------------------')
    print('simpson: {0} -::- accuracy: {1}'.format(integral_1, accuracy_1))
    print("###")


if __name__ == "__main__":
    main()
