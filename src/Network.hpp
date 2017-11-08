#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <array>
#include <random>
#include <algorithm>
#include <cassert>
#include "Current.hpp"
#include "Neuron.hpp"
#include "Constants.hpp"

/** \brief Class representing a Network
 * 
 * Handles neurons and their connections
 * */
class Network {
public:
	/*! \brief Network constructor 
	 *
	 * Initializes a new network
	 * 
	 * \param current		 	a Current object (I)
	 * \param duration			length of the simulation in number of time steps
	 */
	Network(Current* current, long duration = 10000);
	
	/// Network destructor: safely removes dynamically allocated neuron objects
	virtual ~Network();
	
	/*! \brief Run the simulation
	 *
	 * Updates each neuron with the given current for 
	 * the specified number of time steps
	 */
	void run();
	
	/*! \brief Export results to file
	 *
	 * Stores the results of the simulation (all spikes and when they happened)
	 * in a file with the following format:
	 * 
	 * [step at which a spike happened][tab][index of the spiking Neuron]
	 */
	void save() const;
	
	
	/*! \brief Generate random background noise
	 *
	 *  Uses the mt19937 generator, follows a poisson distribution
	 */
	static double getBackgroundNoise() {
		// get random device
		static std::random_device randomDevice;
		
		// init random generator
		static std::mt19937 gen(randomDevice());
		
		// init poisson distribution
		static std::poisson_distribution<> poissonGen(C::V_EXT * C::STEP_DURATION);
		
		// number of spikes during one step
		int nSpikes = poissonGen(gen);
		
		// background noise is number of spikes times J
		return nSpikes * C::J_EXCITATORY;
	}
	
protected:

	/*! \brief Creates random connections and assigns them to the Neurons
	 *
	 *  Fills a fixed-size table with uniformly distributed values 
	 *  between \p min and \p max.
	 * 	The generated numbers represent the indices of the sources of the neuron at index \p idx.
	 *  Assigns the neuron at index \p idx as target of the generated sources.
	 * 
	 * \param table		 	array of any size, determines the amount of numbers generated
	 * \param idx			index of target neurons
	 * \param min			lower bound for random number generation
	 * \param max			upper bound for random number generation
	 */
	template<std::size_t SIZE>
	void createConnections(std::array<int, SIZE>& table, int idx, int min, int max) {
		// init random engine and distribution
		static std::default_random_engine engine;
		std::uniform_int_distribution<int> distr(min, max);

		// fill table with generated values
		std::generate(
			table.begin(),
			table.end(), 
			[&]() { 				
				return distr(engine); 
			}
		);
		
		// assign to connection vector
		for (int source : table) {
			// check for correct index
			assert(source < (int) neurons.size());
			
			// assign new target to source
			neurons[source]->addConnectionTarget(idx);
		}
	}

private:

	Current* current; 							//!< the simulation's current (I)

	long t, tEnd;								//!< current time, ending time

	/** all neurons in the network, where the first C::N_EXCITATORY neurons are excitatory,
	 *  and the rest are inhibitory
	 * */
	std::array<Neuron*, C::N_TOTAL> neurons; 

};

#endif
