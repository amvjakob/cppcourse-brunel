#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <array>
#include <random>
#include <algorithm>
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
	 *  Init a new network
	 * 
	 * \param current		 	a current object
	 * \param duration			duration of the simulation in time steps
	 */
	Network(Current* current, long duration = 10000);
	
	/// Netowrk destructor: safely removes dynamically allocated neuron objects
	virtual ~Network();
	
	/// Main function: run the simulation
	void run();
	
	/// Export results to file
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
	
private:

	/*! \brief Creates random connections and assigns them to the neurons
	 *
	 *  Fills a fixed-size table with uniformly distributed values 
	 *  between min and max, without hitting the idx value.
	 *  Assigns the values as neuron targets 
	 * 
	 * \param table		 	array of any size, determines the amount of numbers generated
	 * \param idx			index of target neurons
	 * \param min			lower bound for random number generation
	 * \param max			upper bound for random number generation
	 */
	template<std::size_t SIZE>
	void createConnections(std::array<int, SIZE>& table, int idx, int min, int max) {
		// init random engine and distribution
		std::default_random_engine engine;
		std::uniform_int_distribution<int> distr(min, max);

		// fill table with generated values
		std::generate(
			table.begin(),
			table.end(), 
			[&]() { 
				// the table can be filled with multiple identical indexes,
				// but not with the index of the receiving neuron itself (no self-transmissions)
				int val = -1;
				do {
					val = distr(engine);
				} while (val == idx);
				
				return val; 
			}
		);
		
		// assign to connection vector
		for (int i = 0; i < (int) table.size(); ++i) {
			neurons[table[i]]->addConnectionTarget(idx);
		}
	}

private:
	std::array<Neuron*, C::N_TOTAL> neurons; 	//!< all neurons in the network
	
	Current* current; 							//!< the simulation's current (I)

	long t, tEnd;								//!< current time, ending time
};

#endif
