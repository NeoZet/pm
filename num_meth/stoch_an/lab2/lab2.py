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



if __name__ == '__main__':    
    parser = create_parser()
    args = parser.parse_args()
    if args.one_dimensional:
        data_1d = read_data_1d(args.one_dimensional)
        gamma = 0.95
        Z_alph95 = 1.96
        disp = dispers(data_1d)
        stand_dev = standard_deviation(data_1d)
        aver = mean(data_1d)
        delta = Z_alph95 * stand_dev / np.sqrt(len(data_1d))
        print("Доверительный интервал при известной дисперсии D = {0}  для мат. ожидания (alpha = {1:.2f}):".format(disp, 1 - gamma))
        print("{0} < M < {1}".format(aver - delta, aver + delta))

        t09 = 1.3070
        t095 = 1.6909
        t099 = 2.4411

        delta = np.sqrt(disp/len(data_1d))
        print("Доверительный интервал при неизвестной дисперсии D для мат. ожидания при alpha = 0.1:")
        print(aver - t09*delta, "< M <", aver + t09*delta)
        print("Доверительный интервал при неизвестной дисперсии D для мат. ожидания при alpha = 0.05:")
        print(aver - t095*delta, "< M <", aver + t095*delta)
        print("Доверительный интервал при неизвестной дисперсии D для мат. ожидания при alpha = 0.01:")
        print(aver - t099*delta, "< M <", aver + t099*delta)

        alpha1 = 0.1
        alpha05 = 0.05
        alpha01 = 0.01

        Z_al9 = 1.64
        Z_al95 = 1.96
        Z_al99 = 2.576

        n = len(data_1d)
        bottom_border = lambda al: ((n - 1) * disp) / ((n - 1) + al * np.sqrt(2 * (n - 1)))
        top_border = lambda al: ((n - 1) * disp) / ((n - 1) - al * np.sqrt(2 * (n - 1)))
        print("Доверительный интервал для дисперсии D = {0:.5f} при alpha = 0.1:".format(disp))
        print("{0} < D < {1}".format(bottom_border(Z_al9), top_border(Z_al9)))
        print("Доверительный интервал для дисперсии D = {0:.5f} при alpha = 0.05:".format(disp))
        print("{0} < D < {1}".format(bottom_border(Z_al95), top_border(Z_al95)))
        print("Доверительный интервал для дисперсии D = {0:.5f} при alpha = 0.01:".format(disp))
        print("{0} < D < {1}".format(bottom_border(Z_al99), top_border(Z_al99)))                