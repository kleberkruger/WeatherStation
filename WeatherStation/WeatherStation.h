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

#include "EthernetPowerControl.h"
#include "GPS.h"
#include "nRF24L01P.h"
#include "SHTx/sht15.hpp" // *Copyright (c) 2010 Roy van Dam <roy@negative-black.org> All rights reserved.
#include "Anemometer.h"
#include "Pluviometer.h"
#include "ReadingData.h"
#include "Utils.h"
#include "Watchdog.h"
#include "Wetting.h"

#define FAULTS_INJECTOR_MODE

#ifdef FAULTS_INJECTOR_MODE
#include "FaultInjector.h"
#endif

//#define SERIAL_DEBUG
//#define GPS_ENABLE

#define FILESYSTEM_NAME			"local"

#define FILEPATH_CONFIG			"/" FILESYSTEM_NAME "/config.cfg"
#define FILEPATH_LOG			"/" FILESYSTEM_NAME "/log.txt"
#define FILEPATH_DATA_1			"/" FILESYSTEM_NAME "/data_1.dat"
#define FILEPATH_DATA_2			"/" FILESYSTEM_NAME "/data_2.dat"
#define FILEPATH_DATA_3			"/" FILESYSTEM_NAME "/data_3.dat"
#define FILEPATH_READY			"/" FILESYSTEM_NAME "/ready"

#define SERIAL_NUMBER           123456
#define NUMBER_OF_PARAMETERS    9
#define NUMBER_OF_READINGS      8
#define CORRECT_READINGS        6

typedef enum {
	READ_UNIT_MIN, READ_UNIT_SEG
} ReadUnitType;

typedef enum {
	ACTION_READ, ACTION_SEND
} ActionType;

typedef enum {
	NO_ERROR = 0, ERROR_OPEN_FILE = 1, ERROR_READ_SENSOR = 2
} ErrorType;

typedef enum {
	STATE_NOT_CONFIGURED, STATE_CONFIGURED, STATE_READ_SENSORS, STATE_SAVE_DATA, STATE_DATA_SAVED, STATE_SEND_DATA
} StationState;

typedef union { // União para acessar os mesmos 4 bytes de memória como float e como long
	float f;
	long l;
} float_long;

class WeatherStation {
public:

	WeatherStation();
	virtual ~WeatherStation();

	void start();

private:

	static const double CONST_VBAT 	= 15.085714286; 	// = 3,3V*(Rinf+Rsup)/Rinf
	static const double INC_PLUV 	= 0.254; 			// Valor em mm de um pulso do pluviômetro
	static const int CONV_ANEM 		= 1; 				// Constante para converter tempo do pulso em ms para velocidade do vento (m/s)

	static const char READ_UNIT 	= READ_UNIT_SEG; 	// 'm' para mim e 's' para seg
	static const int READ_INTERVAL 	= 10; 				// Esse valor deve ser > 1
	static const int SEND_TIME_HOUR = 11;
	static const int SEND_TIME_MIN 	= 38;

	int state, state_copy_1, state_copy_2;
	ReadingData data, data_copy_1, data_copy_2;

	DigitalOut led1, led2, led3, led4;
	DigitalOut WDI, LDBATT;
	DigitalOut gpsPower;
	GPS gps;
	Serial pc;
	LocalFileSystem fs;
	Watchdog wdt;
	Ticker ticker;
	Pluviometer pluv;

#ifdef FAULTS_INJECTOR_MODE
	FaultInjector injector;
	Timeout timer;
#endif

	static float calculateAverage(float data[], int n, int n2, float variation);

	bool checkTime(ActionType action, int unit, int interval);

	void config();

	void configRTC();

	static int compare(const void *n1, const void *n2);

	void fatalError(ErrorType errorCode);

	void flashLed(DigitalOut led);

	void generateFaults();

	int getStateByVoting();

	void loadWatchdog();

	void log(const char *msg);

	bool readGPS();

	float readSensor(float v_ini_par, float v_ini_volts, float v_fim_par, float v_fim_volts, int num_pino);

	void readSensors();

	bool saveData();

	void send();

	void setState(int state);

	void writeConfigData();
};

#endif /* WEATHERSTATION_H_ */
