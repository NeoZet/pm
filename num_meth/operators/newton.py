import matplotlib.pyplot as plt
import numpy as np

EPS = 0.001
F = 1

left_bound = -30.0
right_bound = 30.0

def func(x, F):
    return 6 - np.exp(-2 * x) + 2 * x - F

def derivative(function, x, F, h=0.0001):
    return (function(x + h, F) - function(x - h, F)) / (2 * h)


# TODO change name
def newton_step(equation, x, F):
    return (x - equation(x, F) / derivative(equation, x, F))

def newton(equation, x0, F, eps):
    x = x0
    x_next = newton_step(equation, x0, F)
    while np.abs(x_next - x) > eps:
        x = x_next
        x_next = newton_step(equation, x_next, F)
    return x_next


def main():
    start_values = np.array([0.1, 0.001, 1])

    x_list = np.arange(left_bound, right_bound, 0.01)
    y_list = np.array([func(x, 0) for x in x_list])
    print(func(-4, 0))
    plt.plot(x_list, y_list, linewidth=2)
    plt.title("Newton's method")
    plt.ylim(-25, 30)
    plt.xlim(-20, 20)
    plt.grid(True)
    plt.show()
    
if __name__ == '__main__':
    main()
    
        
