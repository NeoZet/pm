import matplotlib.pyplot as plt
import numpy as np
import math
import decimal

def taylor_sin(arg, number_of_elements):
    res = 0
    for i in range(number_of_elements+1):
        res += ((-1) ** i) * (arg ** (2 * i + 1)) / math.factorial(2 * i + 1)
    return res



# print(taylor_sin(np.deg2rad(90), 10))

number_of_functions = 10

ax = plt.subplot(111)


x = np.arange(-2 * np.pi, 2 * np.pi, 0.01)
y = np.sin(x)
plt.ylim(-2, 2)

cl = ['r', 'b', 'y']

for fn_num in range(number_of_functions):
    new_sin = []
    for arg in x:        
        new_sin.append(taylor_sin(arg, fn_num))
    plt.plot(x, new_sin, label="n=%d"%(arg,))
plt.plot(x, y, color='black', linewidth=2)
plt.grid(True)
plt.show()


