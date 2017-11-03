import numpy as np
import pylab as pl

raw_data = np.genfromtxt('spikes.gdf')

select = np.array([d for d in raw_data if d[1] < 50])
data = select.transpose()

fig = pl.figure(dpi = 200)


# Main plot
main_scatter = fig.add_subplot(211)
main_scatter.scatter(0.1 * data[0], data[1], alpha = 0.8, edgecolors = 'none', s = 10)
main_scatter.set_ylabel('Neuron ID')
#pl.xlim([100, 300])


# Histogram
histo = fig.add_subplot(212)
histo.hist(0.1 * data[0], 500, normed = 0, alpha = 0.75)
histo.set_xlabel('t [ms]')
histo.set_ylabel('rate [Hz]')
#pl.xlim([100, 300])


# Export graph
fig.savefig('spikes.png')


# Show graph
pl.show()
