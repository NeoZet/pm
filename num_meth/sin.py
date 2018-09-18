import matplotlib.pyplot as plt
import numpy as np
import math
import decimal


FUNCTIONS = {1: "x", 2: "x"}
NUMBER_OF_FUNCTIONS = 10

def taylor_sin(arg, number_of_elements):
    res = 0
    for i in range(number_of_elements+1):
        res += ((-1) ** i) * (arg ** (2 * i + 1)) / math.factorial(2 * i + 1)
    return res

ax = plt.subplot()

x = np.arange(-2 * np.pi, 2 * np.pi, 0.01)
y = np.sin(x)
plt.ylim(-2, 2)

for fn_num in range(NUMBER_OF_FUNCTIONS):
    new_sin = []
    for arg in x:        
        new_sin.append(taylor_sin(arg, fn_num))
    plt.plot(x, new_sin)
    
plt.plot(x, y, color='black', linewidth=2)
plt.grid(True)

plt.show()


