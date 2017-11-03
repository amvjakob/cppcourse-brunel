#include <iostream>
#include <fstream>
#include <cassert>
#include <ctime>
#include "Network.hpp"

Network::Network(Current* c, long duration)
	: current(c),
	  t(0), tEnd(std::abs(duration))
{
	std::cout << "Generating network..." << std::flush;
	time_t t1 = time(0);
	
	// init neuron array
	neurons = { nullptr };

	// generate neurons
	for (int i = 0; i < (int) C::N_TOTAL; ++i) {
		// neurons from index 0 to C::N_EXCITATORY are exictatory,
		// the rest are inhibitory
		bool isExcitatory = i < C::N_EXCITATORY;
		neurons[i] = new Neuron(isExcitatory);		
	}
	
	// generate random connections

	std::array<int, (std::size_t) (C::C_EXCITATORY)> excitatoryTable = { 0 };
	std::array<int, (std::size_t) (C::C_INHIBITORY)> inhibitoryTable = { 0 };
	
	for (int i = 0; i < (int) C::N_TOTAL; ++i) {
		// create and assign excitatory connections
		createConnections(excitatoryTable, i, 0, C::N_EXCITATORY - 1);
		
		// create and assign inhibitory connections
		createConnections(inhibitoryTable, i, C::N_EXCITATORY, C::N_TOTAL - 1);
	}
	
	
	time_t t2 = time(0);
	std::cout << " [done in " << t2 - t1 << "s]" << std::endl;
}


Network::~Network() {
	// safely delete all dynamically allocated neurons
	for (auto& neuron : neurons) {
		delete neuron;
		neuron = nullptr;
	}
}

void Network::run() {
	std::cout << "Running..." << std::flush;

	// get beginning of the simulation
	time_t t1 = time(0);
	
	// main simulation loop
	while (t < tEnd) {	
			
		// update the network
		for (auto neuron : neurons) {
			
			// update the neuron, 1 step, no external current
			bool spiked = neuron->update(1, current->getValue(t));
			
			if (spiked) {				
				// transmit spike to targets with delay
				for (auto target : neuron->getConnectionTargets()) {
					neurons[target]->receive(neuron->getTransmissionValue(), t + C::TRANSMISSION_DELAY);
				}
			}
		}
		
		// increment time
		++t;
	}

	// get end of the simulation
	time_t t2 = time(0);

	std::cout << "[done in " << t2 - t1 << " s, " << tEnd << " steps]" << std::endl;
}


void Network::save() const {
	std::cout << "Saving..." << std::endl;
	
	// open result file
	std::ofstream log;
	log.open("spikes.gdf");
	
	// write each spike to the file
	for (int i = 0; i < (int) neurons.size(); ++i) {
		for (auto spikeTime : neurons.at(i)->getSpikeTimes()) {
			log << spikeTime << '\t' << i << '\n';
		}
	}
	
	log.close();
}
