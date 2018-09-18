import matplotlib.pyplot as mplt
import numpy as np

with open("graph_data.txt", "r") as coords_file:
    coords = [[float(elem) for elem in coord.split(' ')] for coord in coords_file.readlines()]
 
x_crd = [coord[0] for coord in coords]
y_crd = [coord[1] for coord in coords]

mplt.plot(x_crd, y_crd)
mplt.grid(True)
mplt.grid(which='minor', color='w', linestyle='-', linewidth=2)
mplt.show()
