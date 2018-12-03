import sys
from functools import reduce

default_init_cond_filename = 'system.txt'

add_vect = lambda x,y: list(map(lambda a,b: a+b, x,y))
sub_vect = lambda x,y: list(map(lambda a,b: a-b, x,y))
scal_mul_vect = lambda x,y: reduce(lambda a,b: a+b, map(lambda a,b: a*b, x,y))
add_matr = lambda x,y: list(map(add_vect, x, y))
sub_matr = lambda x,y: list(map(sub_vect, x, y))

#mul_matr = 

def read_system(filename):
    with open(filename, 'r') as data_file:
        order = int(data_file.readline())
        A = [[float(elem) for elem in row.split(',')] for row in [next(data_file) for line in range(order)]]
        b = [float(elem) for elem in next(data_file).split(',')]
        return [A, b]
    

def jacobi_method(matrix, extens):
    pass


def main():
    init_cond_filename = default_init_cond_filename
    if len(sys.argv) == 2:
        init_cond_filename = sys.argv[1]
    A, b = read_system(init_cond_filename)
    a = [[1,2], [2,3], [3,4]]
    b = [[4,5], [5,6], [6,7]]
    c = [1,2,3]
    d = [4,5,6]    

if __name__ == '__main__':
    main()
