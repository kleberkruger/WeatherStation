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
#include "SHTx/sht15.hpp" // *Copyright (c) 2010 Roy van Dam <roy@negative-black.org> All rights reserved.#include "Anemometer.h"#include "Pluviometer.h"#include "ReadingData.h"#include "Watchdog.h"#include "WeatherStationConfig.h"#include "Wetting.h"

#include "FaultInjector.h"

/*----------------------------------------------------------------------------------------------------------------------
 Weather Station Modules
 ---------------------------------------------------------------------------------------------------------------------*/
#define FAULT_TOLERANCE_ENABLED 			// (uncomment to enable).
//#define FAULT_INJECTION_ENABLED 			// (uncomment to enable).
/*--------------------------------------------------------------------------------------------------------------------*/
#ifdef FAULT_INJECTION_ENABLED
#define FAULT_INJECTION_IN_MEMORY_ENABLED	// (uncomment to enable).´
#define FAULT_INJECTION_IN_SENSOR_ENABLED 	// (uncomment to enable).
#endif
/*--------------------------------------------------------------------------------------------------------------------*/

#define FILESYSTEM_NAME					"local"

#define FILEPATH_CONFIG					"/" FILESYSTEM_NAME "/config.cfg"
#define FILEPATH_LOG					"/" FILESYSTEM_NAME "/log.txt"
#define FILEPATH_DATA					"/" FILESYSTEM_NAME "/data.dat"
#define FILEPATH_INFO					"/" FILESYSTEM_NAME "/info.txt"
#define FILEPATH_READY					"/" FILESYSTEM_NAME "/ready"

class WeatherStation {
public:

	static const int SERIAL_NUMBER = 123456789;

	static const float SOFTWARE_VERSION = 0.1;

	/**
	 * Get an instance of WeatherStation object.
	 * Singleton Pattern.
	 */
	static WeatherStation *getInstance();

//	WeatherStation(WeatherStationConfig *config = NULL);
//	virtual ~WeatherStation();

	/**
	 * Start weather station actions (readings, send).
	 */
	virtual void start();

	/**
	 * Destroy the WeatherStation object.
	 */
	virtual void destroy();

	inline const Logger& getLogger() const {
		return logger;
	}

protected:

	typedef enum {
		POWER_OFF, POWER_ON
	} PowerOpt;

	typedef enum {
		NO_ERROR = 0, ERROR_OPEN_FILE = -1, ERROR_READ_SENSOR = -2
	} ErrorType;

	LocalFileSystem fs;
	Logger logger;
	WeatherStationConfig cfg;
	Watchdog wdt;
	Ticker weak;
	Pluviometer pluv;
	GPS gps;

	ReadingData data;

#ifdef FAULT_INJECTION_IN_MEMORY_ENABLED
	FaultInjector injector;
#endif

	/**
	 * Private constructor.
	 */
	WeatherStation();

	/**
	 * Private destructor.
	 */
	virtual ~WeatherStation();

	/**
	 * Initialize weather station.
	 */
	virtual void init();

	/**
	 * Configure weather station.
	 */
	virtual void config();

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
	virtual void readSensors();

	/**
	 * Read GPS.
	 */
	virtual bool readGPS();

	/**
	 * Save data
	 */
	virtual bool saveData();

	/**
	 * Send data
	 */
	virtual bool send();

	/**
	 * Blink LED.
	 *
	 * @param pin
	 * @param count
	 * @param interval
	 */
	void blinkLED(PinName pin, uint8_t count, int interval);

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
	 * Called when a fatal error occurs.
	 * XXX: really necessary?
	 */
	void fatalError(ErrorType error);

	/**
	 * Print configuration parameters.
	 * XXX: really necessary?
	 */
	void printConfigInfo();

	/**
	 * Print readings parameters.
	 * XXX: really necessary?
	 */
	void printDataInfo(ReadingData *data, const char *prefix);
};

#endif /* WEATHERSTATION_H_ */
