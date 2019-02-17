from numpy import*

def jacobian(f, x):
    h = 1.0e-4
    n = len(x)
    Jac = zeros([n,n])
    f0 = f(x)
    for i in arange(0,n,1):
        tt = x[i]
        x[i] = tt + h
        f1= f(x)
        x[i] = tt
        Jac [:,i] = (f1 - f0)/h
    return Jac, f0
def newton(f, x, tol=1.0e-9):
    iterMax = 500
    for i in range(iterMax):
        Jac, fO = jacobian(f, x)
        if sqrt(dot(fO, fO) / len(x)) < tol:
            return x, i                 
        dx = linalg.solve(Jac, fO)
        x = x - dx
    print ("Too many iterations for the Newton method")
n=100
def f(x):
    f = zeros([2])

    """
    https://old.math.tsu.ru/EEResources/cm/text/3_3.htm
   
    f[0] = 2 * x[1] - cos(x[0]+1)
    f[1] = x[0] + sin(x[1]) + 0.4
    """

    f[0] = power(x[0], 2) - x[1] + 1
    f[1] = x[0] - cos(pi/2 * x[1])
    return f
print(7.32595066e-24 - 2.85372006e-11)
x0 =zeros([2])
x, iter = newton(f, x0)
print ('Solution:\n', x)
print ('Newton iteration = ', iter)
