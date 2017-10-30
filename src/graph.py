import numpy as np
import pylab as pl

data = np.genfromtxt('spikes.gdf')

select = np.array([d for d in data if d[1] < 30])
data1 = select.transpose()
p1.scatter(0.1 * data[0], data1[1], alpha = 0.8, edgecolors = 'none')
p1.show()
