import numpy as np
import copy

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


delta_1 = lambda system, f, xk, t, y_k, tau: max([abs(func) for func in system(f, y_k, xk, t, tau)])
delta_2 = lambda xk, xk_next: max([abs(xi_next - xi) for xi, xi_next in zip(xk, xk_next)])
get_exponent = lambda number: int(str(number).split('e')[1])
format_float = lambda number: '{:.10f}'.format(number)

def jacobian(f_tmp, f, x, t, y_k, tau):
    h = 1.0e-4
    n = len(x)
    jacobian = np.zeros([n,n])
    f0 = f_tmp(f, y_k, x, t, tau)
    for i in np.arange(0,n,1):
        tmp = x[i]
        x[i] = tmp + h
        f_x = f_tmp(f, y_k, x, t, tau)
        x[i] = tmp
        jacobian[:,i] = (f_x - f0)/h
    return jacobian, f0

def newton(f_tmp, f, x, t, y_k, tau, eps_1=0.00001, eps_2=0.00001):
    iterMax = 500
    d1 = []
    d2 = []
        
    for i in range(iterMax):
        Jac, f0 = jacobian(f_tmp, f, x, t, y_k, tau)
        dx = gauss(Jac, f0)
        if dx == 'e':
            print("Решений Нет!")
            return None
        
        x_prev = copy.deepcopy(x)
        x -= dx

        d1.append(delta_1(f_tmp, f, x, t, y_k, tau))
        d2.append(delta_2(x_prev, x))        

        if d1[i] < eps_1 and d2[i] < eps_2:
            return x
        
    print("Слишком много итераций!")
    return None

u_0 = np.array([0.000001, -0.412])
_u_0 = np.array([3, 0.00000001])
Om = 40
a = 2.5 + Om/40
tau_max = 0.1
tau_min = 0.01
T = 1

NUMBER_OF_EQUATIONS = 2


def f(u, t):
    f = np.zeros([NUMBER_OF_EQUATIONS])
    f[0] = -u[0]*u[1] + np.sin(t) / t
    f[1] = -(u[1] ** 2) + a*t/(1+t**2)
    return f

def _f(u, t):
    f = np.zeros([NUMBER_OF_EQUATIONS])
    f[0] = -2*u[0] + 4*u[1]
    f[1] = -u[0] + 3*u[1]
    return f

def f_temp(f, y_k, y_k_next, t, tau):
    F = np.zeros([NUMBER_OF_EQUATIONS])
    for k in range(NUMBER_OF_EQUATIONS):
        F = y_k_next - y_k - tau * f(y_k_next, t)
    return F

def euler():
    eps_dop = 0.001
    t_k = 0.00000001
    y_k = copy.deepcopy(u_0)
    y_k_prev = copy.deepcopy(u_0)
    y_k_next = copy.deepcopy(u_0)
    tau_k_prev = tau_min
    tau_k = tau_min
    tau_k_next = tau_k
    k = 0
    while t_k < T:
        t_k_next = t_k + tau_k
        y_k_next = newton(f_temp, f, y_k_next, t_k_next, y_k, tau_k)        

        eps_k = np.zeros([NUMBER_OF_EQUATIONS])
        for i in range(NUMBER_OF_EQUATIONS):
            eps_k[i] = -tau_k/(tau_k + tau_k_prev)*(y_k_next[i] - y_k[i] - tau_k/tau_k_prev * (y_k[i] - y_k_prev[i]))

        greater = False
        for i in range(NUMBER_OF_EQUATIONS):
            if np.abs(eps_k[i]) > eps_dop:
                tau_k /= 2
                t_k_next = t_k
                y_k_next = copy.deepcopy(y_k)
                greater = True
                break
        if greater == True:
            continue

        tau_k_i = np.zeros([NUMBER_OF_EQUATIONS])
        for i in range(NUMBER_OF_EQUATIONS):
            tau_k_i[i] = eps_dop/(np.abs(y_k_next[i]) + eps_dop / tau_max)

        tau_k_next = min(tau_k_i)
        
        if tau_k_next > tau_max:
            tau_k_next = tau_max
        print('K: [{2}] | Y: {0} | T: {1}'.format(y_k_next, t_k_next, k))
        y_k_prev = copy.deepcopy(y_k)
        y_k = copy.deepcopy(y_k_next)
        tau_k_prev = tau_k
        tau_k = tau_k_next
        t_k = t_k_next
        k += 1
    return y_k_next
    

if __name__ == '__main__':
    euler()
