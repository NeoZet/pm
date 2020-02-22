import matplotlib.pyplot as plt
import numpy as np
import sys

np.set_printoptions(precision=6, suppress=True)

X_LABEL = 'x'
Y_LABEL = 'y'

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


if __name__ == '__main__':
    print(read_data_2d(sys.argv[1]))