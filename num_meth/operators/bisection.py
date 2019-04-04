import matplotlib.pyplot as plt
import numpy as np
import sys
EPS = 0.001
F = 1

LEFT_BOUND = [-10.0, -20, -7]
RIGHT_BOUND = [10.0, 20, 7]

def func(x, F):
    return 6 - np.exp(-2 * x) + 2 * x - F

def bisection(equation, left_bound, right_bound, F, eps):
    mid = (left_bound + right_bound) / 2
    iter = 0
    while np.abs(left_bound - right_bound) and np.abs(equation(mid, F)) > eps:
        f_left = equation(left_bound, F)
        f_mid = equation(mid, F)
        f_right = equation(right_bound, F)
        
        if f_left * f_mid > 0:
            left_bound = mid
        elif f_left * f_mid < 0:
            right_bound = mid
        mid = (left_bound + right_bound) / 2
        iter += 1
    return mid, iter


def main():
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

    plt.title("Bisection method")
    plt.ylim(-25, 30)
    plt.xlim(-20, 20)

    print('########################################\n')
    print("Bisection method\n")
    for left, right in zip(LEFT_BOUND, RIGHT_BOUND):
        print('Initial bounds: [{0}, {1}]'.format(left, right))
        bisection_solution, iterations = bisection(func, left, right, F, EPS)
        print('Solution: {0:.5f} | Iterations: {1}'.format(bisection_solution, iterations))
        print('Accuracy: {0}'.format(func(bisection_solution, F)))
        print('-----------------------------------------------')
    print('\n########################################')

    plt.plot(eq_x_list, eq_y_list, linewidth=2, label="y = 6 - e^(-2x) + 2x")
    plt.plot(F_x_list, F_y_list, linewidth=2, color='red', label="F = -1")

    
    plt.plot(bisection_solution,
             F,
             marker='o',
             markersize=4,
             color='black',
             label='solution'
    )
    plt.annotate("[{0:.5f}, {1}]".format(bisection_solution, F), (bisection_solution, F))
    plt.legend(loc='best')
    plt.savefig('Bisection.png', dpi=300)
    
if __name__ == '__main__':
    main()
