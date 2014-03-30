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

#ifndef WEATHERSTATION_H_
#define WEATHERSTATION_H_

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
#include "SHTx/sht15.hpp" // *Copyright (c) 2010 Roy van Dam <roy@negative-black.org> All rights reserved.#include "Anemometer.h"#include "Pluviometer.h"#include "ReadingData.h"
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
#define FILEPATH_INFO					"/" FILESYSTEM_NAME "/info.txt"

#ifdef FAULT_INJECTOR_ENABLE
#define FILEPATH_READY					"/" FILESYSTEM_NAME "/ready"
#endif

class WeatherStation {
public:

	static const int SERIAL_NUMBER = 123456789;

	WeatherStation(WeatherStationConfig *config = NULL);
	virtual ~WeatherStation();

	/**
	 * Start weather station actions (readings, send)
	 */
	void start();

private:

	typedef enum {
		POWER_OFF, POWER_ON
	} PowerOpt;

	typedef enum {
		NO_ERROR = 0, ERROR_OPEN_FILE = 1, ERROR_READ_SENSOR = 2
	} ErrorType;

	typedef enum {
		STATE_NOT_CONFIGURED, STATE_CONFIGURED, STATE_READ_SENSORS, STATE_SAVE_DATA, STATE_DATA_SAVED, STATE_SEND_DATA
	} StationState;

	int state, state_copy_1, state_copy_2;
	ReadingData data, data_copy_1, data_copy_2;

	LocalFileSystem fs;
	Logger logger;
	WeatherStationConfig cfg;
	Watchdog wdt;
	Ticker weak;
	Pluviometer pluv;
	GPS gps;

#ifdef FAULT_INJECTOR_ENABLE
	FaultInjector injector;
#endif

	/**
	 * Initialize weather station.
	 */
	void init();

	/**
	 * Get state by voting.
	 */
	int getStateByVoting();

	/**
	 * Set state.
	 * @param state - state
	 */
	void setState(int state);

	/**
	 * Restore application from state.
	 * @param state - state
	 */
	void goToState(int state);

	/**
	 * Configure weather station.
	 */
	void config();

	/**
	 * Configure timer.
	 */
	void configTimer();

	/**
	 * Save (record on disk) information file.
	 */
	bool saveInfoFile();

	/**
	 * Reload watchdog time.
	 */
	void reloadWatchdog();

	/**
	 * Checks if it's time to read.
	 */
	bool isTimeToRead();

	/**
	 * Checks if it's time to send.
	 */
	bool isTimeToSend();

	void printDataInfo(ReadingData *data, const char *prefix);

	/**
	 * Read sensor.
	 *
	 * @param pin_num
	 * @param v_start_par
	 * @param v_start_volts
	 * @param v_end_par
	 * @param v_end_volts
	 */
	float readSensor(int pin_num, float v_start_par, float v_start_volts, float v_end_par, float v_end_volts);

	/**
	 * Read all sensors.
	 */
	void readSensors();

	/**
	 * Read GPS.
	 */
	bool readGPS();

	/**
	 * Save data
	 */
	bool saveData();

	/**
	 * Send data
	 */
	bool send();

	void fatalError(ErrorType error);

	/**
	 * Power on/off mbed.
	 */
	void powerMbed(PowerOpt action);

	/**
	 * Power on/off battery.
	 */
	void powerBattery(PowerOpt action);

	/**
	 * Power on/off GPS.
	 */
	void powerGPS(PowerOpt action);

	/**
	 * Power on/off LED.
	 */
	void powerLED(PowerOpt action, PinName pin);

	/**
	 * Blink LED.
	 *
	 * @param pin
	 * @param count
	 * @param interval
	 */
	void blinkLED(PinName pin, uint8_t count, int interval);

	static float avg(float data[], int n, int n2, float variation);
};

#endif /* WEATHERSTATION_H_ */
