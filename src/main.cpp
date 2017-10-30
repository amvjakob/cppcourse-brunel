#include "Network.hpp"
#include "Current.hpp"
#include "Constants.hpp"

// Note: we work with number of steps as "time unit"
int main(int argc, char** argv) {
	
	// use constant value as default value
	double currentMag = C::DEFAULT_CURRENT_MAG;
	
	// launch with command line argument if present
	if (argc > 1) {
		currentMag = atof(argv[1]);
	}
	
	// for now, no outside current
	currentMag = 0.0;
	
	// create new curent pointer
	Current* current = new Current(currentMag, 1000.0, 4000.0);
	
	// creat new network simulation
	Network network = Network(current, 1000);
	
	// run the simulation
	network.run();
	
	// save the results
	network.save();
	
	// delete the current
	delete current;
	current = nullptr;
	
	return 0;
}

