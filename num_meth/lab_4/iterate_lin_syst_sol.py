import sys
import numpy as np
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
    rows_A = len(A)
    cols_A = len(A[0])
    rows_B = len(B)
    cols_B = len(B[0])
    
    C = [[0 for row in range(cols_B)] for col in range(rows_A)]
    for i in range(rows_A):
        for j in range(cols_B):
            for k in range(cols_A):
                C[i][j] += A[i][k] * B[k][j]
    return C

def read_system(filename):
    with open(filename, 'r') as data_file:
        order = int(data_file.readline())
        A = [[float(elem) for elem in row.split(',')] for row in [next(data_file)
                                                                  for line in range(order)]]
        b = [float(elem) for elem in next(data_file).split(',')]
        return [A, b]
    

def jacobi_method(system, extens, eps):
    iter = 0
    D = [[system[i][j] if i == j else 0 for i in range(len(system))] for j in range(len(system))]
    invers_D = [[1/D[i][i] if i == j else 0 for i in range(len(D))] for j in range(len(D))]
    E = unit_matr(len(system))
    C = sub_matr(E, mulmatr(invers_D, system))
    b = mulmatr(invers_D, trans_matr([extens]))
    solve = [[1] for i in range(len(system))]
    norm = 1+eps
    while norm > eps:        
        solve_prev = solve
        solve = add_matr(mulmatr(C, solve), b)
        norm = max(map(abs, sub_matr(trans_matr(solve_prev), trans_matr(solve))[0]))
        iter += 1
    return [list(*zip(*solve)), iter]


def gauss_seidel_method(system, extens, eps):
    norm = 1+eps
    iter = 0
    solve_prev = [1 for i in range(len(system))]
    while norm > eps:
        solve = [0 for i in range(len(system))]
        for i in range(len(system)):
            solve[i] = (-sum([system[i][j] * solve[j] for j in range(i)])
                        -sum([system[i][j] * solve_prev[j] for j in range(i+1, len(system))])
                        + extens[i]) / system[i][i]
        norm = (max(map(abs, sub_vect(solve, solve_prev))) /
                max(map(abs, solve)))
        solve_prev = solve
        iter += 1
    return [solve, iter]

def resid(A, b, solve):
    F = []
    for i in range (len(A)):
        res = 0
        for j in range (len(solve)):
            res += A[i][j] * solve[j]
        F.append(res - b[i])
    return F


def main():
    init_cond_filename = default_init_cond_filename
    if len(sys.argv) == 2:
        init_cond_filename = sys.argv[1]
    A, b = read_system(init_cond_filename)

    solve, iter_num = jacobi_method(A, b, 0.0001)
    print("Jacobi\n********")
    print("Iterations: {0}".format(iter_num))
    [print('X{0} = {1}'.format(i, solve[i])) for i in range(len(solve))]
    residial = resid(A, b, solve)
    print('resid = ({0})'.format(residial))
    norm = max([abs(residial[i]) for i in range(len(residial))])
    print('resid norm = {0}'.format(norm))
    
    solve, iter_num = gauss_seidel_method(A, b, 0.0001)
    print("\nGauss-Seidel\n********")
    print("Iterations: {0}".format(iter_num))
    [print('X{0} = {1}'.format(i, solve[i])) for i in range(len(solve))]
    residial = resid(A, b, solve)
    print('resid = ({0})'.format(residial))
    norm = max([abs(residial[i]) for i in range(len(residial))])
    print('resid norm = {0}'.format(norm))
    
if __name__ == '__main__':
    main()

