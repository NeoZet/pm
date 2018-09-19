import matplotlib.pyplot as plt
import numpy as np
import sys

GRAPH_DATA_FILE = "graph_data.txt"

def _read_coords(filename):
    with open(filename, "r") as coords_file:
        return [[float(elem) for elem in coord.split(' ')] for coord in coords_file.readlines()]

def __get_y(n, x_coord, y_coord):
    if int(n) % 2 == 0:
        m1 = int(n)
        m2 = m1 + 2
        m3 = m1 + 1
    else:
        m3 = int(n)
        m1 = m3 - 1
        m2 = m3 + 1
    t1 = y_coord[x_coord.index(m1)]
    t2 = y_coord[x_coord.index(m2)]    
    t3 = (t1 + t2) / 2    
    def findY(n, m1, m2, m3, t1, t2, t3):
        if n < m3:
            m2 = m3
            m3 = (m1 + m3) / 2
            t2 = t3
            t3 = (t1 + t3) / 2
        elif n > m3:
            m1 = m3
            m3 = (m3 + m2) / 2
            t1 = t3
            t3 = (t3 + t2) / 2
        elif n == m3:
            return t3
        return findY(n,
                     float('{:.3f}'.format(m1)),
                     float('{:.3f}'.format(m2)),
                     float('{:.3f}'.format(m3)),
                     float('{:.3f}'.format(t1)),
                     float('{:.3f}'.format(t2)),
                     float('{:.3f}'.format(t3)))
    return findY(n, m1, m2, m3, t1, t2, t3)

def getY(x, x_coord, y_coord):    
    if x > max(x_coord) or x < min(x_coord):
        print("Error: 'x' out of range", file=sys.stderr)
        return 'e'
    elif x in x_coord:
        return y_coord[x_coord.index(x)]
    else:
        return __get_y(x, x_coord, y_coord)


    
def main():
    coords = _read_coords(GRAPH_DATA_FILE)
    x_crd = [coord[0] for coord in coords]
    y_crd = [coord[1] for coord in coords]
    
    print(getY(260.2, x_crd, y_crd))
    
    plt.plot(x_crd, y_crd)
    plt.grid(True)

if __name__ == "__main__":
    main()
    plt.show()
