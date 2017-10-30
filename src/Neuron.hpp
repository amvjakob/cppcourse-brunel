#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include <array>
#include <random>
#include <algorithm>
#include "Constants.hpp"


class Network;


/// Class representing a neuron
class Neuron {
	
public:
	/*! \brief Neuron constructor
	 *
	 *  Init a neuron with a given resistance and capacity.
	 *  Default values are tau of 20ms
	 *  and capacity of 1 (unitless), resulting in a resistance of 20 (unitless)
	 * 
	 * \param typeExcitatory 	true if the neuron should be excitatory, false otherwise
	 * \param tau				the neuron's membrane constant
	 * \param resistance		the neuron's mebrane's resistance
	 */
	Neuron(bool typeExcitatory = true, double tau = C::TAU, double resistance = C::MEMBRANE_RESISTANCE);
	
	/// default destructor
	virtual ~Neuron() = default;
	
	/// get whether the neuron is refractory
	bool isRefractory() const;
	
	/// get the neuron's current membrane potential
	double getPotential() const;
	
	/// get the neuron's clock
	long getClock() const;
	
	/// get the number of previous spikes
	int getNbSpikes() const;
	

	/*! \brief Get the time of a given previous spike
	 * 
	 * \param index		the index of the spike in the spikes vector
	 * 
	 * \return The time the spike happened if <index> is valid, 
	 * 		   or -2 * C::REFRACTORY_PERIOD otherwise
	 */ 
	long getSpikeTime(int index) const;
	
	
	/*! \brief Get all the neuron's past spikes
	 * 
	 * \return A std::vector containing the time points of the previous spikes
	 */ 
	std::vector<long> getSpikeTimes() const;
	
	
	/*! \brief Get the neuron type
	 *
	 *  \return true if the neuron is excitatory, false if it is inhibitory
	 */
	bool isExcitatory() const;
	
	
	/*! \brief Get the potential delivered to all the neuron's targets after a spike
	 *
	 *  \return C::J_EXCITATORY if the neuron is excitatory, C::J_INHIBITORY if it is inhibitory
	 */
	double getTransmissionValue() const;
	
	
	/*! \brief Add a connection target
	 *
	 *  \param idx		index of the target neuron, relative to the vector of all neurons in the Network
	 */
	void addConnectionTarget(int idx);
	
	
	/*! \brief Get a list of all connection targets
	 *
	 *  \return A constant reference on the targets, seeing as the vector contains 1250 elements
	 */
	const std::vector<int>& getConnectionTargets() const;
	
	
	/*! \brief Receive an incoming spike
	 * 
	 * Adds a transmission potential to the circular buffer
	 * 
	 * \param pot		the potential transmitted post-synaptically from the spiking neuron
	 * \param arrival	the time of arrival of the spike, seen from the simulation clock
	 */ 
	void receive(double pot, long arrival);
	
	
	/*! \brief Main update function
	 *
	 *  Handles firing, membrane potential reset after spike, clock incrementation
	 */
	bool update(int steps, double current);
	
private:

	/*! \brief Updates the neuron's membrane potential
	 *
	 * Updates the neuron's membrane potential according to the general formula,
	 * adds any transmitted potential from the ring buffer,
	 * adds random background noise
	 */
	void updatePotential(double current);
	
	/// Adds a spike to the list of previous spikes
	void fire();
	

private:
	
	double tau;						//!< characteristic circuit constant	
	double resistance;				//!< the membrane's resistance
	double capacity;				//!< the membrane's capacity

	double potential;				//!< the neuron's membrane potential, initialised to 0.0
	
	std::vector<long> spikes; 		//!< a vector containing all the neuron's previous spikes
	
	long clock;						//!< neuron's internal clock, initialised to 0
	
	double c1, c2;					//!< integration constants
	
	/// circular incoming buffer
	std::array<double, C::TRANSMISSION_BUFFER_SIZE> incomingBuffer;
	
	bool typeExcitatory; 			//!< circular incoming buffer
	
	std::vector<int> connections; 	//!< connections
};

#endif
