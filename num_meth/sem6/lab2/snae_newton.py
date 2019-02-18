import numpy as np
import copy 

def gauss(A, b):
    def _prepare_main_elem():
        for i in range(len(b)):
            for j in range(len(b)):
                if(i != j and A[i][i] == 0 and A[j][i] != 0 and A[i][j] != 0):                    
                    for k in range(len(b)):
                        A[j][k], A[i][k] = A[i][k], A[j][k]
                    b[j],b[i] = b[i],b[j]
                    break

    _prepare_main_elem()
    for i in range(len(b)):
        if A[i][i] == 0:
            return 'e'
        for j in range(i+1, len(b)):
            tmp = A[j][i] / A[i][i]
            for t in range(i, len(b)):
                A[j][t] -= tmp * A[i][t]
            b[j] -= tmp * b[i]
    solve = [0 for i in range(len(b))]
    for i in reversed(range(0, len(b))):
        sum_known_elems = 0
        for j in range(i, len(b)):
            sum_known_elems += A[i][j] * solve[j]
        solve[i] = (b[i] - sum_known_elems) / A[i][i]
    return solve;

def resid(A, b, solve):
    F = []
    for i in range (len(A)):
        res = 0
        for j in range (len(solve)):
            res += A[i][j] * solve[j]
        F.append(res - b[i])
    return F

def jacobian(f, x):
    h = 1.0e-4
    n = len(x)
    jacobian = np.zeros([n,n])
    f0 = f(x)
    for i in np.arange(0,n,1):
        tmp = x[i]
        x[i] = tmp + h
        f_x = f(x)
        x[i] = tmp
        jacobian[:,i] = (f_x - f0)/h
    return jacobian, f0

def newton(f, x, tol=1.0e-9):
    iterMax = 500
    for i in range(iterMax):
        Jac, f0 = jacobian(f, x)
        if np.sqrt(np.dot(f0, f0) / len(x)) < tol:
            return x, i                 
        dx = gauss(Jac, f0)
        if dx == 'e':
            print("Решений Нет!")
            exit(1)
        x = x - dx
    print ("Слишком много итераций!")

def f(x):
    f = np.zeros([2])

    """
    https://old.math.tsu.ru/EEResources/cm/text/3_3.htm
 """  
    f[0] = 2 * x[1] - np.cos(x[0]+1)
    f[1] = x[0] + np.sin(x[1]) + 0.4
#    """

    # f[0] = power(x[0], 2) - x[1] + 1
    # f[1] = x[0] - cos(pi/2 * x[1])
    return f

x0 = np.zeros([2])
x, iter = newton(f, x0)
print ('Solution:\n', x)
print ('Newton iteration = ', iter)
