import matplotlib.pyplot as plt
import numpy as np
import math
import decimal

NUMBER_OF_FUNCTIONS = 10

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
    plt.ylim(-2, 2)
    for elem_num in range(number_of_members):
        plt.plot(args, partial_sum(func_sequence, args, elem_num), label='$n={}$'.format(elem_num))


def create_approximation(func, args, number_of_members, accuracy):
    
        

def main():
    args = np.arange(-2 * np.pi, 2 * np.pi, 0.01)

    plot_partial_sums(taylor_sin, args, NUMBER_OF_FUNCTIONS)    
    plot_sin(args)

    plt.legend(loc=3, ncol=6, mode="expand")
    plt.title("Approximation of " + r"$y=sin(x)$")
    plt.xlabel('X, [rad]')
    plt.ylabel('Y, [ ]')
    plt.grid(True)
    
if __name__ == "__main__":
    main()
    plt.show()
