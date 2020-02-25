import numpy as np

import argparse
import copy
import sys

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
    return [{X_LABEL: float(point[0]), Y_LABEL: float(point[1])} for point in data if len(point) == 2]


def dispers(data):
    dispers = 0
    aver = mean(data)
    for elem in data:
        dispers += np.square(elem - aver) / len(data)
    return dispers

def standard_deviation(data):
    return np.sqrt(dispers(data))

def mean(data):
    return sum(data) / len(data)

def dispS(v, Xsr):
	S2 = 0
	n = len(v)
	for i in range(n):
		S2 += ((v[i] - Xsr)**2)
	return np.sqrt((S2/(n-1)))

if __name__ == '__main__':    
    parser = create_parser()
    args = parser.parse_args()
    if args.one_dimensional:
        data_1d = read_data_1d(args.one_dimensional)
        gamma = 0.95
        lapl095 = 1.96
        disp = dispers(data_1d)
        stand_dev = standard_deviation(data_1d)
        aver = mean(data_1d)
        delta = lapl095 * stand_dev / np.sqrt(len(data_1d))
        print("Доверительный интервал при известной дисперсии D = {0}  для мат. ожидания (alpha = {1:.2f}):".format(disp, 1 - gamma))
        print("{0} < M < {1}".format(aver - delta, aver + delta))


# n = len(v)
# xsr = find_Xsr(v)
# lapl095 = 1.96
# disp = 300.0

# delta = lapl095*(disp**(0.5))/n**(0.5)
# print("Доверительный интервал при известной дисперсии D =", disp, " для мат. ожидания при alpha = 0.05:")
# print(xsr - delta, "< M <", xsr + delta)

# t09 = 1.3070
# t095 = 1.6909
# t099 = 2.4411
# disp = dispS(v, xsr)
# delta = disp/n**(0.5)
# print("Доверительный интервал при неизвестной дисперсии D для мат. ожидания при alpha = 0.1:")
# print(xsr - t09*delta, "< M <", xsr + t09*delta)
# print("Доверительный интервал при неизвестной дисперсии D для мат. ожидания при alpha = 0.05:")
# print(xsr - t095*delta, "< M <", xsr + t095*delta)
# print("Доверительный интервал при неизвестной дисперсии D для мат. ожидания при alpha = 0.01:")
# print(xsr - t099*delta, "< M <", xsr + t099*delta)