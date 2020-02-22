import matplotlib.pyplot as plt
import numpy as np
import sys

np.set_printoptions(precision=6, suppress=True)

X_LABEL = 'x'
Y_LABEL = 'y'

def read_data(filename):
    with open(filename, 'r') as file:
        raw_data = file.readline()
    data = [point.replace('(', '').replace(')', '').split(';') for point in raw_data.split(', ')]
    return [{X_LABEL: float(point[0]), Y_LABEL: float(point[1])} for point in data if len(point) == 2]


if __name__ == '__main__':
    print(read_data(sys.argv[1]))