#ifndef CURRENT_H
#define CURRENT_H

/** \brief Class representing a Current (I)
 * 
 * Could be useful for later if one does not simply want to use
 * a constant current between two time points.
 * In hindsight however, this class is superfluous.
 * */
class Current {
	
public:
	/*! \brief Current constructor 
	 *
	 * Initialize a new current
	 * 
	 * \param current		 	the current magnitude, defaulted to 1.0
	 * \param currentStart		the time the current starts, defaulted to 0
	 * \param currentEnd		the time the current ends, defaulted to 1000
	 */
	Current(double current = 1.0, long currentStart = 0, long currentEnd = 1000);
	
	/// Default destructor
	virtual ~Current() = default;
	
	/// Get the value of the current at time step t
	double getValue(long t) const;
	
private:
	double current;					//!< current magnitude
	
	long currentStart, currentEnd;	//!< time interval for current
};

#endif
