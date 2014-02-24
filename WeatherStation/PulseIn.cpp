/*
 =======================================================================================================================
 File       : PulseIn.cpp
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

#include "PulseIn.h"

PulseIn::PulseIn(PinName pin) :
		pin_in(pin), tm(0) {

	config(MEASURE_DEFAULT, 0, 0);
}

PulseIn::PulseIn(PinName pin, unsigned int timeout_time, int stab_time) :
		pin_in(pin), tm(0) {

	config(MEASURE_DEFAULT, timeout_time, stab_time);
}

PulseIn::PulseIn(PinName pin, MeasureType measure_type, unsigned int timeout_time, int stab_time) :
		pin_in(pin), tm(0) {

	config(measure_type, timeout_time, stab_time);
}

PulseIn::~PulseIn() {

	/* Nothing to do */
}

void PulseIn::config(MeasureType measure_type, unsigned int timeout_time, int stab_time) {

	this->measure_type = measure_type;
	this->timeout_time = timeout_time * 1000;
	this->stab_time = stab_time;
}

void PulseIn::start() {

	timer.start();
	wait_ms (stab_time);

	if (measure_type == MEASURE_LITER) {
		pin_in.rise(this, &PulseIn::stop);
		pin_in.fall(NULL);
	} else {
		pin_in.fall(this, &PulseIn::stop);
		pin_in.rise(NULL);
	}
}

void PulseIn::stop() {
	timer.stop();
	timeout.detach();
	tm = timer.read_ms();
	pin_in.rise(NULL);
	pin_in.fall(NULL);
}

void PulseIn::timeoutDetash() {
	tm = 0;
	timeout.detach();
	pin_in.rise(NULL);
	pin_in.fall(NULL);
}

int PulseIn::getTime() {
	timer.reset();
	tm = -1;

	if (measure_type == MEASURE_LITER) {
		pin_in.mode(PullUp);
		pin_in.fall(this, &PulseIn::start);
	} else {
		pin_in.mode(PullDown);
		pin_in.rise(this, &PulseIn::start);
	}

	timeout.attach_us(this, &PulseIn::timeoutDetash, timeout_time);

	while (tm < 0)
		wait_ms(1); // without this wait_ms does not run correctly

	return tm;
}
