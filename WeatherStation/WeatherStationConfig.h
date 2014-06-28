/*
 =======================================================================================================================
 File       : WeatherStationConfig.h
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

#ifndef WEATHERSTATIONCONFIG_H_
#define WEATHERSTATIONCONFIG_H_

#include "mbed.h"

#include "ConfigList.h"
//#include "WeatherStation.h"

class WeatherStationConfig: public ConfigList {
public:

	typedef enum {
		READING_UNIT_SEC, READING_UNIT_MIN
	} ReadingUnitType;

	typedef enum {
		UNIX, MAC, DOS
	} FileFormat;

	WeatherStationConfig();
	virtual ~WeatherStationConfig();

	/**
	 * Save data in the file.
	 *
	 * @param file 		- file path
	 * @param header 	- file header
	 * @param format	- file format
	 *
	 * @return 			- true if it succeed
	 */
	bool saveToFile(const char *file, const char *header = NULL, FileFormat format = UNIX);

	/**
	 * Load data from file.
	 *
	 * @param file 	- file path
	 * @return 		- true if it succeed
	 */
	bool loadFromFile(const char *file);

	/**
	 * Get numberOfReadings
	 */
	uint8_t getNumberOfReadings() const {
		return numberReadings;
	}

//	/**
//	 * Set numberOfReadings
//	 */
//	void setNumberOfReadings(uint8_t n) {
//		numberReadings = (n > 0) ? n : DEFAULT_READINGS_AMOUNT;
//		setMinCorrectReadings(minCorrectReadings);
//	}

	/**
	 * Get minCorrectReadings
	 */
	uint8_t getMinCorrectReadings() const {
		return minCorrectReadings;
	}

//	/**
//	 * Set minCorrectReadings
//	 */
//	void setMinCorrectReadings(uint8_t min) {
//		minCorrectReadings = (min >= getLowerBound() && min <= getUpperBound()) ? min : getDefaultMinCorrectReadings();
//	}

	/**
	 * Set reading configuration.
	 *
	 * @param num 	- number of readings
	 * @param min 	- minimum of correct readings
	 *
	 * @return 		- true if it succeed
	 */
	bool setReadingConfig(uint8_t num, uint8_t min) {

		if (num < 1 || min < getLowerBound(num) || min > getUpperBound(num))
			return false;

		/* Set numberReadings and minCorrectReadings values */
		numberReadings = num;
		minCorrectReadings = min;

		return true;
	}

	/**
	 * Get readingUnit
	 */
	ReadingUnitType getReadingUnit() const {
		return readingUnit;
	}

	/**
	 * Get readingInterval
	 */
	float getReadingInterval() const {
		return readingInterval;
	}

	/**
	 * Set readingInterval
	 */
	bool setReadingInterval(ReadingUnitType unit, float interval) {

		/*
		 * It corrects for the minimum and maximum values ​​allowed.
		 * (this if can be removed).
		 */
		if (!isnan(interval) && interval > 0) {
			if (unit == READING_UNIT_MIN) {
				if (interval < 0.1)	interval = 0.1;
				else if (interval > 1440) interval = 1440;
			} else {
				if (interval < 1.0) interval = 1.0;
				else if (interval > 86400.0) interval = 86400.0;
			}
		}

		if (unit == READING_UNIT_MIN)
			interval *= 60;

		if (interval < 1.0 || interval > 86400.0)
			return false;

		readingUnit = unit;
		readingInterval = interval;

		return true;
	}

	/**
	 * Get formated sendTime
	 */
	const char *getFormatedTime() const {
		strftime((char *) tmStr, 16, "%T", &sendTime);
		return tmStr;
	}

	/**
	 * Get sendTime
	 */
	const struct tm& getSendTime() const {
		return sendTime;
	}

	/**
	 * Set sendTime
	 */
	bool setSendTime(const struct tm& tm) {

		if (!checkTime(&tm))
			return false;

		sendTime.tm_hour = tm.tm_hour;
		sendTime.tm_min = tm.tm_min;
		sendTime.tm_sec = tm.tm_sec;

		return true;
	}

	/**
	 * Set sendTime
	 */
	bool setSendTime(const char *timeStr) {

		if (!checkTime(timeStr))
			return false;

		sscanf(timeStr, "%d:%d:%d", &sendTime.tm_hour, &sendTime.tm_min, &sendTime.tm_sec);
		return true;
	}

	/**
	 * Get watchdogTime
	 */
	float getWatchdogTime() const {
		return watchdogTime;
	}

	/**
	 * Set watchdogTime
	 */
	bool setWatchdogTime(float wdt) {

		/*
		 * It corrects for the minimum and maximum values ​​allowed.
		 * (this if can be removed).
		 */
		if (!isnan(wdt) && wdt > 0) {
			if (wdt < 2.0)
				wdt = 2.0;
			else if (wdt > 3600)
				wdt = 3600;
		}

		if (wdt < 2.0 || wdt > 3600)
			return false;

		watchdogTime = wdt;
		return true;
	}

	void setNumberOfReadings(uint8_t n) {
		numberReadings = n;
	}

	void setMinCorrectReadings(uint8_t n) {
		minCorrectReadings = n;
	}

private:

	static const char* const NEWLINE_UNIX;
	static const char* const NEWLINE_DOS;
	static const char* const NEWLINE_MAC;

	static const char* const CONFIG_HEADER_TXT;							// # Weather station with implementing fault tolerance
	static const char* const DEFAULT_SEND_TIME;							// 11:00:00

	static const int DEFAULT_READINGS_AMOUNT 		= 4;				// Default number of readings
	static const int DEFAULT_READINGS_MIN_CORRECT 	= 75; 				// Default minimum correct readings (in percentage) - (75%)

//#ifdef FAULT_TOLERANCE_ENABLED
//	static const int DEFAULT_READINGS_INTERVAL 		= 10;				// Default interval of the readings
//#else
//	static const int DEFAULT_READINGS_INTERVAL 		= 5;				// Default interval of the readings
//#endif

	static const int DEFAULT_READINGS_INTERVAL 		= 10;				// Default interval of the readings
	static const int DEFAULT_READINGS_UNIT 			= READING_UNIT_SEC; // (Default in seconds)
	static const float DEFAULT_WATCHDOG_TIME 		= DEFAULT_READINGS_INTERVAL * 2; // Default watchdog time (in seconds)

	uint8_t numberReadings;												// Number of readings
	uint8_t minCorrectReadings;											// Minimum correct readings
	float readingInterval; 												// Interval of the readings (in seconds)
	ReadingUnitType readingUnit;										// Reading Unit (seconds or minutes)
	struct tm sendTime;													// Send time
	char tmStr[16];														// Send time formated (string)
	float watchdogTime;													// Watchdog time

	void loadFromList();
	void saveToList();

	ReadingUnitType convertUnit(const char *value);

	/**
	 * Get default minimum of correct readings.
	 */
	inline uint8_t getDefaultMinCorrectReadings() const {
		return (uint8_t) ((DEFAULT_READINGS_AMOUNT * DEFAULT_READINGS_MIN_CORRECT) / 100);
	}

	/**
	 * Get lower-bound of minimum correct readings.
	 */
	inline uint8_t getLowerBound(uint8_t n) const {
		return (uint8_t) ((n * 50) / 100) + 1;
	}

	/**
	 * Get lower-bound of minimum correct readings.
	 */
	inline uint8_t getLowerBound() const {
		return getLowerBound(numberReadings);
	}

	/**
	 * Get upper-bound of minimum correct readings.
	 */
	inline uint8_t getUpperBound(uint8_t n) const {
		return n;
	}

	/**
	 * Get upper-bound of minimum correct readings.
	 */
	inline uint8_t getUpperBound() const {
		return getUpperBound(numberReadings);
	}

	/**
	 * Resets to the default send time.
	 */
	inline void resetToDefaultSendTime() {
		sscanf(DEFAULT_SEND_TIME, "%d:%d:%d", &sendTime.tm_hour, &sendTime.tm_min, &sendTime.tm_sec);
	}

	/**
	 * Checks if informed time is valid.
	 *
	 * @param hour 	- hour
	 * @param min 	- minute
	 * @param sec 	- second
	 */
	static bool checkTime(int hour, int min, int sec);

	/**
	 * Checks if informed time is valid.
	 *
	 * @param t
	 */
	static bool checkTime(const char *t);

	/**
	 * Checks if informed time is valid.
	 *
	 * @param t
	 */
	static bool checkTime(const struct tm *t);
};

#endif /* WEATHERSTATIONCONFIG_H_ */
