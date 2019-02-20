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


delta_1 = lambda system, xk: max([abs(func) for func in system(xk)])
delta_2 = lambda xk, xk_next: max([abs(xi_next - xi) for xi, xi_next in zip(xk, xk_next)])
get_exponent = lambda number: int(str(number).split('e')[1])
format_float = lambda number: '{:.10f}'.format(number)

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

def newton(f, x, eps_1, eps_2):
    iterMax = 500
    d1 = []
    d2 = []
        
    for i in range(iterMax):
        Jac, f0 = jacobian(f, x)
        dx = gauss(Jac, f0)
        if dx == 'e':
            print("Решений Нет!")
            return None
        
        x_prev = copy.deepcopy(x)
        x -= dx
        
        d1.append(delta_1(f, x))
        d2.append(delta_2(x_prev, x))        
        
        if d1[i] < eps_1 and d2[i] < eps_2:
            return {'solution': x, 'iteration': i, 'delta_1': d1, 'delta_2': d2}
        
    print("Слишком много итераций!")
    return None

NUMBER_OF_EQUATIONS = 2

def f(x):
    f = np.zeros([NUMBER_OF_EQUATIONS])

    """
    https://old.math.tsu.ru/EEResources/cm/text/3_3.htm
   
    f[0] = 2 * x[1] - np.cos(x[0]+1)
    f[1] = x[0] + np.sin(x[1]) + 0.4
    """   

    # f[0] = 2*x[0] - np.sin((x[0]-x[1])/2)
    # f[1] = 2*x[1] - np.cos((x[0]+x[1])/2)
    
    f[0] = np.power(x[0], 2) - x[1] + 1
    f[1] = x[0] - np.cos(np.pi/2 * x[1])
    return f

EPS_1 = 1.0e-9
EPS_2 = 1.0e-9

def main():
    x0 = np.zeros([NUMBER_OF_EQUATIONS])
    res = newton(f, x0, EPS_1, EPS_2)
    print('''
Iter   |      delta_1           |     delta_2
---------------------------------------------------''')
    for iter, d1, d2 in zip(range(res['iteration']), res['delta_1'], res['delta_2']):
        print('''
  {0}    |      {1}      |     {2} '''.format(iter, format_float(d1), format_float(d2)))
    print('---------------------------------------------------\n')

    print('Solution:')
    for i in range(len(res['solution'])):
        print('X{0} = {1}'.format(i, format_float(res['solution'][i])))

if __name__ == '__main__':
    main()
