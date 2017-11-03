#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include <array>
#include "Constants.hpp"

// Circular dependency resolution
class Network;


/// Class representing a Neuron
class Neuron {
	
public:
	/*! \brief Neuron constructor
	 *
	 *  Initialize a neuron of a specific type, with specific values for tau, R
	 *  Default values are tau of 20 and resistance of 20, 
	 *  resulting in a resistance of 1
	 * 
	 * \param typeExcitatory 	true if the neuron should be excitatory, false otherwise
	 * \param tau				the neuron's membrane constant
	 * \param resistance		the neuron's membrane's resistance
	 */
	Neuron(bool typeExcitatory = true, double tau = C::TAU, double resistance = C::MEMBRANE_RESISTANCE);
	
	/// Default destructor
	virtual ~Neuron() = default;
	
	
	/// Get the neuron's current membrane potential
	double getPotential() const;
	
	/// Get the neuron's internal clock
	long getClock() const;
	
	/// Get the number of previous spikes
	int getNbSpikes() const;
	

	/*! \brief Get the time of a given previous spike
	 * 
	 * \param index		the index of the spike in the spikes vector
	 * 
	 * \return The time the spike happened if \p index is valid, 
	 * 		   or -2 * C::REFRACTORY_PERIOD otherwise
	 */ 
	long getSpikeTime(int index) const;
	
	
	/*! \brief Get all the neuron's past spikes
	 * 
	 * \return A std::vector containing the time points of the previous spikes
	 */ 
	std::vector<long> getSpikeTimes() const;


	/*! \brief Get whether the neuron is refractory
	 *
	 *  The neuron is refractory for C::REFRACTORY_TIME time steps after
	 *  its last spike
	 */
	bool isRefractory() const;
	
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
	 *  \return A constant reference on the targets, seeing as the vector contains around 1250 elements
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
	 *  Handles firing, potential updating, resetting of incoming buffer, 
	 *  clock incrementation
	 */
	bool update(int steps, double current);
	
protected:

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
	
	long clock;						//!< neuron's internal clock, initialised to 0

	bool typeExcitatory; 			//!< circular incoming buffer

	std::vector<long> spikes; 		//!< a vector containing all the neuron's previous spikes
	
	double c1, c2;					//!< integration constants
	
	/// circular incoming buffer
	std::array<double, C::TRANSMISSION_BUFFER_SIZE> incomingBuffer;
	
	/// target connections - index of target neuron in the Network's list of Neurons
	std::vector<int> connections; 	
};

#endif
