/*
 =======================================================================================================================
 File       : WeatherStation.h
 -----------------------------------------------------------------------------------------------------------------------
 Author     : Kleber Kruger
 Email      : kleberkruger@gmail.com
 Date       : 2014-03-01
 Version    : 1.0
 Copyright  : Faculty of Computing, FACOM - UFMS
 -----------------------------------------------------------------------------------------------------------------------
 Description: Weather station with implementing fault tolerance
 =======================================================================================================================
 */

#include "WeatherStation.h"

#include "mbed.h"

void test(PinName pin) {

	Serial pc(USBTX, USBRX);
	DigitalOut led(pin);

	pc.printf("Start!\n");

	for (int i = 0; i < 10; i++) {
		led = 1;
		wait_ms(200);
		led = 0;
		wait_ms(200);
	}

	pc.printf("Finished!\n");
}

int main() {

//	WeatherStation station;
//	station.start();

	WeatherStation::getInstance()->start();

	return 0;
}
