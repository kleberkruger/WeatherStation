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

#include "ConfigFile.h"

class WeatherStationConfig: public ConfigFile {
public:

	typedef enum {
		READING_UNIT_SEC, READING_UNIT_MIN
	} ReadingUnitType;

	WeatherStationConfig();
	virtual ~WeatherStationConfig();

	bool load();
	bool save();

	inline uint8_t getMinCorrectReadings() const {
		return minCorrectReadings;
	}
	inline void setMinCorrectReadings(uint8_t min) {
		minCorrectReadings = (min >= getLowerBound() && min <= getUpperBound()) ? min : getDefaultMinCorrectReadings();
	}

	inline uint8_t getNumberOfReadings() const {
		return numberReadings;
	}
	inline void setNumberOfReadings(uint8_t n) {
		numberReadings = (n > 0) ? n : DEFAULT_READINGS_AMOUNT;
		setMinCorrectReadings(minCorrectReadings);
	}

	inline unsigned int getReadingInterval() const {
		return readingInterval;
	}
	inline void setReadingInterval(unsigned int readingInterval) {
		/* XXX */
		this->readingInterval = (readingInterval > 0) ?	readingInterval : DEFAULT_READINGS_INTERVAL;
	}

	inline ReadingUnitType getReadingUnit() const {
		return readingUnit;
	}
	inline void setReadingUnit(ReadingUnitType readingUnit) {
		this->readingUnit = readingUnit;
	}

	inline const struct tm& getSendTime() const {
		return sendTime;
	}
	inline void setSendTime(const char *sendTime) {
		const char *tm_str = (checkTime(sendTime)) ? sendTime : DEFAULT_SEND_TIME;
		sscanf(tm_str, "%d:%d:%d", &this->sendTime.tm_hour, &this->sendTime.tm_min, &this->sendTime.tm_sec);
	}
//	inline void setSendTime(const struct tm& sendTime) {
//
//		if (checkTime(&sendTime)) {
//
//			this->sendTime.tm_hour = sendTime.tm_hour;
//			this->sendTime.tm_min = sendTime.tm_min;
//			this->sendTime.tm_sec = sendTime.tm_sec;
//
//		} else {
//			sscanf(DEFAULT_SEND_TIME, "%d:%d:%d", &this->sendTime.tm_hour, &this->sendTime.tm_min,
//					&this->sendTime.tm_sec);
//		}
//	}

	inline float getWatchdogTime() const {
		return watchdogTime;
	}
	inline void setWatchdogTime(float watchdogTime) {
		this->watchdogTime = watchdogTime;
	}

private:

	/**
	 * Checks if informed time is valid.
	 *
	 * @param t
	 */
	static bool checkTime(struct tm *t);

	/**
	 * Checks if informed time is valid.
	 *
	 * @param t
	 */
	static bool checkTime(const char *t);

	/**
	 * Checks if informed time is valid.
	 *
	 * @param hour 	- hour
	 * @param min 	- minute
	 * @param sec 	- second
	 */
	static bool checkTime(int hour, int min, int sec);

	static const int DEFAULT_READINGS_AMOUNT = 8;
	static const int DEFAULT_READINGS_MIN_CORRECT = 80; /* in percentage */
	static const int DEFAULT_READINGS_INTERVAL = 15;
	static const int DEFAULT_WATCHDOG_TIME = 5;

	static const char* CONFIG_HEADER_TXT;

	static const char* DEFAULT_SEND_TIME;

	uint8_t numberReadings;
	uint8_t minCorrectReadings;

	unsigned int readingInterval;
	ReadingUnitType readingUnit;

	struct tm sendTime;

	float watchdogTime;

	/**
	 * Get default minimum of correct readings.
	 */
	inline uint8_t getDefaultMinCorrectReadings() const {
		return (uint8_t) ((DEFAULT_READINGS_AMOUNT * DEFAULT_READINGS_MIN_CORRECT) / 100);
	}

	/**
	 * Get lower-bound of minimum correct readings.
	 */
	inline uint8_t getLowerBound() const {
		return (uint8_t) ((DEFAULT_READINGS_AMOUNT * 50) / 100);
	}

	/**
	 * Get upper-bound of minimum correct readings.
	 */
	inline uint8_t getUpperBound() const {
		return numberReadings;
	}
};

#endif /* WEATHERSTATIONCONFIG_H_ */
