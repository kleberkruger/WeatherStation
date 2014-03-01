/*
 =======================================================================================================================
 File       : FaultRecovery.cpp
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

#include "FaultRecovery.h"

static inline int compare(const void *n1, const void *n2);

FaultRecovery::FaultRecovery() :
		fs("local"), cfg("/local/config.txt"), logger("/local/log.txt") {

	recovery(); // Recovery from reset by watchdog
}

FaultRecovery::FaultRecovery(const char *fs_name, const char *log_path, const char *cfg_path) :
		fs(fs_name), cfg(cfg_path), logger(log_path) {

	recovery(); // recovery from reset by watchdog
}

FaultRecovery::~FaultRecovery() {

	/* Nothing to do */
}

void FaultRecovery::recovery() {

	/* Reset by watchdog */
	if ((LPC_WDT->WDMOD >> 2) & 1) {

		logger.log("Reset by watchdog.");

		/* Blink LED 2 */
		blinkLED(LED2, 10, 100);

		/* Restore to state */
		goToState(atoi(cfg.getValue("state")));

	} else {

		logger.log("Reset by power-button.");

		/* Blink LED 1 */
		blinkLED(LED1, 10, 100);

		/* Configure mbed */
		init();
	}
}

void FaultRecovery::goToState(int state) {

	logger.log("goToState() - restore from state %d.", state);

	FaultRecoveryPoint *frp;

	for (unsigned int i = 0; i < states.size(); i++) {

		frp = states.get(i);

		if (frp->getState() == state) {
			frp->execute();
			break;
		}
	}

//	switch (state) {
//
//		case STATE_NOT_CONFIGURED:
//		case STATE_CONFIGURED:
//			config();
//			break;
//
//		case STATE_READ_SENSORS:
//		case STATE_SAVE_DATA:
//			config();
//			readSensors();
//			break;
//
//		case STATE_DATA_SAVED:
//			config();
//			break;
//
//		case STATE_SEND_DATA:
//			config();
//			send();
//			break;
//
//		default:
//			config();
//			break;
//	}
}

int FaultRecovery::getStateByVoting() {

	if (state_1 == state_2)
		state_3 = state_1; // restore correct state
	else if (state_1 == state_3)
		state_2 = state_1; // restore correct state
	else if (state_2 == state_3)
		state_1 = state_2; // restore correct state
	else
		return -1;

	return state_1;
}

void FaultRecovery::setState(int state) {

	/* Get state by voting */
	if (state != getStateByVoting()) {

		char state_str[8];

		state_timer.stop();
		logger.log("State %d: %.3f", state, state_timer.read());

		state_timer.reset();
		state_timer.start();

		/* Clean string and print state */
		memset(state_str, 0, sizeof(char) * 8);
		sprintf(state_str, "%d", state);

		/* Write a configuration value. */
		cfg.setValue("state", state_str);

		/* Write a configuration file to a mbed. */
//		cfg.write(cfg_path, "# Weather station with implementing fault tolerance.");
		cfg.save();

		state_1 = state_2 = state_3 = state;

		logger.log("set state: %d.", state);
	}
}

void FaultRecovery::addState(int state, void (*fptr)(void), float wdt) {
	states.add(new FaultRecoveryPoint(state, fptr, wdt));
}

template<typename T>
void FaultRecovery::addState(int state, T* tptr, void (T::*mptr)(void), float wdt) {
	states.add(new FaultRecoveryPoint(state, tptr, mptr, wdt));
}

void FaultRecovery::blinkLED(PinName pin, uint8_t count, int interval) {

	if (pin == LED1 || pin == LED2 || pin == LED3 || pin == LED4) {

		DigitalOut led(pin);

		for (uint8_t i = 0; i < count; i++) {
			led = 1;
			wait_ms(interval);
			led = 0;
			wait_ms(interval);
		}
	}
}

float FaultRecovery::avg(float data[], int n, int n2, float variation) {

	int i, j, left, right, lc;
	float result = 0;

	qsort(data, n, sizeof(int), compare);

	// Mínimo 50% de chances...
	left = right = n / 2;
	lc = 1;

	for (i = left; i >= 0; i--) {
		for (j = i + 1; j < n && data[j] <= data[i] + (variation * 2); j++)
			;
		if (j - i > lc) {
			lc = j - i;
			left = i;
			right = (j - 1); // A última comparação é falsa.
		}
	}

	if (lc < n2)
		return NAN;

	for (i = left; i <= right; i++)
		result += data[i];

	result = result / lc;

	return result;
}

static inline int compare(const void *n1, const void *n2) {
	return (*(float*) n1 - *(float*) n2);
}
