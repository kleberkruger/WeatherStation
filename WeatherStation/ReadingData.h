/*
 =======================================================================================================================
 File       : ReadingData.h
 -----------------------------------------------------------------------------------------------------------------------
 Author     : Kleber Kruger
 Email      : kleberkruger@gmail.com
 Date       : 2014-02-02
 Version    : 1.0
 Copyright  : Faculty of Computing, FACOM - UFMS
 -----------------------------------------------------------------------------------------------------------------------
 Description: Weather station with implementing fault tolerance
 =======================================================================================================================
 */

#ifndef READINGDATA_H_
#define READINGDATA_H_

#include "mbed.h"

#include "Serializable.h"

#define PARSE_TIME(fmt) { struct tm *info = localtime((time_t *) &tm); strftime((char *) tmStr, 32, fmt, info); }

class ReadingData {
public:

	static enum {

		INDEX_ANEMOMETER,
		INDEX_PLUVIOMETER,
		INDEX_WETTING,
		INDEX_TEMPERATURE,
		INDEX_HUMIDITY,
		INDEX_SOIL_TEMPERATURE,
		INDEX_SOIL_HUMIDITY,
		INDEX_SOLAR_RADIATION,
		INDEX_BATTERY_VOLTAGE,

		NUMBER_OF_PARAMETERS // (last item).

	} ParamIndex;

	/**
	 * Creates a new reading data.
	 */
	ReadingData();

	/**
	 * Destroy the reading data.
	 */
	virtual ~ReadingData();

	/**
	 * Creates a new reading data.
	 *
	 * @param data_1
	 * @param data_2
	 * @param data_3
	 */
	static ReadingData* create(ReadingData *data_1, ReadingData *data_2, ReadingData *data_3);

    /**
     * Load the reading data from file.
     *
     * @param file
     */
    static ReadingData* load(const char *file);

    /**
     * Save reading data in the file.
     *
     * @param file
     */
    bool save(const char *file);

	/**
	 * Calculates CRC (Cyclic Redundancy Check)
	 */
    uint32_t calculateCRC();

	/**
	 * Checks CRC (Cyclic Redundancy Check)
	 */
    bool checkCRC();

    /**
     * Checks CRC (Cyclic Redundancy Check)
     */
	bool checkCRC(uint32_t crc);

	inline int32_t getTime() const { return tm; }
	inline void setTime(int32_t tm) { this->tm = tm; }

	inline const char *getFormatedTime() const { return getFormatedTime("%F %T"); }
	inline const char *getFormatedTime(const char *fmt) const { PARSE_TIME(fmt) return tmStr; }

	inline float getAnemometer() const { return paramValues[INDEX_ANEMOMETER]; }
	inline void setAnemometer(float anemometer) { paramValues[INDEX_ANEMOMETER] = anemometer; }

	inline float getPluviometer() const { return paramValues[INDEX_PLUVIOMETER]; }
	inline void setPluviometer(float pluviometer) {	this->paramValues[INDEX_PLUVIOMETER] = pluviometer; }

	inline float getWetting() const { return paramValues[INDEX_WETTING]; }
	inline void setWetting(float wetting) {	paramValues[INDEX_WETTING] = wetting; }

	inline float getTemperature() const { return paramValues[INDEX_TEMPERATURE]; }
	inline void setTemperature(float temperature) {	paramValues[INDEX_TEMPERATURE] = temperature; }

	inline float getHumidity() const { return paramValues[INDEX_HUMIDITY]; }
	inline void setHumidity(float humidity) { paramValues[INDEX_HUMIDITY] = humidity; }

	inline float getSoilTemperaure() const { return paramValues[INDEX_SOIL_TEMPERATURE]; }
	inline void setSoilTemperaure(float soilTemperaure) { paramValues[INDEX_SOIL_TEMPERATURE] = soilTemperaure; }

	inline float getSoilHumidity() const { return paramValues[INDEX_SOIL_HUMIDITY]; }
	inline void setSoilHumidity(float soilHumidity) { paramValues[INDEX_SOIL_HUMIDITY] = soilHumidity; }

	inline float getSolarRadiation() const { return paramValues[INDEX_SOLAR_RADIATION]; }
	inline void setSolarRadiation(float solarRadiation) { paramValues[INDEX_SOLAR_RADIATION] = solarRadiation; }

	inline float getBatteryVoltage() const { return paramValues[INDEX_BATTERY_VOLTAGE]; }
	inline void setBatteryVoltage(float batteryVoltage) { paramValues[INDEX_BATTERY_VOLTAGE] = batteryVoltage; }

	inline const char *getParameterName(uint8_t i) { return (i < NUMBER_OF_PARAMETERS) ? paramNames[i] : NULL; }
	inline float getParameterValue(uint8_t i) {	return (i < NUMBER_OF_PARAMETERS) ? paramValues[i] : NAN; }
	inline void setParameterValue(uint8_t i, float v) {	if (i < NUMBER_OF_PARAMETERS) paramValues[i] = v; }

	inline uint32_t getCRC() const { return crc; }
	inline void setCRC(uint32_t crc) { this->crc = crc; }

private:

	typedef union {
		float float_t;
		uint32_t uint_t;
	} FloatUInt;

	static const uint8_t MAX_NAME_SIZE = 32;

	/**
	 * Reading time
	 */
	int32_t tm;

	/**
	 * Reading time (formated string)
	 */
	char tmStr[32];

	/**
	 * Parameter Names
	 */
	char paramNames[NUMBER_OF_PARAMETERS][MAX_NAME_SIZE];

	/**
	 * Parameter values
	 */
	float paramValues[NUMBER_OF_PARAMETERS];

	/**
	 * CRC value
	 */
	uint32_t crc;
};

#endif /* READINGDATA_H_ */
