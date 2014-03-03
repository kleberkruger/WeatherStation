/*
 =======================================================================================================================
 File       : WeatherStation.h
 -----------------------------------------------------------------------------------------------------------------------
 Author     : Kleber Kruger
 Email      : kleberkruger@gmail.com
 Reference  : Fábio Iaione
 Date       : 2014-03-01
 Version    : 1.0
 Copyright  : Faculty of Computing, FACOM - UFMS
 -----------------------------------------------------------------------------------------------------------------------
 Description: Weather station with implementing fault tolerance
 =======================================================================================================================
 */

#ifndef WEATHERSTATION2_H_
#define WEATHERSTATION2_H_

/*======================================================================================================================
 Enabled Modules. (Comment to disable).
 ---------------------------------------------------------------------------------------------------------------------*/
#define GPS_ENABLE
#define FAULT_INJECTOR_ENABLE
/*====================================================================================================================*/

#include "mbed.h"

#include "ConfigFile.h"
#include "EthernetPowerControl.h"
#include "GPS.h"
#include "Logger.h"
#include "nRF24L01P.h"
#include "SHTx/sht15.hpp" // *Copyright (c) 2010 Roy van Dam <roy@negative-black.org> All rights reserved.
#include "Anemometer.h"
#include "Pluviometer.h"
#include "ReadingData.h"
#include "Watchdog.h"
#include "WeatherStationConfig.h"
#include "Wetting.h"

#ifdef FAULT_INJECTOR_ENABLE
#include "FaultInjector.h"
#endif

#define FILESYSTEM_NAME					"local"

#define FILEPATH_CONFIG					"/" FILESYSTEM_NAME "/config.cfg"
#define FILEPATH_LOG					"/" FILESYSTEM_NAME "/log.txt"
#define FILEPATH_DATA_1					"/" FILESYSTEM_NAME "/data_1.dat"
#define FILEPATH_DATA_2					"/" FILESYSTEM_NAME "/data_2.dat"
#define FILEPATH_DATA_3					"/" FILESYSTEM_NAME "/data_3.dat"

#ifdef FAULT_INJECTOR_ENABLE
#define FILEPATH_READY					"/" FILESYSTEM_NAME "/ready"
#endif

class WeatherStation2 {
public:

	typedef enum {
		READING_UNIT_SEC, READING_UNIT_MIN
	} ReadingUnitType;

	WeatherStation2();
	virtual ~WeatherStation2();

private:

	WeatherStationConfig cfg;

	void config();
};

#endif /* WEATHERSTATION2_H_ */
