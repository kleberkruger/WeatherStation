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

int main() {

	WeatherStation station;
	station.start();

	return 0;
}
