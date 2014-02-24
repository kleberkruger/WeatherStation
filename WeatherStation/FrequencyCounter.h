/*
 =======================================================================================================================
 File       : FrequencyCounter.cpp
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

#ifndef FREQUENCYCOUNTER_H_
#define FREQUENCYCOUNTER_H_

#include "mbed.h"

class FrequencyCounter {
public:

	typedef enum {
		TRANS_FALL, TRANS_RISE
	} TransType;

	/**
	 * Creates the InterruptIn on the pin specified to Counter.
	 * Modified to add debounce.
	 *
	 * @param pin 			- pin name
	 * @param trans_type 	- transmission type
	 * @param pin_mode 		- pin mode
	 * @param stab_time 	- stabilization time (in us)
	 */
	FrequencyCounter(PinName pin, PinMode pin_mode, TransType trans_type, unsigned int stab_time);

	/**
	 * Destructor.
	 */
	virtual ~FrequencyCounter();

	void resetCount();
	int getCount();

private:

	InterruptIn interrupt;
	Timeout timeout;
	DigitalIn pin;
	TransType trans_type;
	unsigned int stab_time;
	volatile int count;

	void schedule();
	void increment();
};

#endif /* FREQUENCYCOUNTER_H_ */
