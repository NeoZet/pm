import matplotlib.pyplot as plt
import numpy as np

EPS = 0.001
F = 1

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
    x0 = 0.1
    
    eq_x_list = np.arange(-40, 40, 0.01)
    eq_y_list = np.array([func(x, 0) for x in eq_x_list])

    F_x_list = np.arange(-40, 40, 0.01)
    F_y_list = np.array([F for x in F_x_list])
    
    fig,ax = plt.subplots()
    ax.grid(True)
    ax.tick_params(axis='x', which='major', direction='inout',
                bottom=True, top=False, left=True, right=False, 
                color='b', labelcolor='black',
                labelbottom=True, labeltop=False, labelleft=True, labelright=False)

    plt.title("Newton's method")
    plt.ylim(-25, 30)
    plt.xlim(-20, 20)

    newton_solution = newton(func, x0, F, EPS)
    
    plt.plot(eq_x_list, eq_y_list, linewidth=2, label="y = 6 - e^(-2x) + 2x")
    plt.plot(F_x_list, F_y_list, linewidth=2, color='red', label="F = 1")
    plt.plot(newton_solution,
             F,
             marker='o',
             markersize=4,
             color='black',
             label='solution'
    )
    plt.annotate("[{0:.5f}, {1}]".format(newton_solution, F), (newton_solution, F))
    plt.legend(loc='best')
    plt.show()
    
if __name__ == '__main__':
    main()
    
        
