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
    D = [[system[i][j] if i == j else 0 for i in range(len(system))] for j in range(len(system))]
    invers_D = [[1/D[i][i] if i == j else 0 for i in range(len(D))] for j in range(len(D))]
    E = unit_matr(len(system))
    C = sub_matr(E, mulmatr(invers_D, system))
    b = mulmatr(invers_D, trans_matr([extens]))
    x_k = [[1] for i in range(len(system))]
    norm = 1
    while norm > eps:        
        x_k_prev = x_k
        x_k = add_matr(mulmatr(C, x_k), b)
        norm = max(map(abs, sub_matr(trans_matr(x_k_prev), trans_matr(x_k))[0]))
    return x_k

#def gauss_zeidel_meth(system, extens, eps):
import numpy as np

def gauss_seidel(a, b, errorfactor = 0.01):
    a = np.array(a)
    b = np.array(b)
    l = np.zeros((len(a), len(a[0])))
    for i in range(len(a)):
        for j in range(i+1):
            l[i][j] = float(a[i][j]) # Get the lower matrix of a

    x0 = [0.0] * len(a[0])
    xprev = np.array(x0)
    prevnorm = None
    while True:
        z = np.inner(a, xprev)
        r = b - z
        rnorm = np.linalg.norm(r)
        if prevnorm is None:
            prevnorm = rnorm
        elif prevnorm < rnorm: # It's going far away
            return xprev
        ebnorm = errorfactor * np.linalg.norm(b)
        if ebnorm > rnorm:
            return xprev 
        for i in range(len(a)): # Forward substitution
            cum = 0.0
            for j in range(i):
                cum += float(l[i][j] * z[j])
            z[i] = (r[i] - cum)/float(l[i][i])
        xprev += z    
    
def main():
    init_cond_filename = default_init_cond_filename
    if len(sys.argv) == 2:
        init_cond_filename = sys.argv[1]
    A, b = read_system(init_cond_filename)    
    print(jacobi_method(A, b, 0.0001))
    print(gauss_seidel(A, b, 0.0001))
    
if __name__ == '__main__':
    main()

