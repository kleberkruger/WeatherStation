/*
 =======================================================================================================================
 File       : WeatherStationConfig.cpp
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

#include <WeatherStationConfig.h>

WeatherStationConfig::WeatherStationConfig() : ConfigFile("/local/config.cfg") {
	// TODO Auto-generated constructor stub
}

WeatherStationConfig::~WeatherStationConfig() {
	// TODO Auto-generated destructor stub
}

bool WeatherStationConfig::checkTime(struct tm *tm) {
	return checkTime(tm->tm_hour, tm->tm_min, tm->tm_sec);
}

bool WeatherStationConfig::checkTime(const char *time_str) {
	int hour = -1;
	int min = -1;
	int sec = 0;

	sscanf(time_str, "%d:%d:%d", &hour, &min, &sec);

	return checkTime(hour, min, sec);
}

bool WeatherStationConfig::checkTime(int hour, int min, int sec) {
	if (hour < 0 || hour > 23)
		return false;
	if (min < 0 || min > 59)
		return false;
	if (sec < 0 || sec > 59)
		return false;

	return true;
}
