import matplotlib.pyplot as plt
import numpy as np
import sys
import argparse
import decimal

GRAPH_DATA_FILE = "graph_data.txt"

def _read_coords(filename):
    with open(filename, "r") as coords_file:
        return [[float(elem) for elem in coord.split(',')] for coord in coords_file.readlines()]


def getY(x, x_coord, y_coord):
    ########## >>>> __get_y
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
        ########### >>>> findY
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
                         float('{:.10f}'.format(m1)),
                         float('{:.10f}'.format(m2)),
                         float('{:.10f}'.format(m3)),
                         float('{:.10f}'.format(t1)),
                         float('{:.10f}'.format(t2)),
                         float('{:.10f}'.format(t3)))
        ############ findY <<<<
        return findY(n, m1, m2, m3, t1, t2, t3)
    ########## __get_y <<<<
    1
    if x > max(x_coord) or x < min(x_coord):
        print("Error: 'x' out of range", file=sys.stderr)
        return 'e'
    elif x in x_coord:
        return y_coord[x_coord.index(x)]
    else:
        return __get_y(x, x_coord, y_coord)


def Lagrange_interpolation_polynomial(x, x_list, y_list):
    if len(x_list) != len(y_list):
        print("Error! Invalid argument", file=sys.stderr)
        return 'e'
    L = 0    
    for i in range(len(y_list)):         
        p = 1
        for j in range(len(y_list)):
            if j != i:
                p *= ((x - x_list[j]) / (x_list[i] - x_list[j]))
        L += y_list[i] * p
    return L


def Newton_interpolation_polynomial(x, x_list, y_list):
    if len(x_list) != len(y_list):
        print("Error! Invalid argument", file=sys.stderr)
        return 'e'
    P = y_list[0]
    for k in range(2, len(y_list)+1):
        f_k = 0
        for i in range(k):
            pk_x = 1
            for j in range(k):
                if j != i:
                   pk_x *= x_list[i] - x_list[j]            
            f_k += y_list[i] / pk_x
        p_x = 1
        for i in range(k-1):
            p_x *= x - x_list[i]
        P += f_k * p_x
    return P
        
    
def create_parser():
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest='command')
    
    parser_polynom = subparsers.add_parser('polynom', help='work with polynomial')
    parser_polynom.add_argument('-ip', '--interpolation_polynomial',
                                dest='interpolation_polynom',
                                help='''specifing interpolation polynomial
                                (Lagrange or Newton, both by default)''',
                                default='')
    parser_polynom.add_argument('-x', '--x_list',
                                dest='x_list',
                                help='list of x values (For example "x_1 x_2 x_3")',
                                type=str)
    
    parser.add_argument('-f', '--filename',
                        dest='filename',
                        help='file with coordiantes of points',
                        type=str)
    parser.add_argument('-x', '--x_coord',
                        help='print point on plot by X coordinate',
                        type=float)
    return parser
    
    
def main():
    parser = create_parser()
    args = parser.parse_args()

    if args.filename:
        coords = _read_coords(args.filename)
    else:        
        coords = _read_coords(GRAPH_DATA_FILE)
        if not coords:
            print("Error! File with coordinates are apsent", file=sys.stderr)
            return 1
    x_crd = [coord[0] for coord in coords]
    y_crd = [coord[1] for coord in coords]
    plt.plot(x_crd, y_crd, label = 'Pit plot')        

    if args.command == 'polynom':
        if not args.x_list:
            print('WARNING! Incorrect points list')
            args.interpolation_polynom = False
        else:
            x_list = [float(x) for x in args.x_list.split(' ')]
            y_list = [getY(x, x_crd, y_crd) for x in x_list]
            print(getY(13, x_crd, y_crd))
            
        if args.interpolation_polynom == 'Lagrange':     
            lagr_y = [Lagrange_interpolation_polynomial(x, x_list, y_list) for x in x_crd]
            plt.plot(x_crd, lagr_y, linewidth=2, label='Lagrange interpolation polynomial')
        elif args.interpolation_polynom == 'Newton':
            newt_y = [Newton_interpolation_polynomial(x, x_list, y_list) for x in x_crd]
            plt.plot(x_crd, newt_y, linewidth=2, label='Newton interpolation polynomial')
        elif args.interpolation_polynom is not None:
            print(y_list)
            lagr_y = [Lagrange_interpolation_polynomial(x, x_list, y_list) for x in x_crd]            
            newt_y = [Newton_interpolation_polynomial(x, x_list, y_list) for x in x_crd]
            plt.plot(x_crd, lagr_y, linewidth=3, color='orange', label='Lagrange interpolation polynomial')        
            plt.plot(x_crd, newt_y, linewidth=1, color='black', label='Newton interpolation polynomial')

    if args.x_coord and args.x_coord >= min(x_crd) and args.x_coord <= max(x_crd):
        y = getY(args.x_coord, x_crd, y_crd) 
        plt.plot(args.x_coord, y, marker='o', markersize=4)
        plt.annotate("[{0}, {1}]".format(args.x_coord, y), (args.x_coord, y))
    elif args.x_coord:
        print("Invalid argument", file=sys.stderr)

    plt.ylim(min(y_crd) - 20, max(y_crd) + 20)  
    plt.legend(loc='best')
    plt.grid(True)

if __name__ == "__main__":
    main()
    plt.show()
