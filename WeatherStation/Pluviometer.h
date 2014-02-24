/*
 =======================================================================================================================
 File       : Pluviometer.h
 -----------------------------------------------------------------------------------------------------------------------
 Author     : Kleber Kruger
 Email      : kleberkruger@gmail.com
 Date       : 2014-02-02
 Version    : 1.0
 Copyright  : Faculty of Computing, FACOM - UFMS
 -----------------------------------------------------------------------------------------------------------------------
 Description: Weather station with implementing fault tolerance
 =======================================================================================================================
 */

#ifndef PLUVIOMETER_H_
#define PLUVIOMETER_H_

#include "FrequencyCounter.h"

class Pluviometer: public FrequencyCounter {
public:

	Pluviometer() : FrequencyCounter(p22, PullUp, TRANS_FALL, 5000) {}
	virtual ~Pluviometer() {}

	inline float read() { return FrequencyCounter::getCount() * PULSE_TO_MILLIMETER; }

private:

	static const double PULSE_TO_MILLIMETER = 0.254; // Value (in millimeter) of a pluviometer pulse.
};

#endif /* PLUVIOMETER_H_ */
