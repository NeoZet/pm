import matplotlib.pyplot as plt
import numpy as np
import math
import decimal

NUMBER_OF_FUNCTIONS = 10
ACCURACY = 0.001

def partial_sum(func_sequence, args, number_of_elements):
    partial_sums_list = []
    for arg in args:        
        partial_sums_list.append(func_sequence(arg, number_of_elements))
    return partial_sums_list


def taylor_sin(arg, number_of_elements):
    res = 0
    for i in range(number_of_elements+1):
        res += ((-1) ** i) * (arg ** (2 * i + 1)) / math.factorial(2 * i + 1)
    return res


def plot_sin(x_args):
    y = np.sin(x_args)
    plt.plot(x_args, y, color='black', linewidth=2, label='$y=sin(x)$')


def plot_partial_sums(func_sequence, args, number_of_members):
    for elem_num in range(number_of_members):
        plt.plot(args, partial_sum(func_sequence, args, elem_num), label='$n={}$'.format(elem_num + 1))


def create_approximation(func, number_of_members, accuracy):
    k = 0
    approximation = {}
    for elem_num in range(number_of_members):
        while np.fabs(np.sin(k) - func(k, elem_num)) < accuracy:
            k += accuracy
        k -= accuracy
        approximation[elem_num] = float('{:.3f}'.format(k))
    return approximation
            

def plot_approximation_result(approximation_result):
    plt.plot(approximation_result.keys(), approximation_result.values(), 'ro', label='approximate value')
    plt.xticks(np.arange(int(min(approximation_result.keys()) - 1),
                         int(max(approximation_result.keys()) + 1),
                         1))
    plt.yticks(np.arange(int(min(approximation_result.values()) - 1),
                         int(max(approximation_result.values()) + 1),
                         0.5))


def final_plot():
    args = np.arange(-3 * np.pi, 3 * np.pi, 0.01)
    plt.figure(1)
    plot_partial_sums(taylor_sin, args, NUMBER_OF_FUNCTIONS)
    plt.ylim(-2, 2)
    plot_sin(args)

    text_note = "Note: 'n' - number of elements in partial\nsum of Taylor series for sin"
    plt.text(4, -2.5, text_note, fontsize=7)
    plt.subplots_adjust(right=0.9)
    plt.legend(loc=3, ncol=6, mode="expand")
    plt.title("Approximation by curves for " + r"$y=sin(x)$")
    plt.xlabel('X, [rad]')
    plt.ylabel('Y, [ ]')
    plt.grid(True)
    
    plt.figure(2)
    plot_approximation_result(create_approximation(taylor_sin,
                                                   NUMBER_OF_FUNCTIONS,
                                                   ACCURACY))
    plt.legend(loc=2)
    plt.title("Approximation of " + r"$y=sin(x)$" + " with accuracy = {}".format(ACCURACY))
    plt.xlabel('Iterations, [ ]')
    plt.ylabel('Values, [ ]')    
    plt.grid(True)
    
def main():
    final_plot()


if __name__ == "__main__":
    main()
    plt.show()
