#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>

namespace C {
	
	/// Duration in s of one simulation step (0.1ms)
	constexpr auto STEP_DURATION = 0.1E-3;
	
	/// Tau of neuron membrane (20ms)
	constexpr double TAU = 20E-3;
	
	/// Resistance of neuron membrane (20 GOhm)
	constexpr double MEMBRANE_RESISTANCE = 20.0;
	
	/// Capacity of neuron membrane 
	constexpr double MEMBRANE_CAPACITY = TAU / MEMBRANE_RESISTANCE;
	
	/** Membrane potential threshold;
	 *  a neuron with a membrane potential above this value will fire
	 * */
	constexpr double V_THRESHOLD = 20.0;

	/// Membrane potential at rest
	constexpr double V_REST = 0.0;

	/// Amount of steps the neuron stays inactive after spiking
	constexpr int REFRACTORY_TIME = 20;

	/// Delay in spike transmission in steps
	constexpr int TRANSMISSION_DELAY = 15;
	
	/// Ring buffer size for spike transmission storage
	constexpr int TRANSMISSION_BUFFER_SIZE = TRANSMISSION_DELAY + 1;

	
	/// Default current, should not create spikes
	constexpr auto DEFAULT_CURRENT_MAG = 1.0;
	
	
	constexpr auto G = 5;									//!< Ratio of inhibitory vs excitatory transmission values
	constexpr auto J_EXCITATORY = 0.1; 						//!< Post-synaptic excitement after spike
	constexpr auto J_INHIBITORY = -G * J_EXCITATORY;		//!< Post-synaptic inhibition after spike
	
	
	constexpr double N_RATIO = 0.25;						//!< Ratio of inhibitory vs excitatory neurons
	constexpr int N_EXCITATORY = 10000;						//!< Number of excitatory neurons
	constexpr int N_INHIBITORY = N_RATIO * N_EXCITATORY;	//!< Number of inhibitory neurons
	constexpr int N_EXT = N_EXCITATORY;						//!< Number of external neurons
	
	constexpr int N_TOTAL = N_EXCITATORY + N_INHIBITORY;	//!< Total number of neurons in simulation
	
	
	/// Connectivity value
	constexpr double EPSILON = 0.1;
	
	/// Number of incoming excitatory connections for any neuron
	constexpr int C_EXCITATORY = (int) (EPSILON * N_EXCITATORY);
	/// Number of incoming inhibitory connections for any neuron
	constexpr int C_INHIBITORY = (int) (EPSILON * N_INHIBITORY);
	/// Number of total incoming connections for any neuron
	constexpr int C_TOTAL = C_EXCITATORY + C_INHIBITORY;
	
	/// Multiplier for the average frequency of external random spikes
	constexpr double ETA = 2.0;

	/// Average frequency of external random spikes
	constexpr double V_EXT = ETA * V_THRESHOLD / (J_EXCITATORY * TAU);
	

	/* // unused
	
	/// Transform current time to steps
	static long timeToSteps(long t) {
		return static_cast<unsigned long>(std::ceil(t / STEP_DURATION));
	}
	
	/// Transform steps to time
	static long stepsToTime(long steps) {
		return steps * STEP_DURATION;
	}
	* */
}

#endif
