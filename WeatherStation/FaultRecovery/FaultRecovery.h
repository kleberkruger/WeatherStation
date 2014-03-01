/*
 =======================================================================================================================
 File       : FaultRecovery.h
 -----------------------------------------------------------------------------------------------------------------------
 Author     : Kleber Kruger
 Email      : kleberkruger@gmail.com
 Date       : 2014-02-05
 Version    : 1.0
 Copyright  : Faculty of Computing, FACOM - UFMS
 -----------------------------------------------------------------------------------------------------------------------
 Description: Fault Recovery Library
 =======================================================================================================================
 */

#ifndef FAULTRECOVERY_H_
#define FAULTRECOVERY_H_

#include "mbed.h"

#include "ConfigFile.h"
#include "DLinkedList.h"
#include "FaultRecoveryPoint.h"
#include "Logger.h"

/**
 * Execute function.
 *
 * @param function 	- function to execute
 * @param state		- application state
 */
#define EXECUTE(function, state)																\
	FaultRecovery::setState(state);																\
	function

/**
 * Execute function.
 *
 * @param function 	- function to execute
 * @param preState	- application state
 * @param postState	- next state if condition is satisfied
 * @param condition - condition
 */
#define EXECUTE_AND_NEXTSTATE(function, state, nextState, condition)							\
	FaultRecovery::setState(state);																\
	if (function == condition) FaultRecovery::setState(nextState)

/**
 * Try executing and go to next state if stop condition is satisfied.
 *
 * @param result
 * @param function
 * @param state
 * @param condition
 * @param n
 */
#define TRY_EXECUTING_N_TIMES(result, function, state, condition, n)							\
	FaultRecovery::setState(state);																\
	for (int _att = 1; ((result = function) && condition) && (_att <= n); _att++)

/**
 * Try executing and go to next state if stop condition is satisfied.
 *
 * @param function
 * @param state
 * @param nextState
 * @param condition
 * @param n
 */
#define TRY_EXECUTING_N_TIMES_AND_NEXTSTATE(result, function, state, nextState, condition, n)	\
	{																							\
		int _att;																				\
		FaultRecovery::setState(state);															\
		for (_att = 1; ((result = function) && condition) && (_att <= n); _att++);				\
		if (_att <= n) FaultRecovery::setState(state);											\
	}

/**
 * @param result 	- result
 * @param rdIntv 	- interval between readings
 * @param numAtt 	- number of attempts
 * @param function 	- reading function
 * @param numRd		- number of readings
 * @param mimCorrRd - minimum of correct readings
 * @param maxResVar	- maximum variation in result
 */
#define READ_SENSOR(result, rdIntv, numAtt, function, numRd, minCorrRd, maxResVar) 				\
	{																							\
		float _samples[numRd];																	\
		int _att = 0;																			\
		do {																					\
			for (int _i = 0; _i < numRd; _i++) {												\
				_samples[_i] = function;														\
				if (rdIntv > 0) wait_ms(rdIntv);												\
			}																					\
			result = FaultRecovery::avg(_samples, numRd, minCorrRd, maxResVar); 				\
		} while (isnan(result) && _att++ < numAtt);												\
	}

class FaultRecovery {
public:

	/**
	 * Constructor
	 */
	FaultRecovery();

	/**
	 * Constructor
	 *
	 * @param fs_name	- file system name
	 * @param cfg_path 	- configuration file path
	 * @param log_path	- log file path
	 */
	FaultRecovery(const char *fs_name, const char *config_path, const char *log_path);

	/**
	 * Destructor
	 */
	virtual ~FaultRecovery();

	/**
	 * Attach a function to be called by the Ticker, specifying the interval in micro-seconds.
	 *
	 * @param state - application state
	 * @param fptr 	- pointer to the function to be called
	 * @param wdt	- watchdog time
	 */
	void addState(int state, void (*fptr)(void), float wdt);

	/**
	 * Attach a member function to be called by the Ticker, specifying the interval in micro-seconds.
	 *
	 * @param state - application state
	 * @param tptr 	- pointer to the object to call the member function on
	 * @param mptr 	- pointer to the member function to be called
	 * @param wdt	- watchdog time
	 */
	template<typename T>
	void addState(int state, T* tptr, void (T::*mptr)(void), float wdt);

	/**
	 * Blink LED.
	 *
	 * @param pin
	 * @param count
	 * @param interval
	 */
	virtual void blinkLED(PinName pin, uint8_t count, int interval);

protected:

	LocalFileSystem fs;
	ConfigFile cfg;
	Logger logger;
	Timer state_timer;

	/**
	 * Restore application from state.
	 * @param state - state
	 */
	void goToState(int state);

	/**
	 * Get state by voting.
	 */
	int getStateByVoting();

	/**
	 * Set state.
	 * @param state - state
	 */
	void setState(int state);

	/**
	 * Calculate readings average.
	 *
	 * @param data 		- samples array
	 * @param n 		- samples amount
	 * @param n2		- minimum of correct samples
	 * @param variation - maximum variation in result
	 */
	static float avg(float data[], int n, int n2, float variation);

private:

	/* Store application state */
	int state_1, state_2, state_3;

	DLinkedList<FaultRecoveryPoint> states;

	/**
	 * Recovery application from current state.
	 */
	void recovery();

	virtual void init() = 0;
};

#endif /* FAULTRECOVERY_H_ */
