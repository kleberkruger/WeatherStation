/*
 =======================================================================================================================
 File       : Wetting.cpp
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

#include "Wetting.h"

Wetting::Wetting(PinName pin_ad, PinName pin_s, PinName pin_i) :
		pin_ad(pin_ad), pin_s(pin_s), pin_i(pin_i) {

	this->pin_s = 0;
	this->pin_i = 0;

	config(0, 0, 0, 0);
}

Wetting::Wetting(PinName pin_ad, PinName pin_s, PinName pin_i, float res, int time, float r_min, float r_max) :
		pin_ad(pin_ad), pin_s(pin_s), pin_i(pin_i) {

	this->pin_s = 0;
	this->pin_i = 0;

	config(res, time, r_min, r_max);
}

Wetting::~Wetting() {

	/* Nothing to do */
}

void Wetting::config(float res, int time, float r_min, float r_max) {

	this->r = res * 1000;
	this->tm = time / 2;
	this->r_min = r_min * 1000;
	this->r_max = r_max * 1000;
}

float Wetting::read() {

	float wet_val, lcad;

	pin_s = 1;
	wait_us(tm);
	lcad = pin_ad.read();
	wait_us(tm);
	pin_s = 0;
	pin_i = 1;
	wait_us(tm);
	pin_ad.read();
	wait_us(tm);
	pin_i = 0;
	wet_val = lcad * r / (1.0 - lcad);

	if (wet_val < r_min || lcad == 0.0)
		return (-INFINITY);
	if (wet_val > r_max || lcad == 1.0)
		return (INFINITY);

	return wet_val;
}
