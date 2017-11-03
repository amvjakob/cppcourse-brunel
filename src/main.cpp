#include "Network.hpp"
#include "Current.hpp"
#include "Constants.hpp"

// note: we work with number of steps as "time unit"
int main(int argc, char** argv) {
	
	/*
	double currentMag = C::DEFAULT_CURRENT_MAG;
	
	if (argc > 1) { // launch with command line argument if present
		currentMag = atof(argv[1]);
	}
	*/
	
	// create current (I) object - no external current
	Current* current = new Current(
		0.0, 	// magnitude
		0,		// start
		0		// stop
	);
	
	// generate new network
	Network network = Network(
		current, 
		10000	// length of the simulation in time steps
	);
	
	// run the simulation
	network.run();
	
	// save the results
	network.save();
	
	// delete the current
	delete current;
	current = nullptr;
	
	return 0;
}

