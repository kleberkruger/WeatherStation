/*
 =======================================================================================================================
 File       : WeatherStation.h
 -----------------------------------------------------------------------------------------------------------------------
 Author     : Kleber Kruger
 Email      : kleberkruger@gmail.com
 Reference  : Fábio Iaione
 Date       : 2013-06-11
 Version    : 1.0
 Copyright  : Faculty of Computing, FACOM - UFMS
 -----------------------------------------------------------------------------------------------------------------------
 Description: Weather station without implementing fault tolerance
 =======================================================================================================================
 */

#ifndef WEATHERSTATION_H_
#define WEATHERSTATION_H_

#include "mbed.h"

#include "ConfigFile.h"
#include "EthernetPowerControl.h"
#include "GPS.h"
#include "Logger.h"
#include "nRF24L01P.h"
#include "SHTx/sht15.hpp" // *Copyright (c) 2010 Roy van Dam <roy@negative-black.org> All rights reserved.#include "Anemometer.h"#include "Pluviometer.h"#include "ReadingData.h"#include "Utils.h"
#include "Watchdog.h"
#include "Wetting.h"

#define FAULTS_INJECTOR_MODE

#ifdef FAULTS_INJECTOR_MODE
#include "FaultInjector.h"
#endif

//#define SERIAL_DEBUG
//#define GPS_ENABLE

#define FILESYSTEM_NAME					"local"

#define FILEPATH_CONFIG					"/" FILESYSTEM_NAME "/config.cfg"
#define FILEPATH_LOG					"/" FILESYSTEM_NAME "/log.txt"
#define FILEPATH_DATA_1					"/" FILESYSTEM_NAME "/data_1.dat"
#define FILEPATH_DATA_2					"/" FILESYSTEM_NAME "/data_2.dat"
#define FILEPATH_DATA_3					"/" FILESYSTEM_NAME "/data_3.dat"
#define FILEPATH_READY					"/" FILESYSTEM_NAME "/ready"

#define SERIAL_NUMBER           		123456

typedef enum {
	READING_UNIT_SEC, READING_UNIT_MIN
} ReadingUnitType;

typedef enum {
	NO_ERROR = 0, ERROR_OPEN_FILE = 1, ERROR_READ_SENSOR = 2
} ErrorType;

typedef enum {
	POWER_OFF, POWER_ON
} PowerOpt;

typedef enum {
	STATE_NOT_CONFIGURED, STATE_CONFIGURED, STATE_READ_SENSORS, STATE_SAVE_DATA, STATE_DATA_SAVED, STATE_SEND_DATA
} StationState;

class WeatherStation {
public:

	WeatherStation();
	virtual ~WeatherStation();

	void start();

private:

	static const double CONST_VBAT = 15.085714286; 	// = 3,3V*(Rinf+Rsup)/Rinf

	static const int DEFAULT_READINGS_AMOUNT 		= 8;
	static const int DEFAULT_READINGS_MIN_CORRECT 	= 6;
	static const int DEFAULT_READINGS_INTERVAL 		= 15;

	static const int DEFAULT_SEND_TIME_HOUR = 11;
	static const int DEFAULT_SEND_TIME_MIN 	= 30;
	static const int DEFAULT_SEND_TIME_SEC 	= 00;

	/* Configuration Parameters */
	uint8_t numberReadings;
	uint8_t minCorrectReadings;

	unsigned int readingInterval;
	ReadingUnitType readingUnit;

	uint8_t sendTimeHour;
	uint8_t sendTimeMin;
	uint8_t sendTimeSec;

	float watchdogTime;

	int state, state_copy_1, state_copy_2;
	ReadingData data, data_copy_1, data_copy_2;

	DigitalOut led1, led2, led3, led4;
	DigitalOut WDI, LDBATT;
	DigitalOut gpsPower;
	GPS gps;
	Serial pc;
	LocalFileSystem fs;
	ConfigFile cfg;
	Logger logger;
	Watchdog wdt;
	Ticker ticker;
	Timer state_timer;
	Pluviometer pluv;

#ifdef FAULTS_INJECTOR_MODE
	FaultInjector injector;
	Timeout timer;
#endif

	void blinkLED(PinName pin, uint8_t count, int interval);

	static float calculateAverage(float data[], int n, int n2, float variation);

	void config();

	void configRTC();

	static int compare(const void *n1, const void *n2);

	void fatalError(ErrorType errorCode);

	void generateFaults();

	int getStateByVoting();

	void goToState(int state);

	void init();

	bool isTimeToRead();
	bool isTimeToSend();

	void loadWatchdog();

	void powerMbed(PowerOpt action);
	void powerBattery(PowerOpt action);
	void powerGPS(PowerOpt action);
	void powerLED(PowerOpt action, PinName pin);

	bool readGPS();

	float readSensor(float v_ini_par, float v_ini_volts, float v_fim_par, float v_fim_volts, int num_pino);

	void readSensors();

	bool saveData();

	void send();

	void setState(int state);

	void writeConfigData();
};

#endif /* WEATHERSTATION_H_ */
