import matplotlib.pyplot as plt
import numpy as np

EPS = 0.001
F = 1
t = 0.1 #magic number

def func(x, F):
    return 6 - np.exp(-2 * x) + 2 * x - F

def explicit(equation, x0, F, eps):
    def _expl_on_step(eq, x_prev, F):
        return x_prev - t * eq(x_prev, F)
    
    x_prev = x0
    x = _expl_on_step(equation, x_prev, F)
    iter = 0
    while np.abs(x - x_prev) > eps:
        print(x)
        x_prev = x
        x = _expl_on_step(equation, x_prev, F)
        iter += 1
    return x, iter


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

    plt.title("Explicit iterative method")
    plt.ylim(-25, 30)
    plt.xlim(-20, 20)

    print("Explicit iteration method")
    explicit_solution, iterations = explicit(func, x0, F, EPS)
    print('Solution: {0:.5f} | Iterations: {1}'.format(explicit_solution, iterations))
    plt.plot(eq_x_list, eq_y_list, linewidth=2, label="y = 6 - e^(-2x) + 2x")
    plt.plot(F_x_list, F_y_list, linewidth=2, color='red', label="F = 1")
    plt.plot(explicit_solution,
             F,
             marker='o',
             markersize=4,
             color='black',
             label='solution'
    )
    plt.annotate("[{0:.5f}, {1}]".format(explicit_solution, F), (explicit_solution, F))
    plt.legend(loc='best')
    plt.savefig('Explicit.png', dpi=300)
    
if __name__ == '__main__':
    main()    
    print(func(explicit(func, 0.1, F, EPS)[0], F))
