import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm

fname= raw_input("Please write file name:")

x, y, temp = np.loadtxt(fname, delimiter=' ', unpack=True)
nrows, ncols = 1000, 1000
grid = temp.reshape((nrows, ncols))
grid = grid.T

plt.imshow(grid, extent=(x.min(), x.max(), y.min(), y.max()),interpolation='nearest') #, cmap=cm.gist_rainbow
plt.show()