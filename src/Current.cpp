#include "Current.hpp"

Current::Current(double c, long cStart, long cEnd)
	: current(c), currentStart(cStart), currentEnd(cEnd)
{
	// make sure the times are correct
	assert(currentStart <= currentEnd);
}

double Current::getValue(long t) const {
	// if we are in a state of current, return current
	if (currentStart <= t && t <= currentEnd) {
		return current;
	}
	
	// default is no current
	return 0.0;
}
