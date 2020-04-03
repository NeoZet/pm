import matplotlib.pyplot as plt
import numpy as np
import math

import sys
import argparse


np.set_printoptions(precision=3, suppress=True)

X_LABEL = 'x'
Y_LABEL = 'y'

# Comand line parser
def create_parser():
    parser = argparse.ArgumentParser(
        formatter_class=argparse.RawTextHelpFormatter
    )
    parser.add_argument('-o', '--one-dimensional',
                        help='File with one-dimensional data array',
                        default=''
    )
    parser.add_argument('-t', '--two-dimensional',
                        help='File with two-dimensional data array',
                        default=''
    )
    return parser

# Read data functions

def read_raw_data(filename):
    try:
        with open(filename, 'r') as file:
            raw_data = file.readlines()
    except FileNotFoundError as ex:
        print(ex)
        return None

    return ''.join(raw_data).replace('\n', ' ')

def read_data_2d(filename):
    raw_data = read_raw_data(filename)
    if raw_data is None:
        return None
    data = [point.replace('(', '').replace(')', '').split(';') for point in raw_data.split(', ')]
    points = [{X_LABEL: float(point[0]), Y_LABEL: float(point[1])} for point in data if len(point) == 2]
    for i in range(len(points)):
        points[i].update({'id': i})
    return points


def mean(data):
    return sum(data) / len(data)

def dispers(data):
    dispers = 0
    aver = mean(data)
    for elem in data:
        dispers += ((elem - aver) ** 2) / len(data)
    return dispers

def list_log_sum(array):
    log = 0
    for i in array:
        log += math.log(i)
    return log

def list_square_log_sum(array):
    log = 0
    for i in array:
        log += math.log(i)**2
    return log

def lists_log_sum(array1, array2):
    log = 0
    for x, y in zip(array1, array2):
        log += math.log(x)*math.log(y)
    return log


if __name__ == '__main__':    
    parser = create_parser()
    args = parser.parse_args()
    if args.one_dimensional:
        sys.exit(1)
    if args.two_dimensional:
        data = read_data_2d(args.two_dimensional)

    X = [point[X_LABEL] for point in data]
    Y = [point[Y_LABEL] for point in data]
    n = len(data)

    sortX = sorted(X)
    sortY = sorted(Y) 

    plt.scatter(X, Y)
    plt.ylabel('Y')
    plt.xlabel('X')
    plt.show()

    plt.plot(sortX, sortY)
    plt.ylabel('Y')
    plt.xlabel('X')

    # y = a*x**b
    x_aver = (sortX[0]*sortX[n-1])**(0.5)
    y_aver = (sortY[0]*sortY[n-1])**(0.5)
    plt.scatter(x_aver,y_aver)
    plt.annotate(1, (x_aver, y_aver))
    
    # y = ab**x
    x_aver = (sortX[0]+sortX[n-1])/2 
    plt.scatter(x_aver,y_aver)
    plt.annotate(2, (x_aver, y_aver))

    # y = 1/(a+bx)
    y_aver = 2*sortY[0]*sortY[n-1]/(sortY[0]+sortY[n-1])
    plt.scatter(x_aver,y_aver)
    plt.annotate(3, (x_aver, y_aver))

    # y = a+b*lg(x)
    x_aver = (sortX[0]*sortX[n-1])**(0.5)
    y_aver = (sortY[0]+sortY[n-1])/2
    plt.scatter(x_aver,y_aver)
    plt.annotate(4, (x_aver, y_aver))

    # y = a + b/x
    x_aver = 2*sortX[0]*sortX[n-1]/(sortX[0]+sortX[n-1])
    plt.scatter(x_aver,y_aver)
    plt.annotate(5, (x_aver, y_aver))

    # y = ax/(b+x)
    y_aver = 2*sortY[0]*sortY[n-1]/(sortY[0]+sortY[n-1])
    plt.scatter(x_aver,y_aver)
    plt.annotate(6, (x_aver, y_aver))

    plt.show()

    x_aver = mean(X)
    print('Mean X =', x_aver)

    y_aver = mean(Y)
    print('Mean Y =', y_aver)

    x_dispers = dispers(X)
    y_dispers = dispers(Y)

    print('x dispers =', x_dispers)
    print('y dispers =', y_dispers)

    #y = a*x**b -> Y = A+b*X, Y = lny, A = lna, X = lnX
    A = [[n, list_log_sum(X)],
         [list_log_sum(X),  list_square_log_sum(X)]]
    B = [list_log_sum(Y), lists_log_sum(X, Y)]
    res = np.linalg.solve(A, B)

    a = res[1]
    b = res[0]

    print('y =', a,'x +', b, ' - empirical regression equation')

    y = []
    for i in range(n):
    	y.append(a*math.log(X[i]) + b)


    x_aver = mean(X)
    y_aver = mean(Y)

    res = 0
    for i in range(n):
    	res += (Y[i] - y[i])**2

    resSr = 0
    for i in range(n):
    	resSr += (Y[i] - y_aver)**2

    R2 = 1 - resSr/res
    print('R^2 =', R2)

    F = ((R2)*(n-2))/(1-R2)
    print('F =', F, ' > Fcrit=1.7, the obtained regression equation statistically significantly describes the results of the experiment')