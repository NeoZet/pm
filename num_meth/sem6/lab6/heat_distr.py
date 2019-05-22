import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
import json
import os.path
import sys

# results filename should be added into config
def get_default_parameters():
	default_parameters = {   
		"a": 1.9,
		"coords_info": {
			"x_size": 50,
			"y_size": 30,
			"step": 0.7,
		},
		"duration_info": {
			"duration": 20,
			"step": 0.1
		},
		"border_temperature": {
			"left": 300,
			"right": 300,
			"top": 300,
			"bottom": 300
		},
		"heat_source": {
			"temperature": 1200,
			"size": 3
		},
		"global_temperature_info": {
			"max": 1200,
			"min": -200,
			"inital": 0
		}
	}
	default_parameters["coords_info"]["x_points_number"] = int(default_parameters["coords_info"]["x_size"] / default_parameters["coords_info"]["step"])
	default_parameters["coords_info"]["y_points_number"] = int(default_parameters["coords_info"]["y_size"] / default_parameters["coords_info"]["step"])
	return default_parameters

def parse_config(config_name='config.json'):
	if not os.path.isfile(config_name):
		print("File {0} not found. Default parameters will be used".format(config_name))
		return get_default_parameters()

	with open(config_name, 'r') as cfg:
		params = json.load(cfg)
	params["coords_info"]["x_points_number"] = int(params["coords_info"]["x_size"] / params["coords_info"]["step"])
	params["coords_info"]["y_points_number"] = int(params["coords_info"]["y_size"] / params["coords_info"]["step"])
	return params

def setup_initial_and_border_conditions(params):
	nx = params["coords_info"]["x_points_number"]
	ny = params["coords_info"]["y_points_number"]
	inital_distrib = params["global_temperature_info"]["inital"]
	
	heat_src_size = params["heat_source"]["size"]
	heat_src_temp = params["heat_source"]["temperature"]

	T = [[inital_distrib] * nx for j in range(ny)]

	for i in range(ny):
		T[i][-1] = params["border_temperature"]["right"]
		T[i][0] = params["border_temperature"]["left"]

	for i in range(nx):
		T[-1][i] = params["border_temperature"]["bottom"]
		T[0][i] = params["border_temperature"]["top"]
			
	midy = int(ny/2) 
	midx = int(nx/2)
	for i in range(heat_src_size):
		for j in range(heat_src_size):
			T[midy+i][midx+j] = heat_src_temp
			T[midy-i][midx-j] = heat_src_temp
			T[midy-i][midx+j] = heat_src_temp
			T[midy+i][midx-j] = heat_src_temp
	return T


def temperature_distribution_at_moment(params, current_distrib):
	nx = params["coords_info"]["x_points_number"]
	ny = params["coords_info"]["y_points_number"]
	tau = params["duration_info"]["step"]
	a = params["a"]
	h = params["coords_info"]["step"]

	heat_src_size = params["heat_source"]["size"]
	T = current_distrib

	## Will be deprecated when heat source coordinates becomes configurable ##
	midy = int(ny/2) 
	midx = int(nx/2)

	for i in range(1, ny-1):
		for j in range(1, nx-1):
			if(i < midy+heat_src_size and i > midy-heat_src_size and j < midx+heat_src_size and j > midx-heat_src_size):
				continue
			T[i][j] = int(T[i][j] + 
						 (tau * a / h**2) * 
						 ((T[i+1][j] - 2*T[i][j] + T[i-1][j]) + 
						  (T[i][j+1] - 2*T[i][j] + T[i][j-1])))	
	return T


def calculate_and_animate_result(params, initial_and_border_cond, fig, im, save=False): 
	min_temp = params["global_temperature_info"]["min"]
	max_temp = params["global_temperature_info"]["max"]

	global T 
	T = initial_and_border_cond
	
	def init_animation():
		im.set_data(T)
		return im,

	def update_frame(*args):
		global T
		T = temperature_distribution_at_moment(params, T)
		im.set_array(T)
		return im,

	ani = animation.FuncAnimation(fig, update_frame, frames=200, interval=200, blit=True, init_func=init_animation)

	if save:
		ani.save('res.gif', dpi=100, writer='imagemagick')
	return ani


def calculate_and_plot_result(params, initial_and_border_cond, im, save=False):
	time = 0
	time_end = params["duration_info"]["duration"]
	tau = params["duration_info"]["step"]
	min_temp = params["global_temperature_info"]["min"]
	max_temp = params["global_temperature_info"]["max"]
	
	T = initial_and_border_cond

	while time < time_end:
		time += tau
		T = temperature_distribution_at_moment(params, T)
	im.set_data(T)
	if save:
		plt.savefig("res.png", dpi=200)
	return 0


def calculate_and_display_results(params, type="plot", save=False):
	min_temp = params["global_temperature_info"]["min"]
	max_temp = params["global_temperature_info"]["max"]

	initial_and_border_cond = setup_initial_and_border_conditions(params)

	fig = plt.figure()
	im=plt.imshow(initial_and_border_cond, cmap='gist_rainbow_r', vmin=min_temp, vmax=max_temp)

	levels = np.arange(min_temp, max_temp, 100)
	c = plt.colorbar()
	c.set_ticks(levels)
	c.set_ticklabels(levels)

	if type == "plot":
		res = calculate_and_plot_result(params, initial_and_border_cond, im, save)
	elif type == "anim":
		res = calculate_and_animate_result(params, initial_and_border_cond, fig, im, save)
	else:
		print("INCORRECT DISPLAY TYPE")
		return -1

	if not save:
		plt.show()
	return res

def print_usage():
	print("Usage: {0} [plot | anim]".format(sys.argv[0]))
	print("Use --help, -h for show this message")

def main():
	params = parse_config()
	save_state = False
	if len(sys.argv) < 2:
		print_usage()
		return -1
	elif sys.argv[1] != "plot" and sys.argv[1] != "anim":
		print_usage()
		return -1
	elif len(sys.argv) == 3 and sys.argv[2] == "save":
		save_state = True

	arg = sys.argv[1] if sys.argv[1] == "anim" else "plot"
	return calculate_and_display_results(params, type=arg, save=save_state)


if __name__ == "__main__":
	main()
