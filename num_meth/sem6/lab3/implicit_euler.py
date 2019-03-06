import numpy as np
import copy

e_dop = 0.00001
u_0 = np.array([0, -0.412])
u_0_test = np.array([3, 0])
Om = 40
a = 2.5 + Om/40
tau_max = 0.01
T = 1

NUMBER_OF_EQUATIONS = 2

def step(f):
    tau = []
    for i in range(NUMBER_OF_EQUATIONS):
        tau.append(e_dop / np.abs(f[i]) + e_dop / tau_max)
    return min(tau)

def f(u, t):
    f = np.zeros([NUMBER_OF_EQUATIONS])
    f[0] = -u[0]*u[1] + np.sin(t) / t
    f[1] = -(u[1] ** 2) + a*t/(1+t**2)
    return f

def f_test(u, t):
    f = np.zeros([NUMBER_OF_EQUATIONS])
    f[0] = -2*u[0] + 4*u[1]
    f[1] = -u[0] + 3*u[1]
    return f

def euler():
    t_k = 0.00000001
    y_k = u_0
    k = 0
    while t_k < T:
        f_vec = f(y_k, t_k)
        tau_k = step(f_vec)
        y_k = y_k + tau_k * f_vec
        t_k += tau_k        
        print('K: [{2}] | Y: {0} | T: {1}'.format(y_k, t_k, k))
        k += 1


if __name__ == '__main__':
    euler()
