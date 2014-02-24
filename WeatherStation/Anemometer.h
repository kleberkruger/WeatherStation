/*
 =======================================================================================================================
 File       : Anemometer.h
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

#ifndef ANEMOMETER_H_
#define ANEMOMETER_H_

#include "PulseIn.h"

class Anemometer: public PulseIn {
public:

	Anemometer(PinName pin) : PulseIn(pin, MEASURE_LITER, 500, 5) {}
	virtual ~Anemometer() {}

	inline float read() { return PulseIn::getTime() * PULSETIME_TO_WINDSPEED; }

private:

	static const int PULSETIME_TO_WINDSPEED = 1; // Constant to convert pulse time (in milliseconds) to wind speed (m/s)
};

#endif /* ANEMOMETER_H_ */
