import numpy as np
import sys
import matplotlib.pyplot as plt


def read_data(filename):
    data_dict = {}
    try:
        with open(filename, 'r') as file:
            raw_data = file.readlines()
        data_dict['result_attribute'] = [float(point) for point in raw_data[0].split(',')]
        for line,i in zip(raw_data[1:], range(len(raw_data[1:]))):
            data_dict[i] = [float(point) for point in line.split(',')]
        print(data_dict)
        return data_dict

    except FileNotFoundError as ex:
        print(ex)
        return None

def mean(data):
    return sum(data) / len(data)

def dispers(data):
    dispers = 0
    aver = mean(data)
    for elem in data:
        dispers += ((elem - aver) ** 2) / len(data)
    return dispers

def arrays_multiplication_mean(arr_1, arr_2):
    mean = 0
    for i in range(len(arr_1)):
        mean += arr_1[i] * arr_2[i]
    return (mean / len(arr_1))

def standard_deviation(data):
    return dispers(data) ** 0.5

def list_squares_sum(data):
    sum = 0
    for i in data:
        sum += i ** 2
    return sum

def lists_multiplication_sum(list_1, list_2):
    sum = 0
    for i in range(len(list_1)):
        sum += list_1[i] * list_2[i]
    return sum

def significance(Y, X, a, b):
    aver_x = mean(X)
    n = len(data)
    Sx = 0
    S = 0
    for i in range(n):
        S += (Y[i] - regr_func(a, b, X[i]))**2
        Sx += (X[i] - aver_x)**2
    S = (S/(n-2)) ** (0.5)
    Sx = (Sx/(n-1)) ** (0.5)
    Sa = S/(Sx*(n-1)**0.5)
    Sb = S * (((1/n) + (aver_x**2)/((n-1) * Sx**2)) ** (0.5))

    tst095 = 2.0345
    print('-----------------')
    if abs(a) > tst095 * Sa:
        print("a = ", a, "- значимый коэффициент")
    if abs(b) > tst095 * Sb:
        print("b =", b, " - значимый коэффициент") 

def regr_func(a, b, x):
    return float(a)*float(x) + float(b)

def regressModel(Y,X):
    n = len(Y)
    xSum = 0.0
    ySum = 0.0
    x2Sum = 0.0
    xySum = 0.0
    for i in range(n):
        xSum += X[i]
        ySum += Y[i]
        x2Sum += X[i]**2
        xySum += X[i]*Y[i]

    A = np.array([[n, xSum], [xSum, x2Sum]])
    B = np.array([ySum, xySum])
    res = np.linalg.solve(A, B)
    print("a = ", res[1])
    print("b = ", res[0])

    significance(Y, X, res[1], res[0])

    data = sorted(X)
    dataYLinReg = []
    dataX = []
    dataY = []
    for i in range(n):
        dataYLinReg.append(regr_func(res[1], res[0], float(X[i])))
        dataX.append(float(X[i]))
        dataY.append(float(Y[i]))

    plt.scatter(dataX, dataY, color='orange')
    plt.plot(dataX, dataYLinReg)
    plt.ylabel('yi')
    plt.xlabel('xi')


def pair_correl_coef(arr_1, arr_2):
    return (arrays_multiplication_mean(arr_1, arr_2) - mean(arr_1) * mean(arr_2)) / (standard_deviation(arr_1) * standard_deviation(arr_2))

if __name__ == "__main__":
    data = read_data(sys.argv[1])
    for attribute, array in data.items():
        if not 'result_attribute' in str(attribute):
            regressModel(data['result_attribute'], array)
            plt.title(attribute)
            plt.show()
    sys.exit(0)

X6 = [ 0.4, 0.26, 0.4, 0.5, 0.4, 0.19, 0.25, 0.44, 0.17, 0.39, 0.33, 0.25, 0.32, 0.02, 0.06, 0.15, 0.08, 0.2, 0.2, 0.3, 0.24, 0.1, 0.11, 0.47, 0.53, 0.34, 0.2, 0.24, 0.54, 0.4, 0.2, 0.64, 0.42, 0.27, 0.37, 0.38, 0.35, 0.42, 0.32, 0.33, 0.29, 0.3, 0.56, 0.42, 0.26, 0.16, 0.45, 0.31, 0.08, 0.68, 0.03, 0.02, 0.22]
X8 = [1.23, 1.04, 1.8, 0.43, 0.88, 0.57, 1.72, 1.7, 0.84, 0.6, 0.82, 0.84, 0.67, 1.04, 0.66, 0.86, 0.79, 0.34, 1.6, 1.46, 1.27, 1.58, 0.68, 0.86, 1.98, 0.33, 0.45, 0.74, 0.03, 0.99, 0.24, 0.57, 1.22, 0.68, 1, 0.81, 1.27, 1.14, 1.89, 0.67, 0.96, 0.67, 0.98, 1.16, 0.54, 1.23, 0.78, 1.16, 4.44, 1.06, 2.13, 1.21, 2.2]
X11 = [26006,23935, 22589, 21220, 7394, 11586, 26609, 7801, 11587, 9475, 10811, 6371, 26761, 4210, 3557, 14148, 9872, 5975, 16662, 9166, 15118, 11429, 6462, 24628, 49727, 11470, 19448, 18963, 9185, 17478, 6265, 8810, 17659, 10342, 8901, 8402, 32625, 31160, 46461, 13833, 6391, 11115, 6555, 11085, 9484, 3967, 15283, 20874, 19418, 3351, 6338, 9756, 11705]
X12 = [167.69, 186.1, 220.45, 169.3, 39.53, 40.41, 102.96, 37.02, 45.74, 40.07, 45.44, 41.08, 136.14, 42.39, 37.39, 101.78, 47.55, 32.61, 103.25, 38.95, 81.32, 67.26, 59.92, 107.34, 512.6, 53.81, 80.83, 59.42, 36.96, 91.43, 17.16, 27.29, 184.33, 58.42, 59.4, 49.63, 391.27, 258.62, 75.66, 123.68, 37.21, 53.37, 32.87, 45.63, 48.41, 13.58, 63.99, 104.55, 222.11, 25.76, 29.52, 41.99, 78.11]
X17 = [17.72, 18.39, 26.46, 22.37, 28.13, 17.55, 21.92, 19.52, 23.99, 21.76, 25.68, 18.13, 25.74, 21.21, 22.97, 16.38, 13.21, 14.48, 13.38, 13.69, 16.66, 15.06, 20.09, 15.98, 18.27, 14.42, 22.76, 15.41, 19.35, 16.83, 30.53, 17.98, 22.09, 18.29, 26.05, 26.2, 17.26, 18.83, 19.7, 16.87, 14.63, 22.17, 22.62, 26.44, 22.26, 19.13, 18.28, 28.23, 12.39, 11.64, 8.62, 20.1, 19.41]
Y1 = [9.26, 9.38, 12.11, 10.81, 9.35, 9.87, 8.17, 9.12, 5.88, 6.3, 6.2, 5.49, 6.5, 6.61, 4.32, 7.37, 7.02, 8.25, 8.15, 8.72, 6.64, 8.1, 5.52, 9.37, 13.17, 6.67, 6.68, 5.22, 10.02, 8.16, 3.78, 6.48, 10.44, 7.65, 8.77, 7, 11.06, 9.02, 13.28, 9.27, 6.7, 6.69, 9.42, 7.24, 5.39, 5.61, 5.59, 6.57, 6.54, 4.23, 5.22, 18, 11.03]

k=5 #количество факторов
n = len(X11)
X11cp = mean(X11)
X12cp = mean(X12)
X6cp = mean(X6)
X8cp = mean(X8)
X17cp = mean(X17)
Y1cp = mean(Y1)
#Среднеквадратическое отклонение
SX11 = standard_deviation(X11)
SX12 = standard_deviation(X12)
SX6  = standard_deviation(X6)
SX8  = standard_deviation(X8)
SX17 = standard_deviation(X17)
SY1  = standard_deviation(Y1)
#Cpеднее произведений
X11X12cp = arrays_multiplication_mean(X11, X12)
X11X6cp = arrays_multiplication_mean(X11, X6)
X11X8cp = arrays_multiplication_mean(X11, X8)
X11X17cp = arrays_multiplication_mean(X11, X17)
X12X6cp = arrays_multiplication_mean(X12, X6)
X12X8cp = arrays_multiplication_mean(X12, X8)
X12X17cp = arrays_multiplication_mean(X12, X17)
X6X8cp = arrays_multiplication_mean(X6, X8)
X6X17cp = arrays_multiplication_mean(X6, X17)
X8X17cp = arrays_multiplication_mean(X8, X17)
Y1X11cp = arrays_multiplication_mean(Y1, X11)
Y1X12cp = arrays_multiplication_mean(Y1, X12)
Y1X6cp = arrays_multiplication_mean(Y1, X6)
Y1X8cp = arrays_multiplication_mean(Y1, X8)
Y1X17cp = arrays_multiplication_mean(Y1, X17)
#коэффициенты парной корреляции
rX11X12 = (X11X12cp - X11cp*X12cp)/(SX11*SX12) 
rX11X6 = (X11X6cp - X11cp*X6cp)/(SX11*SX6) 
rX11X8 = (X11X8cp - X11cp*X8cp)/(SX11*SX8) 
rX11X17 = (X11X17cp - X11cp*X17cp)/(SX11*SX17) 
rX12X6 = (X12X6cp - X12cp*X6cp)/(SX12*SX6) 
rX12X8 = (X12X8cp - X12cp*X8cp)/(SX12*SX8)
rX12X17 = (X12X17cp - X12cp*X17cp)/(SX12*SX17)
rX6X8 = (X6X8cp - X6cp*X8cp)/(SX6*SX8)
rX6X17 = (X6X17cp - X6cp*X17cp)/(SX6*SX17)
rX8X17 = (X8X17cp - X8cp*X17cp)/(SX8*SX17)
rY1X11 = (Y1X11cp - Y1cp*X11cp)/(SY1*SX11) 
rY1X12 = (Y1X12cp - Y1cp*X12cp)/(SY1*SX12) 
rY1X6 = (Y1X6cp - Y1cp*X6cp)/(SY1*SX6) 
rY1X8 = (Y1X8cp - Y1cp*X8cp)/(SY1*SX8) 
rY1X17 = (Y1X17cp - Y1cp*X17cp)/(SY1*SX17)  


tnYX11 = ((n - 2)* rY1X11**2 / (1 - rY1X11**2))**(0.5)
tnYX12 = ((n - 2)* rY1X12**2 / (1 - rY1X12**2))**(0.5)
tnYX17 = ((n - 2)* rY1X17**2 / (1 - rY1X17**2))**(0.5)
tnYX8 = ((n - 2)* rY1X8**2 / (1 - rY1X8**2))**(0.5)
tnYX6 = ((n - 2)* rY1X6**2 / (1 - rY1X6**2))**(0.5)
tkr = 2.007
print("Для tкрит = ", tkr, "значимыми являются X11 (tнабл =", tnYX11, "), X12 (tнабл =", tnYX12, "), X8 (tнабл =", tnYX8, ")")
print("Остальные незначимы: tнабл = ", tnYX17, tnYX6)

print(rY1X8, rY1X11, rY1X12)

#A = numpy.array([[sum(X11), sum(X12), n], 
#    [lists_multiplication_sum(X11, X12), list_squares_sum(X12), sum(X12)],
#    [list_squares_sum(X11), lists_multiplication_sum(X11, X12), sum(X11)]])
#B = numpy.array([sum(Y1), lists_multiplication_sum(Y1, X12), lists_multiplication_sum(Y1, X11)])
#res = numpy.linalg.solve(A, B)
#print("y = ", res[0], "x11 + ", res[1], "x12 + ", res[2])

RYX11X12 = ((rY1X11**2 + rY1X12**2 - 2*rY1X11*rY1X12*rX11X12)/(1-rX11X12**2))**(0.5)

# Проверка адекватности модели множественной линейной корреляции
# 2 - число факторных признаков.
Fnabl = ((n-2-1)*(RYX11X12**2))/(2*(1-RYX11X12**2))
Fkrit = 3.19 # 0.05 и 50
print("Fнабл =", Fnabl, "Fкрит =", Fkrit)
if Fnabl >= Fkrit:
    print('с вероятностью 0,95 гипотеза о статистической значимости эмпирических данных принимается, корреляционная модель может быть построена. \n')
else:
    print('гипотеза отвергается\n')
