import matplotlib.pyplot as plt
import numpy as np

import argparse
import copy
import sys

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

def read_data_1d(filename):
    raw_data = read_raw_data(filename)
    if raw_data is None:
        return None
    return [float(point) for point in raw_data.split(';')]

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

def regr_func(a, b, x):
    return float(a)*float(x) + float(b)

def significance(data, a, b):
    aver_x = mean([point[X_LABEL] for point in data])
    n = len(data)
    Sx = 0
    S = 0
    for i in range(n):
        S += (data[i][Y_LABEL] - regr_func(a, b, data[i][X_LABEL]))**2
        Sx += (data[i][X_LABEL] - aver_x)**2
    S = (S/(n-2)) ** (0.5)
    Sx = (Sx/(n-1)) ** (0.5)
    Sa = S/(Sx*(n-1)**0.5)
    Sb = S * (((1/n) + (aver_x**2)/((n-1) * Sx**2)) ** (0.5))

    tst095 = 2.0345
    print('-----------------')
    if abs(a) > tst095 * Sa:
        print("a = ", a, "- значимый коэффициент")
    if abs(b) > tst095 * Sb:
        print("b =", b, " - значимый коэффициент") 


def regressModel(data):
    n = len(data)
    xSum = 0.0
    ySum = 0.0
    x2Sum = 0.0
    xySum = 0.0
    for i in range(n):
        xSum += data[i][X_LABEL]
        ySum += data[i][Y_LABEL]
        x2Sum += data[i][X_LABEL]**2
        xySum += data[i][X_LABEL]*data[i][Y_LABEL]

    A = np.array([[n, xSum], [xSum, x2Sum]])
    B = np.array([ySum, xySum])
    res = np.linalg.solve(A, B)
    print("a = ", res[1])
    print("b = ", res[0])

    significance(data, res[1], res[0])

    data = sorted(data, key=lambda k: k['x'])
    dataYLinReg = []
    dataX = []
    dataY = []
    for i in range(n):
        dataYLinReg.append(regr_func(res[1], res[0], float(data[i][X_LABEL])))
        dataX.append(float(data[i][X_LABEL]))
        dataY.append(float(data[i][Y_LABEL]))

    plt.plot(dataX, dataY)
    plt.plot(dataX, dataYLinReg)
    plt.ylabel('yi')
    plt.xlabel('xi')
    plt.show()


if __name__ == '__main__':    
    parser = create_parser()
    args = parser.parse_args()
    if args.one_dimensional:
        sys.exit(1)
    if args.two_dimensional:
        data_2d = read_data_2d(args.two_dimensional)
        regressModel(data_2d)
