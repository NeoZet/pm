import sys

default_init_cond_filename = 'system.txt'

def read_system(filename):
    with open(filename, 'r') as data_file:
        order = int(data_file.readline())
        A = [[float(elem) for elem in row.split(',')] for row in [next(data_file) for line in range(order)]]
        b = [float(elem) for elem in next(data_file).split(',')]
    return [A, b]


def jacobi_method(matrix, extens):
    


def main():
    init_cond_filename = default_init_cond_filename
    if len(sys.argv) == 2:
        init_cond_filename = sys.argv[1]
    A, b = read_system(init_cond_filename)

if __name__ == '__main__':
    main()
