/*
 =======================================================================================================================
 File       : WeatherStationFT.h
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

#ifndef WEATHERSTATIONFT_H_
#define WEATHERSTATIONFT_H_

#include "mbed.h"

#include "WeatherStation.h"

#define FILEPATH_DATA_COPY_1				"/" FILESYSTEM_NAME "/data_c1.dat"
#define FILEPATH_DATA_COPY_2				"/" FILESYSTEM_NAME "/data_c2.dat"

class WeatherStationFT: public WeatherStation {
public:

	static const int SERIAL_NUMBER = 123456789;

	static const float SOFTWARE_VERSION = 0.1;

	static WeatherStationFT *getInstance();

//	WeatherStationFT(WeatherStationConfig *config = NULL);
//	virtual ~WeatherStationFT();

	/**
	 * Start weather station actions (readings, send)
	 */
	virtual void start();

	/**
	 * Destroy the WeatherStation object.
	 */
	void destroy();

private:

	typedef enum {
		STATE_NOT_CONFIGURED,
		STATE_CONFIGURED,
		STATE_READ_SENSORS,
		STATE_SAVE_DATA,
		STATE_DATA_SAVED,
		STATE_SEND_DATA
	} StationState;

//	LPC1768
//	char buf0[0x4000] __attribute__((section("AHBSRAM0"))); // USB
//	char buf1[0x4000] __attribute__((section("AHBSRAM1"))); // Ethernet

//	LPC11U24
//	char buf2[0x0800] __attribute__((at(0x20004000)));

	int state, state_copy_1, state_copy_2;

	ReadingData data_copy_1, data_copy_2;

	/**
	 * Private constructor
	 */
	WeatherStationFT();

	/**
	 * Destructor
	 */
	virtual ~WeatherStationFT();

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

	/**
	 * Checks if all data is consistent
	 */
	bool allDataIsConsistent();

	/**
	 * Averaging the readings.
	 *
	 * @param data
	 * @param n
	 * @param n2
	 * @param variation
	 *
	 * @return
	 */
	float avg(float data[], int n, int n2, float variation);
};

#endif /* WEATHERSTATIONFT_H_ */
