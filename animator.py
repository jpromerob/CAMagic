
# %matplotlib inline


import sys, getopt
import pdb
import time
import cv2 as cv
import numpy as np
import pdb
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from utils import getNextRow, preprocess, getColorMap, print_all


    
evcmap = getColorMap()

x_max = 640
y_max = 480
scale = 4
fps = 60
activity = 1

screen = np.zeros((x_max, y_max),dtype=int)

# First set up the figure, the axis, and the plot element we want to animate
fig = plt.figure()
ax = plt.axes(xlim=(0, x_max), ylim=(0, y_max))
im = plt.imshow(screen, interpolation='none', cmap=evcmap)

inputfile = "recordings/rec_cam3.aedat4"
nb_events = -1 # i.e. ALL
line_count = 0
t_next_change = 0
first_t = 0
photo_counter = 0
change_interval = int(1000/fps)*1000 # 33 ms : 30Hz  


# initialization function: plot the background of each frame
def init():
    im.set_data(np.zeros((x_max, y_max), dtype=int))
    return [im]

# animation function.  This is called sequentially
def animate(i):

    global line_count, change_interval, x_max, y_max, screen, first_t, t_next_change, evcmap

    for t, x, y, p in getNextRow(inputfile):    
        
        # Extract time offset
        if line_count == 0 :
            first_t = t
            t_next_change = change_interval

        t_relative = t - first_t # first event occurs at t=0

        # Visualize Stuff and Prepare for next frame
        if t_relative > t_next_change:

            screen = preprocess(screen) 
            print_all(screen)
            im.set_array(np.transpose(screen))

            # Clear window
            screen = np.zeros((x_max, y_max),dtype=int)
            t_next_change += change_interval
            
            break
            
        else:
            screen[x,y] += 2*p-1


        line_count += 1
    
    return im

if __name__ == "__main__":

    anim = animation.FuncAnimation(
                                fig, 
                                animate, 
                                frames = fps*2,
                                interval = 1000 / fps, # in ms
                                )   

    plt.show()
