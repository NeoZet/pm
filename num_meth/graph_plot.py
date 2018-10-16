import matplotlib.pyplot as plt
import numpy as np
import sys
import argparse
import decimal

GRAPH_DATA_FILE = "graph_data.csv"

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
###########
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





    
def cubic_spline(x_list, y_list, nodes_number):
    interval_len = x_list_g[-1] // (nodes_number - 1)
    x_spline_points = [x_list_g[0] + (interval_len * i) for i in range(nodes_number)]
    y_spline_points = [getY(x_spline_points[i], x_list_g, y_list_g) for i in range(nodes_number)]            

    def _c_coeff(nodes_num, x_spline_points, y_spline_points):
        k = []
        k.append(0)
        c = []
        c.append(0)
        for i in range(1, n):
            j = i - 1
            m = j - 1
            a = x_spline_points[i] - x_spline_points[j]
            b = x_spline_points[j] - x_spline_points[m]
            r = 2 * (a + b) - b * c[j]
            c[i] = a / r
            k[i] = (3 * ((y_spline_points[i] - y_spline_points[j]) / a - (y_spline_points[j] - y_spline_points[m]) / b) - (b * k[j]) / r)

        c[n-1]=k[n-1]
        for i in reversed(range(1, n-1)):
            c[i]=k[i]-c[i]*c[i+1]
        return c
 
def Spl(n, x,f,c, x1, p,p1,p2):
    i=0
    while (x1>x[i]) and (i!=n-2):
        i=i+1
    j=i-1
    a=f[j]
    b=x[j]
    q=x[i]-b
    r=x1-b
    p=c[i]
    d=c[i+1]
    b=(f[i]-a)/q - (d+2*p)*q/3.0
    d=(d-p)/q*r
    p1=b+r*(2*p+d)
    p2=2*(p+d)
    p=a+r*(b+r*(p+d/3.0))
    return p


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
    parser.add_argument('-px', '--point_by_x',
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
    #cubic_spline(x_crd, y_crd, 100)
    nodes_number = 287
    interval_len = x_crd[-1] / (nodes_number - 1)
    # print(float('{:.10f}'.format(interval_len)))    
    x_spl = [float('{:.10f}'.format(x_crd[0] + (interval_len * i))) for i in range(nodes_number)]

    y_spl = [getY(x_spl[i], x_crd, y_crd) for i in range(nodes_number)]

    c = Coeff(nodes_number, x_spl, y_spl)
    print(c)
    p = 0
    p1 = 0
    p2 = 0
    spline = [Spl(nodes_number, x_spl, y_spl, c, x, p, p1, p2) for x in x_spl]
    spline[-1] = y_crd[-1]
    # print(Interpolate(, x_spl, y_spl, c))
    # spline = [Interpolate(x, x_spl, y_spl, c) for x in x_crd]

    print(x_spl)
    print(y_spl)
    print(spline)
    plt.plot(x_crd, y_crd, label='Pit plot', linewidth=3)                
    plt.plot(x_spl, spline, label='Spline', linewidth=2)
    x_coord = None
    if args.point_by_x and args.point_by_x >= min(x_crd) and args.point_by_x <= max(x_crd):
        x_coord = args.point_by_x
        y = getY(args.point_by_x, x_crd, y_crd)
        print("\nPit:\n-----------\nX:{0}\nY:{1}\n============".format(x_coord, y))
        plt.plot(args.point_by_x, y, marker='o', markersize=5, color='red')
        plt.annotate("[{0}, {1}]".format(args.point_by_x, y), (args.point_by_x, y))
    elif args.point_by_x:
        print("Invalid argument for --x_coord", file=sys.stderr)
        
    if args.command == 'polynom':
        if not args.x_list:
            print('WARNING! Incorrect points list')
            args.interpolation_polynom = False
        else:
            x_list = [float(x) for x in args.x_list.split(' ')]
            y_list = [getY(x, x_crd, y_crd) for x in x_list]

        polynom_y = []
        if args.interpolation_polynom == 'Lagrange':
            plt.title("Lagrange interpolation polynomial")
            polynom_y = [Lagrange_interpolation_polynomial(x, x_list, y_list) for x in x_crd]
            y = getY(x_coord, x_crd, polynom_y)
            print("Lagrange:\n-----------\nX:{0}\nY:{1}\n============".format(x_coord, y))
            plt.plot(x_crd, polynom_y, linewidth=2, label='Lagrange polynomial')
        elif args.interpolation_polynom == 'Newton':
            plt.title("Newton interpolation polynomial")
            polynom_y = [Newton_interpolation_polynomial(x, x_list, y_list) for x in x_crd]
            y = getY(x_coord, x_crd, polynom_y)
            print("Newton:\n-----------\nX:{0}\nY:{1}\n============".format(x_coord, y))
            plt.plot(x_crd, polynom_y, linewidth=2, label='Newton polynomial')
        elif args.interpolation_polynom is not None:
            plt.title("Lagrange & Newton interpolation polynomials")

            polynom_y = [Lagrange_interpolation_polynomial(x, x_list, y_list) for x in x_crd]
            y = getY(x_coord, x_crd, polynom_y)
            print("Lagrange:\n-----------\nX:{0}\nY:{1}\n============".format(x_coord, y))
            plt.plot(x_crd, polynom_y, linewidth=3, color='orange', label='Lagrange polynomial')
            
            polynom_y = [Newton_interpolation_polynomial(x, x_list, y_list) for x in x_crd]
            y = getY(x_coord, x_crd, polynom_y)
            print("Newton:\n-----------\nX:{0}\nY:{1}\n============".format(x_coord, y))
            plt.plot(x_crd, polynom_y, linewidth=1, color='black', label='Newton polynomial')

        if x_coord is not None:
            y = getY(x_coord, x_crd, polynom_y) 
            plt.plot(x_coord, y, marker='o', markersize=4, color='green')
            plt.annotate("[{0}, {1}]".format(float('{:.5f}'.format(x_coord)),
                                             float('{:.5f}'.format(y))), (x_coord, y))
            
    

    #plt.ylim(min(y_crd) - 20, max(y_crd) + 20)
    plt.xlabel('X, [mm]')
    plt.ylabel('Y, [mm]')
    plt.legend(loc='best')
    plt.grid(True)

if __name__ == "__main__":
    main()
    plt.show()
