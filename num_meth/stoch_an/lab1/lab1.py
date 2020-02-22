import matplotlib.pyplot as plt
import numpy as np

import argparse
import copy
import sys

np.set_printoptions(precision=3, suppress=True)

X_LABEL = 'x'
Y_LABEL = 'y'

groups_number = lambda data_number: int(np.around(1 + np.log2(data_number)))
step = lambda data: np.around((data.max() - data.min()) / groups_number(data.size), decimals=1)
interval = lambda data, rbound, lbound: data[np.where((data < rbound) & (data >= lbound))]

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


# Numerical characteristics

def mean(data):
    return sum(data) / len(data)

def mode(data):
    return max(data, key=data.count)

def median(data):
    data_copy = copy.deepcopy(data)
    size = len(data_copy) 
    data_copy.sort()     
    
    if size % 2 == 0: 
        median1 = data_copy[size // 2] 
        median2 = data_copy[size // 2 + 1] 
        median = (median1 + median2) / 2
    else: 
        median = data_copy[size // 2] 
    return median


def dispers(data):
    dispers = 0
    aver = mean(data)
    for elem in data:
        dispers += ((elem - aver) ** 2) / len(data)
    return dispers

def standard_deviation(data):
    return dispers(data) ** 0.5

def excess(data):
    m4 = 0
    aver = mean(data)
    stad_dev = standard_deviation(data)
    for elem in data:
        m4 += ((elem - aver) ** 4.0) * data.count(elem) / len(data)
    return (m4 / stad_dev ** 4.0) - 3.0

def assimetry(data):
    m3 = 0
    aver = mean(data)
    stad_dev = standard_deviation(data)
    for elem in data:
        m3 += ((elem - aver) ** 3.0) * data.count(elem) / len(data)
    return (m3 / stad_dev ** 3.0)

def variation_coef(data):
    return standard_deviation(data) / mean(data) * 100


def frequencies(data):
    _step = step(data)
    lbound = data.min()
    rbound = lbound + _step    
    grp_num = groups_number(data.size)
    freqs = []
    for i in range(1, grp_num+1):
        freqs.append(interval(data, rbound, lbound).size)
        rbound += _step
        lbound += _step
    return dict(zip([i for i in range(1, grp_num+1)], freqs))

def intervals_medians(data):
    _step = step(data)
    lbound = data.min()
    rbound = lbound + _step    
    grp_num = groups_number(data.size)
    centers = []
    for i in range(1, grp_num+1):
        intr = interval(data, rbound, lbound)
        centers.append(np.median(intr) if intr.size != 0 else lbound)
        rbound += _step
        lbound += _step       
    return dict(zip([i for i in range(1, grp_num+1)], centers))

def histogram(data):
    _step = step(data)
    lbound = data.min()
    rbound = lbound + _step    
    grp_num = groups_number(data.size)
    widths = []
    bounds = []
    for i in range(1, grp_num+1):
        intr = interval(data, rbound, lbound)
        widths.append(intr.min() if intr.size != 0 else 0)
        bounds.append("{0:.5f}-{1:.5f}".format(lbound, rbound))
        rbound += _step
        lbound += _step
    freqs = np.array(list(frequencies(data).values())) / 200
    plt.bar(widths, freqs, label="Histogram", align='edge', width=_step, zorder=3, color='orange', edgecolor='black')    
   # plt.title('Histogram\n[Groups number: {0}; Split step: {1}]'.format(groups_number(data.size), step(data)))    
    plt.ylabel('Frequencies')
    plt.xlabel('Intervals')
    ax = plt.subplot(111)    
    ax.set_xticks(list(intervals_medians(data).values()))
    ax.set_xticklabels(bounds,rotation=40)

def polygon(data):
    freqs = np.array(list(frequencies(data).values())) / 200
    centers = intervals_medians(data).values()
    plt.xticks(list(centers))
    plt.yticks(np.arange(np.min(list(freqs)), np.max(list(freqs))+1, step=50))
    plt.plot(centers, freqs, marker='o', label='Polygon', zorder=4)
    # plt.title('Polygon\n[Groups number: {0}; Split step: {1}]'.format(groups_number(data.size), step(data)))
    plt.ylabel('Frequencies')
    plt.xlabel('Medians')
    plt.grid(zorder=0)
    for x,y in zip(centers, freqs):
        plt.annotate(str(y), (x, y))

def empirical_distr_func(data):
    func = lambda x: (data[np.where(data < x)].size / data.size)
    F = []
    grp_num = groups_number(data.size)
    _step = step(data)
    intervals = np.array([i for i in np.arange(data.min()-_step, data.max()+_step, _step)])
    for i in intervals:
        F.append(func(i))

    #intreval case
    #plt.plot(intervals, F, marker='o', label='Empirical distribution function')

    #discret case
    for i in range(intervals.size):        
        plt.arrow(intervals[i]+_step, F[i], -_step, 0, head_width=0.03, width=0.005, color='orange', length_includes_head=True, zorder=4)
        plt.plot((intervals[i], intervals[i]), (np.min(F)-_step*2, F[i]), linestyle='dashed', color='black', linewidth=1)
        plt.plot((np.min(intervals)-_step*2, intervals[i]), (F[i], F[i]), linestyle='dashed', color='black', linewidth=1)
        plt.axis(xmin=np.min(intervals)-_step,ymin=np.min(F)-_step)

    plt.xticks(np.arange(intervals.min() - _step, intervals.max() + _step, step=_step))
    plt.yticks(np.arange(0, 1.1, step=_step/3))
    plt.ylim(-0.1,1.1)
    plt.grid(zorder=0)
    plt.ylabel('F*(x)')
    plt.xlabel('x')
    plt.title("Empirical distribution function")
    for x,y in zip(intervals, F):
        plt.annotate(str(y), (x-0.2, y+0.02), zorder=5, )#, xytext=(x+_step,y), arrowprops=dict(color='green', arrowstyle='->', lw=3.5), zorder=5)

if __name__ == '__main__':    
    parser = create_parser()
    args = parser.parse_args()
    if args.one_dimensional:
        data_1d = read_data_1d(args.one_dimensional)
        print('Mean:               {0}'.format(mean(data_1d)))
        print('Mode:               {0}'.format(mode(data_1d)))
        print('Median:             {0}'.format(median(data_1d)))
        print('Dispers:            {0}'.format(dispers(data_1d)))
        print('Standard deviation: {0}'.format(standard_deviation(data_1d)))
        print('Excess:             {0}'.format(excess(data_1d)))
        print('Assimetry:          {0}'.format(assimetry(data_1d)))
        print('Variation coef:     {0} %'.format(variation_coef(data_1d)))
        
        #histogram(np.array(data_1d))
        #polygon(np.array(data_1d))
        empirical_distr_func(np.array(data_1d))
    if args.two_dimensional:
        data_2d = read_data_2d(args.two_dimensional)
        print(data_2d[0])

    plt.legend()
    plt.show()