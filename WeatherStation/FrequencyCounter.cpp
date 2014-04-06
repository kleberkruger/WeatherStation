/*
 =======================================================================================================================
 File       : Counter.cpp
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

#include "FrequencyCounter.h"

/**
 * Constructor.
 * Creates the InterruptIn on the pin specified to Counter.
 */
FrequencyCounter::FrequencyCounter(PinName pin, PinMode pin_mode, TransType trans_type, unsigned int stab_time) :
		interrupt(pin), pin(pin), trans_type(trans_type), stab_time(stab_time) {

	if (trans_type == TRANS_FALL)
		interrupt.fall(this, &FrequencyCounter::schedule); // attach increment function of this counter instance
	else if (trans_type == TRANS_RISE)
		interrupt.rise(this, &FrequencyCounter::schedule); // attach increment function of this counter instance

	interrupt.mode(pin_mode);

	resetCount(); // reset counter
}

/**
 * Destructor.
 */
FrequencyCounter::~FrequencyCounter() {

	/* Nothing to do */
}

void FrequencyCounter::schedule() {
	timeout.attach_us(this, &FrequencyCounter::increment, stab_time);
}

void FrequencyCounter::increment() {
	if (pin == trans_type) {
		count++;
		LPC_RTC->GPREG0 = count;
	}
}

void FrequencyCounter::resetCount() {
	count = 0;
	LPC_RTC->GPREG0 = count;
}

int FrequencyCounter::getCount() {
	count = LPC_RTC->GPREG0;
	return count;
}
