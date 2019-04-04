import matplotlib.pyplot as plt
import numpy as np

EPS = 0.001
F = 1
TAU = 0.000001 #magic number

def func(x, F):
    return 6 - np.exp(-2 * x) + 2 * x - F

def implicit(equation, x0, F, eps):
    def _temp_eq(func, x, x_prev, F, t):
        return x_prev - t * func(x, F) - x

    def _temp_eq_deriv(func, x, x_prev, F, t, h=0.0001):
        return (_temp_eq(func, x+h, x_prev, F, t) - _temp_eq(func, x-h, x_prev, F, t)) / (2 * h)
    
    def _newton_step(equation, x, x_prev, F, t):
        return (x - _temp_eq(equation, x, x_prev, F, t) / _temp_eq_deriv(equation, x, x_prev, F, t))

    def __t_n(n, tau):
        return 1 / n ** tau
    
    x = x0
    x_prev = x
    t = __t_n(1, TAU)
    x_next = _newton_step(equation, x0, x0, F, t)
    iter = 0
    while np.abs(x_next - x) > eps:
        iter += 1
        x_prev = x
        x = x_next
        t = __t_n(iter, TAU)
        x_next = _newton_step(equation, x_next, x, F, t)
    return x_next, iter


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

    plt.title("Implicit iteration method")
    plt.ylim(-25, 30)
    plt.xlim(-20, 20)

    print("Implicit iteration method")
    implicit_solution, iterations = implicit(func, x0, F, EPS)
    print('Solution: {0:.5f} | Iterations: {1}'.format(implicit_solution, iterations))
    
    plt.plot(eq_x_list, eq_y_list, linewidth=2, label="y = 6 - e^(-2x) + 2x")
    plt.plot(F_x_list, F_y_list, linewidth=2, color='red', label="F = 1")
    plt.plot(implicit_solution,
             F,
             marker='o',
             markersize=4,
             color='black',
             label='solution'
    )
    plt.annotate("[{0:.5f}, {1}]".format(implicit_solution, F), (implicit_solution, F))
    plt.legend(loc='best')
    plt.savefig('Implicit.png', dpi=300)
    
if __name__ == '__main__':
    main()
    print(func(implicit(func, 0.1, F, EPS)[0], F))
    
