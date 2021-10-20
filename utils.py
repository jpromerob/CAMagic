
# %matplotlib inline




from dv import AedatFile
import numpy as np
from matplotlib import cm
from matplotlib.colors import ListedColormap, LinearSegmentedColormap


def getNextRow(inputfile):

    with AedatFile(inputfile) as ifile:
        # loop through the "events" stream
        for e in ifile["events"]:
            t = e.timestamp
            x = e.x
            y = e.y
            p = int(e.polarity)
            yield (t, x, y, p)

def preprocess(m):
    
    th_green = 1
    th_red = -th_green
    
    
    m[((m < th_green) & (m > th_red))] = 0
    m[m > th_green] = 1
    m[m < th_red] = -1    

    m += np.ones(m.shape, dtype=int)

    return m*128


def getColorMap():
    viridis = cm.get_cmap('viridis', 256)
    newcolors = viridis(np.linspace(0, 1, 256))
    green = np.array([0/256, 255/256, 0/256, 1])
    red = np.array([255/256, 0/256, 0/256, 1])
    black = np.array([0/256, 0/256, 0/256, 1])
    white = np.array([255/256, 255/256, 255/256, 1])
    newcolors[129:256, :] = green  
    newcolors[128, :] = black
    newcolors[0:127, :] = red  
    newcmp = ListedColormap(newcolors)
    
    return newcmp

def print_all(m):
    
    x,y = m.shape
    count_red = 0
    count_green = 0
    count_black = 0
    print("[\n")
    for i in range(x):
        for j in range (y):
            if m[i,j] == 0:
                count_black += 1
            else:                
                print("\t(%d,%d) : %d\n" %(i,j,m[i,j]))
                if m[i,j] < 0:
                    count_red += 1
                if m[i,j] > 0:
                    count_green += 1
    print("]\n")
    
    print("# red: %d\n" %(count_red))
    print("# green: %d\n" %(count_green))
    print("# black: %d\n" %(count_black))


