#include <cmath>
#include <iostream>
#include "Neuron.hpp"
#include "Network.hpp"

Neuron::Neuron(bool type, double t, double r)
	: tau(t), resistance(r), capacity(r != 0 ? t / r : C::MEMBRANE_CAPACITY),
	  potential(C::V_REST), 
	  clock(0),
	  typeExcitatory(type)
{
	// spikes, empty
	spikes = { };
	
	// ODE integration constants, calculated once
	c1 = exp(- C::STEP_DURATION / tau);
	c2 = resistance * (1.0 - c1);
	
	// incoming potential buffer, zero-initalised
	incomingBuffer = { };
	
	// connections, empty
	connections = { };
}


// get the current membrane potential
double Neuron::getPotential() const {
	return potential;
}

// get the neuron's clock
long Neuron::getClock() const {
	return clock;
}

// get number of previous spikes
int Neuron::getNbSpikes() const {
	return spikes.empty() ? 0 : spikes.size();
}

// returns time of given previous spike
long Neuron::getSpikeTime(int index) const {
	// if the index is valid, return the time of the spike
	if (index >= 0 && index < getNbSpikes()) {
		return spikes.at(index);
	}
	// otherwise, return minus two times the refractory time
	// note: this value is used to make sure the neuron is not in refractory mode by error
	return (long) (-2 * C::REFRACTORY_TIME);
}
// returns all previous spikes
std::vector<long> Neuron::getSpikeTimes() const {
	return spikes;
}

// check if neuron is still in refractory mode
bool Neuron::isRefractory() const {	
	// the neuron is refractory if there was a spike in the last
	// C::REFRACTORY_TIME amount of steps 
	return !spikes.empty() && clock - spikes.back() < C::REFRACTORY_TIME;
}


// get if the neuron is refractory
bool Neuron::isExcitatory() const {
	return typeExcitatory;
}

// get the post-synaptic transmission value
double Neuron::getTransmissionValue() const {
	return isExcitatory() ? C::J_EXCITATORY : C::J_INHIBITORY;
}

// add a connection target
void Neuron::addConnectionTarget(int idx) {
	connections.push_back(idx);
}

// get the list of all connections
const std::vector<int>& Neuron::getConnectionTargets() const {
	return connections;
}

// receive incoming spike
void Neuron::receive(double pot, long arrival) {
	// buffered transmission
	incomingBuffer[arrival % incomingBuffer.size()] += pot;
}

// main update function
bool Neuron::update(int steps, double current) {
	bool spiked = false;
	
	for (int i = 0; i < steps; ++i) {
		// if the potential is over the threshold, emit a spike
		if (potential >= C::V_THRESHOLD) {
			fire();
			spiked = true;
		}
		
		if (!isRefractory()) {
			// update the potential
			updatePotential(current);
		}
		
		// reset incoming buffer field
		incomingBuffer[clock % incomingBuffer.size()] = 0.0;
		
		// increment clock
		++clock;
	}
	
	// return whether the neuron spiked
	return spiked;
}

// update the neuron's potential
void Neuron::updatePotential(double current) {
	// update the potential according to the general formula
	potential = c1 * potential + c2 * current;
	
	// incoming spikes
	potential += incomingBuffer[clock % incomingBuffer.size()];
	
	// background noise
	potential += Network::getBackgroundNoise();
}

// the neurons emits a spike
void Neuron::fire() {
	// add new spike to the list 
	spikes.push_back(clock);

	// reset the membrane potential
	potential = C::V_REST;
}

