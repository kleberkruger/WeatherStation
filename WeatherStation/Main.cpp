/*
 =======================================================================================================================
 File       : WeatherStation.h
 -----------------------------------------------------------------------------------------------------------------------
 Author     : Kleber Kruger
 Email      : kleberkruger@gmail.com
 Date       : 2013-06-11
 Version    : 1.0
 Copyright  : Faculty of Computing, FACOM - UFMS
 -----------------------------------------------------------------------------------------------------------------------
 Description: Weather station without implementing fault tolerance
 =======================================================================================================================
 */

#include "WeatherStation.h"

int main() {

	WeatherStation station;
	station.start();

	return 0;
}
