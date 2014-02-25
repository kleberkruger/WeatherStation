/*
 =======================================================================================================================
 File       : ReadingData.cpp
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

#include "ReadingData.h"

ReadingData::ReadingData() {

	/*
	 * Initialize parameters with NAN value
	 */
	pluviometer 	= NAN;
	anemometer 		= NAN;
	wetting 		= NAN;
	temperature 	= NAN;
	humidity 		= NAN;
	soil_temperaure = NAN;
	soil_humidity 	= NAN;
	solar_radiation = NAN;
	battery_voltage = NAN;

	/*
	 * Set attribute names and pointers to them
	 */
	params[0].key 	= "Pluviometer";
	params[0].value = &pluviometer;

	params[1].key 	= "Anemometer";
	params[1].value = &anemometer;

	params[2].key 	= "Wetting";
	params[2].value = &wetting;

	params[3].key 	= "Temperature";
	params[3].value = &temperature;

	params[4].key 	= "Humidity";
	params[4].value = &humidity;

	params[5].key 	= "Soil temperature";
	params[5].value = &soil_temperaure;

	params[6].key 	= "Soil humidity";
	params[6].value = &soil_humidity;

	params[7].key 	= "Solar radiation";
	params[7].value = &solar_radiation;

	params[8].key 	= "Battery voltage";
	params[8].value = &battery_voltage;
}

ReadingData::~ReadingData() {

	/* Nothing to do */
}

ReadingData * ReadingData::create(ReadingData *data_1, ReadingData *data_2, ReadingData *data_3) {

	ReadingData::KeyValue *params;
	ReadingData::KeyValue *params_1, *params_2, *params_3;

	ReadingData *reading = new ReadingData();

	/*
	 * Get parameters array
	 */
	params = reading->getParameters();
	params_1 = data_1->getParameters();
	params_2 = data_3->getParameters();
	params_3 = data_3->getParameters();

	/*
	 * Set time
	 */
	if (data_1->getTime() == data_2->getTime())
		reading->setTime(data_1->getTime());
	else if (data_1->getTime() == data_3->getTime())
		reading->setTime(data_1->getTime());
	else if (data_2->getTime() == data_3->getTime())
		reading->setTime(data_2->getTime());
	else {
		free(reading);
		return NULL;
	}

	/*
	 * Set parameters
	 */
	for (int i = 0; i < NUMBER_OF_PARAMETERS; i++) {

		if (params_1[i].value == params_2[i].value)
			params[i].value = params_1[i].value;
		else if (params_1[i].value == params_3[i].value)
			params[i].value = params_1[i].value;
		else if (params_2[i].value == params_3[i].value)
			params[i].value = params_2[i].value;
		else {
			free(reading);
			return NULL;
		}
	}

	/*
	 * Set CRC
	 */
	if (data_1->getCRC() == data_2->getCRC())
		reading->setCRC(data_1->getCRC());
	else if (data_1->getCRC() == data_3->getCRC())
		reading->setCRC(data_1->getCRC());
	else if (data_2->getCRC() == data_3->getCRC())
		reading->setCRC(data_2->getCRC());
	else {
		free(reading);
		return NULL;
	}

	return reading;
}

long ReadingData::calculateCRC() {

	long crc = tm; // Calculates CRC

	crc ^= (long) pluviometer;
	crc ^= (long) anemometer;
	crc ^= (long) wetting;
	crc ^= (long) temperature;
	crc ^= (long) humidity;
	crc ^= (long) soil_temperaure;
	crc ^= (long) soil_humidity;
	crc ^= (long) solar_radiation;
	crc ^= (long) battery_voltage;

	return crc;
}

bool ReadingData::checkCRC(long crc) {

	this->crc = calculateCRC();

	if (this->crc == crc)
		return true;

	return false;
}
