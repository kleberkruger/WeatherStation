/*
 =======================================================================================================================
 File       : Watchdog.h
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

#ifndef WATCHDOG_H_
#define WATCHDOG_H_

#include "mbed.h"

class Watchdog {
public:

	Watchdog();
	virtual ~Watchdog();

	/**
	 * Load timeout value in watchdog timer and enable.
	 */
	void kick(float s);

	/**
	 * "kick" or "feed" the dog - reset the watchdog timer
	 * by writing this required bit pattern.
	 */
	void kick();
};

#endif /* WATCHDOG_H_ */
