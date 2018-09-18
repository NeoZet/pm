import matplotlib.pyplot as plt
import numpy as np
import math
import decimal

NUMBER_OF_FUNCTIONS = 10

def taylor_sin(arg, number_of_elements):
    res = 0
    for i in range(number_of_elements+1):
        res += ((-1) ** i) * (arg ** (2 * i + 1)) / math.factorial(2 * i + 1)
    return res

def partial_sum(sequence_func, args, number_of_elements):
    seq_part = []
    for arg in args:        
        seq_part.append(sequence_func(arg, number_of_elements))
    return seq_part

def create_sin(x_args):
    y = np.sin(x_args)
    plt.plot(x_args, y, color='black', linewidth=2)

def create_sin_approximation(args):
    plt.ylim(-2, 2)
    for elem_num in range(NUMBER_OF_FUNCTIONS):
        plt.plot(args, partial_sum(taylor_sin, args, elem_num), label='n={}'.format(elem_num))
    
def main():
    ax = plt.subplots()
    args = np.arange(-2 * np.pi, 2 * np.pi, 0.01)
    create_sin_approximation(args)
    create_sin(args)
    plt.grid(True)
    ax.legend(ncol=ncols, loc='best')
    
if __name__ == "__main__":
    main()
    plt.show()
