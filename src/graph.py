import numpy as np
import pylab as pl

raw_data = np.genfromtxt('spikes.gdf')

n_neurons = 400

select = np.array([d for d in raw_data if d[1] < n_neurons])
data = select.transpose()

fig = pl.figure()

main_scatter = fig.add_subplot(211)
main_scatter.scatter(0.1 * data[0], data[1], alpha = 0.8, edgecolors = 'none', s = 5)
main_scatter.set_ylabel('Neuron ID')

histo = fig.add_subplot(212)
histo.hist(0.1 * data[0], bins = 100)
histo.set_xlabel('t [ms]')
histo.set_ylabel('rate [Hz]')


pl.show()
