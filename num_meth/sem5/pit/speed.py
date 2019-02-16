import matplotlib.pylab as plt
import matplotlib.animation as animation
import array as arr
import numpy as np
import math
n=0

def _read_coords(filename):
    with open(filename, "r") as coords_file:
        return [[float(elem) for elem in coord.split(',')] for coord in coords_file.readlines()]

coords = _read_coords("graph_data.csv")
x = [coord[0] for coord in coords]
y = [coord[1] for coord in coords]

print(x)
print(y)
fig=plt.figure()
speed_ball = []
x = []
x_ball=[]
y_ball=[]
delta_x=0.1
delta_t=0.1
resistance=0.07
g=10
for n in range(len(y)):# create an array X with step = 2
 x.append(int (2*n))

temp_x= float(input("Введите координату х: "))
if(temp_x<=0 or temp_x>=287):
 print("Ошибка!!! Выход за пределы графика!!!")
 exit(0)


def find_y(temp_x):
 x0=int (round(temp_x-0.5))#Looking for X 
 if x0%2!=0:               # followed
  x0=x0-1                 # by temp_x
 if x0<2*(len(x)-1) and x0>=0: #check whether the entered value
  y0=y[x0//2]                   # is within the range
  y1=y[x0//2+1]
  return (float (y0-(y0-y1)*(temp_x-x0)/2))# calculate Y
temp_y=find_y(temp_x)
energy=g*find_y(temp_x)

def move1(now_x, now_speed):
  now_y=find_y(now_x)
  a=g*(find_y(now_x)-find_y(now_x+delta_x))/pow(pow(find_y(now_x)-find_y(now_x+delta_x),2)+pow(delta_x,2),0.5)
  speed=now_speed+a*delta_t
  s=speed*delta_t
  next_x=now_x+s/pow(((pow((find_y(now_x+delta_x)-find_y(now_x))/delta_x,2))+1),0.5)
  speed-=s*resistance
  next_y=find_y(next_x)
  x_ball.append(next_x)
  y_ball.append(next_y)
  speed_ball.append(speed)
  return(next_x,speed)


def init():
 redDot.set_data([],[])
 return redDot,
i=0
def animate(i):
    #if(i>=len(x_ball)):
    # exit(0)
    redDot.set_data(x_ball[i], y_ball[i])
    return redDot,

def plot_ball():
   now_x=temp_x
   speed=0
   speed_ball.append(speed)
   x_ball.append(temp_x)
   y_ball.append(temp_y)
   while(abs(speed)>=2 or (find_y(now_x+delta_x)-find_y(now_x))/delta_x!=0):
    (now_x,speed)=move1(now_x,speed)

plot_ball()



pit=plt.grid()

plt.xlabel('x[mm]')
plt.ylabel('speed[mm/s]')
plt.title ('Скорость шарика')


plt.plot(x_ball,speed_ball)
plt.show()
