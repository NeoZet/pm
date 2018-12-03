import sys
from functools import reduce

default_init_cond_filename = 'system.txt'

add_vect = lambda x,y: list(map(lambda a,b: a+b, x,y))
sub_vect = lambda x,y: list(map(lambda a,b: a-b, x,y))
scal_mul_vect = lambda x,y: reduce(lambda a,b: a+b, map(lambda a,b: a*b, x,y))
add_matr = lambda x,y: list(map(add_vect, x, y))
sub_matr = lambda x,y: list(map(sub_vect, x, y))
trans_matr = lambda x: [*zip(*x)]
unit_matr = lambda ord: [[1 if i == j else 0 for i in range(ord)] for j in range(ord)]

def mulmatr(A, B):
    Bt = trans_matr(B)
    return [[sum(elem_a*elem_b for elem_a, elem_b in zip(row_a, col_b)) 
             for col_b in Bt] for row_a in A]

def read_system(filename):
    with open(filename, 'r') as data_file:
        order = int(data_file.readline())
        A = [[float(elem) for elem in row.split(',')] for row in [next(data_file)
                                                                  for line in range(order)]]
        b = [float(elem) for elem in next(data_file).split(',')]
        return [A, b]
    

def jacobi_method(system, extens, eps):
    D = [system[i][i] for i in range(len(system))]
    invers_D = [1/D[i][i] for i in range(len(D))]
    E = unit_matr(len(system))
    C = sub_matr(E, mulmatr(invers_D, system))
    b = mulmatr(invers_D, extens)
    x_k = [[1] for i in range(len(system))]
    it = 100
    while it:#norm > eps:
        x_k = add_matr(mulmatr(C, x_k), b)
        it -= 1
    printf(x_k)
    
    


def main():
    init_cond_filename = default_init_cond_filename
    if len(sys.argv) == 2:
        init_cond_filename = sys.argv[1]
    A, b = read_system(init_cond_filename)
    a = [[1,2,3], [2,3,4], [3,4,5]]
    b = [[4,5,6], [5,6,7], [6,7,8]]
    c = [1,2,3]
    d = [4,5,6]
    print([a[i][i] for i in range(len(a))])
    #print([1/D[i][i] for i in range(len(D))])
#    jacobi_method(A, b, 0.001)
    
if __name__ == '__main__':
    main()
