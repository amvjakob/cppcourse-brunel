# cppcourse-brunel

### Aim
This program simulates a spiking neural network of 12500 neurons described in Nicolas Brunel's paper "Dynamics of Sparsely Connected Networks of Excitatory and Inhibitory Spiking Neurons", and aims to reproduce figure 8 of the mentioned paper.


### Running
The default values for ETA and G are, respectively, 2 and 5. These can be modified in the src/Constants.hpp file to reproduce the different graphs.
The simulation takes about 20s to run from start to finish.

To run the program, follow these steps:

1. Download or clone repository
2. Navigate to the cloned / downloaded folder (cppcourse-brunel)
3. `cd build` to enter the build folder
4. `cmake ..` to run CMake and generate the makefiles
5. `make` to make both the simlation as well as the tests. Alternatively, `make NeuroSimulation` to generate the simulation only or `make NeuroSimulation_UnitTest` to generate the unit tests only
6. `./NeuroSimulation` to run the simulation, `./NeuroSimulation_UnitTest` to run the tests
7. The result file is created under results/, with the name "spikes_eta[eta_val]_g[g_val].gdf", and contains the times and ids of the neurons that spiked.


### Documentation
To create the documentation from scratch, simply type `make doc` from the build folder to generate it in the doc folder.


### Plot
In order to plot the data, I wrote a script (src/graph.py) to produce both a scatterplot and a histogram.

* The scatterplot shows neuron ID vs spiking time. Every dot corresponds to one spike.

* The histogram shows the number of spikes for a given time interval (500 bins).

Alternatively, the [web application](https://cs116-plot.antoinealb.net/) can also be used to generate the plots.
