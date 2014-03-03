/*
 * WeatherStation2.cpp
 *
 *  Created on: 02/03/2014
 *      Author: Kleber
 */

#include <WeatherStation2.h>

WeatherStation2::WeatherStation2() {
	// TODO Auto-generated constructor stub
}

WeatherStation2::~WeatherStation2() {
	// TODO Auto-generated destructor stub
}

void WeatherStation2::config() {

	char *value;

	cfg.setWatchdogTime((value = cfg.getValue("watchdogTime")) ? atoff(value) : 0);
	cfg.setNumberOfReadings((value = cfg.getValue("numberOfReadings")) ? atoi(value) : 0);
	cfg.setMinCorrectReadings((value = cfg.getValue("minCorrectReadings")) ? atoi(value) : 0);
	cfg.setReadingInterval((value = cfg.getValue("readingInterval")) ? atoi(value) : 0);
//	cfg.setReadingUnit((value = cfg.getValue("readingUnit")) ? atoi(value) : 0);
}
