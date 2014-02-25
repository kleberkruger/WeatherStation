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

class ReadingData {
public:

	typedef struct {
		const char *key;
		float *value;
	} KeyValue;

	static const int NUMBER_OF_PARAMETERS = 9;

	ReadingData();
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
	 * Calculates CRC (Cyclic Redundancy Check)
	 */
	long calculateCRC();

	/**
	 * Checks CRC (Cyclic Redundancy Check)
	 */
	bool checkCRC(long crc);

	inline float getAnemometer() const { return anemometer; }
	inline void setAnemometer(float anemometer) { this->anemometer = anemometer; }

	inline float getBatteryVoltage() const { return battery_voltage; }
	inline void setBatteryVoltage(float batteryVoltage) { battery_voltage = batteryVoltage; }

	inline long getCrc() const { return crc; }
	inline void setCrc(long crc) {	this->crc = crc; }

	inline float getHumidity() const {	return humidity; }
	inline void setHumidity(float humidity) { this->humidity = humidity; }

	inline float getPluviometer() const { return pluviometer; }
	inline void setPluviometer(float pluviometer) { this->pluviometer = pluviometer; }

	inline float getSoilHumidity() const {	return soil_humidity; }
	inline void setSoilHumidity(float soilHumidity) { soil_humidity = soilHumidity; }

	inline float getSoilTemperaure() const { return soil_temperaure; }
	inline void setSoilTemperaure(float soilTemperaure) { soil_temperaure = soilTemperaure; }

	inline float getSolarRadiation() const { return solar_radiation; }
	inline void setSolarRadiation(float solarRadiation) { solar_radiation = solarRadiation; }

	inline float getTemperature() const { return temperature; }
	inline void setTemperature(float temperature) { this->temperature = temperature; }

	inline time_t getTime() const { return tm; }
	inline void setTime(time_t tm) {	this->tm = tm; }

	inline float getWetting() const { return wetting; }
	inline void setWetting(float wetting) {	this->wetting = wetting; }

	inline KeyValue *getParameters() { return params; }

	inline long getCRC() const { return crc; }
	inline void setCRC(long crc) { this->crc = crc; }

private:

	time_t tm;

	float pluviometer;
	float anemometer;
	float wetting;
	float temperature;
	float humidity;
	float soil_temperaure;
	float soil_humidity;
	float solar_radiation;
	float battery_voltage;

	KeyValue params[NUMBER_OF_PARAMETERS];

	long crc;
};

#endif /* READINGDATA_H_ */

