import matplotlib.pyplot as plt
import numpy as np

EPS = 0.001
MAX_ITER = 1000
F = 1

#magic numbers
T = 0.01
H = 0.01
s = 0.05

def func(x, F):
    return 6 - np.exp(-2 * x) + 2 * x - F

def beta(t, s):
    return 1/(t ** s)

def runge_kutta(equation, x0, F, eps):
    h = H
    t = T
    x = x0
    x_prev = x
    iter = 0
    
    while np.abs(func(x, F)) > eps:        
        while iter < MAX_ITER:
            iter += 1
            k1 = h/3 * equation(x, F)                                    * (-beta(t, s))
            k2 = h/3 * equation(x + k1, F)                               * (-beta(t+h/3, s))
            k3 = h/3 * equation(x + (k1/2) + (k2/2), F)                  * (-beta(t+h/3, s))
            k4 = h/3 * equation(x + (3 * k1/8)+(9 * k3/8), F)            * (-beta(t+h/2, s))
            k5 = h/3 * equation(x + (3 * k1/2) - (9 * k3/2) + 6 * k4, F) * (-beta(t+h, s))

            delta = np.abs(k1-(9/2)*k3+4*k4-(1/2)*k5)
            if delta < 5 * eps:
                break 
            h /= 2                
            
        if delta < 5/32 * eps:
            h *= 2        

        x += (1/2) * (k1 + 4*k4 + k5)
        t += h
    return x,iter

def main():
    start_values = np.array([0.1, 0.001, 1])
    x0 = 0.1
    
    eq_x_list = np.arange(-40, 40, 0.01)
    eq_y_list = np.array([func(x, 0) for x in eq_x_list])

    F_x_list = np.arange(-40, 40, 0.01)
    F_y_list = np.array([F for x in F_x_list])
    
    fig,ax = plt.subplots()
    ax.grid(True)
    ax.tick_params(axis='x', which='major', direction='inout',
                bottom=True, top=False, left=True, right=False, 
                color='b', labelcolor='black',
                labelbottom=True, labeltop=False, labelleft=True, labelright=False)

    plt.title("Runge-Kutta's method")
    plt.ylim(-25, 30)
    plt.xlim(-20, 20)

    print('########################################\n')
    print("Runge-Kutta's method\n")
    for x in start_values:
        print('Initial approximation: {0}'.format(x))
        runge_kutta_solution, iterations = runge_kutta(func, x, F, EPS)        
        print('Solution: {0:.5f} | Iterations: {1}'.format(runge_kutta_solution, iterations))
        print('Accuracy: {0}'.format(func(runge_kutta_solution, F)))
        print('-----------------------------------------------')
    print('\n########################################')
    plt.plot(eq_x_list, eq_y_list, linewidth=2, label="y = 6 - e^(-2x) + 2x")
    plt.plot(F_x_list, F_y_list, linewidth=2, color='red', label="F = -1")
    plt.plot(runge_kutta_solution,
             F,
             marker='o',
             markersize=4,
             color='black',
             label='solution'
    )
    plt.annotate("[{0:.5f}, {1}]".format(runge_kutta_solution, F), (runge_kutta_solution, F))
    plt.legend(loc='best')
    plt.savefig('Runge-Kutta.png', dpi=300)
    
if __name__ == '__main__':
    main()    


