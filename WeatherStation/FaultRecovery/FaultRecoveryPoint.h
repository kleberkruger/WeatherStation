/*
 =======================================================================================================================
 File       : FaultRecoveryPoint.h
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

#ifndef FAULTRECOVERYPOINT_H_
#define FAULTRECOVERYPOINT_H_

#include "mbed.h"

class FaultRecoveryPoint {
public:

	/**
	 * Create a FaultRecoveryPoint.
	 *
	 * @param state - application state
	 * @param fptr 	- pointer to the function to be called
	 * @param wdt	- watchdog time
	 */
	FaultRecoveryPoint(int state, void (*fptr)(void), float wdt) :
			state(state), wdt(wdt), _function(fptr) { }

	/**
	 * Create a FaultRecoveryPoint
	 *
	 * @param state - application state
	 * @param tptr 	- pointer to the object to call the member function on
	 * @param mptr 	- pointer to the member function to be called
	 * @param wdt	- watchdog time
	 */
	template<typename T>
	FaultRecoveryPoint(int state, T* tptr, void (T::*mptr)(void), float wdt) :
			state(state), wdt(wdt), _function(tptr, mptr) { }

	virtual ~FaultRecoveryPoint() { }

	inline const char *getName() const { return name; }
	inline void setName(const char *name) { this->name = name; }

	inline int getState() const { return state; }
	inline void setState(int state) { this->state = state; }

	inline float getWatchdogTime() const { return wdt; }
	inline void setWatchdogTime(float wdt) { this->wdt = wdt; }

	inline void execute() { _function.call(); }

private:

	int state;
	float wdt;
	const char *name;
	FunctionPointer _function;
};

#endif /* FAULTRECOVERYPOINT_H_ */
