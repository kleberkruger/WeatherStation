/*
 =======================================================================================================================
 File       : Watchdog.cpp
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

#include "Watchdog.h"

Watchdog::Watchdog() {

	/* Nothing to do */
}

Watchdog::~Watchdog() {

	/* Nothing to do */
}

void Watchdog::kick(float s) {
	LPC_WDT ->WDCLKSEL = 0x1; 				// Set CLK src to PCLK
	uint32_t clk = SystemCoreClock / 16; 	// WD has a fixed /4 prescaler, PCLK default is /4
	LPC_WDT ->WDTC = s * (float) clk;
	LPC_WDT ->WDMOD = 0x3; 					// Enabled and Reset
	kick();
}

/**
 * "kick" or "feed" the dog - reset the watchdog timer
 * by writing this required bit pattern
 */
void Watchdog::kick(void) {
	LPC_WDT ->WDFEED = 0xAA;
	LPC_WDT ->WDFEED = 0x55;
}
