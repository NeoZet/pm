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

def linear_coef_correl(data):
    aver_x = mean([point[X_LABEL] for point in data])
    aver_y = mean([point[Y_LABEL] for point in data])
    n = len(data)

    y2Sum = 0.0
    x2Sum = 0.0
    xySum = 0.0
    for i in range(n):
        xySum += (data[i][X_LABEL] - aver_x) * (data[i][Y_LABEL] - aver_y)
        x2Sum += (data[i][X_LABEL] - aver_x)**2
        y2Sum += (data[i][Y_LABEL] - aver_y)**2
    return xySum / np.sqrt(x2Sum * y2Sum)


def spirman_coef_correl(data):
    n = len(data)
    rsum = 0.0
    tmpdata = copy.deepcopy(data)
    sorted_data = sorted(tmpdata, key=lambda k: k['x'])
    x_rgs = [i for i in range(len(sorted_data))]
    y_rgs = [point['id'] for point in sorted_data]

    for (x_rg, y_rg) in zip(x_rgs, y_rgs):
        rsum += np.square(x_rg - y_rg)
    return 1 - (6*rsum / (n * (np.square(n) - 1)))


if __name__ == '__main__':    
    parser = create_parser()
    args = parser.parse_args()
    if args.one_dimensional:
        sys.exit(1)
    if args.two_dimensional:
        data_2d = read_data_2d(args.two_dimensional)
        rl = linear_coef_correl(data_2d)
        rs = spirman_coef_correl(data_2d)
        print("Linear: r =", rl)
        print("Spirman: r =", rs)

        stat_crit = lambda r: r * np.sqrt(len(data_2d)-2) / np.sqrt(1 - np.square(r))
        tlin = abs(stat_crit(rl))
        tspir = abs(stat_crit(rs))

        al_1 = 0.1
        al_05 = 0.05

        Z_al9 = 1.64
        Z_al95 = 1.96
        
        if tlin > Z_al9:
            print("Линейный коэффициент r = ", rl, "значим при al=0.1")
        else:
            print("Линейный коэффициент r = ", rl, "не значим при al=0.1")
        if tlin > Z_al95:
            print("Линейный коэффициент r = ", rl, "значим при al=0.05")
        else:
            print("Линейный коэффициент r = ", rl, "не значим при al=0.05")
        if tspir > Z_al9:
            print("Спирмана коэффициент r = ", rs, "значим при al=0.1")
        else:
            print("Спирмана коэффициент r = ", rs, "не значим при al=0.1")
        if tspir > Z_al95:
            print("Спирмана коэффициент r = ", rs, "значим при al=0.05")
        else:
            print("Спирмана коэффициент r = ", rs, "не значим при al=0.05")