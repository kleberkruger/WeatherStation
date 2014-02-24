/*
 =======================================================================================================================
 File       : PulseIn.h
 -----------------------------------------------------------------------------------------------------------------------
 Author     : Kleber Kruger
 Email      : kleberkruger@gmail.com
 Reference  : Fábio Iaione
 Date       : 2014-02-02
 Version    : 1.0
 Copyright  : Faculty of Computing, FACOM - UFMS
 -----------------------------------------------------------------------------------------------------------------------
 Description: Weather station with implementing fault tolerance
 =======================================================================================================================
 */

#ifndef PULSEIN_H_
#define PULSEIN_H_

#include "mbed.h"

class PulseIn {
public:

	typedef enum {
		MEASURE_DEFAULT, MEASURE_LITER,
	} MeasureType;

	/**
	 * Constructor
	 * @param pin 	- pin name
	 */
	PulseIn(PinName pin);

	/**
	 * Constructor
	 *
	 * @param pin 			- pin name
	 * @param timeout_time	- timeout time
	 * @param stab_time		- stabilization time
	 */
	PulseIn(PinName pin, unsigned int timeout_time, int stab_time);

	/**
	 * Constructor
	 *
	 * @param pin 			- pin name
	 * @param measure_type 	- measure type
	 * @param timeout_time	- timeout time
	 * @param stab_time		- stabilization time
	 */
	PulseIn(PinName pin, MeasureType measure_type, unsigned int timeout_time, int stab_time);

	/**
	 * Destructor
	 */
	virtual ~PulseIn();

	void config(MeasureType measure_type, unsigned int timeout_time, int stab_time);
	int getTime();

private:

	InterruptIn pin_in;
	Timer timer;
	Timeout timeout;
	MeasureType measure_type;
	unsigned int timeout_time;
	int stab_time;
	int tm;

	void start();
	void stop();
	void timeoutDetash();
};

#endif /* PULSEIN_H_ */
