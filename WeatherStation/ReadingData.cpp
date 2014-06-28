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

	/* Initialize time and CRC variables with zero */
	tm = crc = 0;

	/* Clean names array */
	memset(paramNames, 0, sizeof(paramNames));

	/*
	 * Initialize parameters with NAN value
	 */
	for (int i = 0; i < NUMBER_OF_PARAMETERS; i++)
		paramValues[i] = NAN;

	/*
	 * Set attribute names
	 */
	sprintf(paramNames[0], "%s", "Anemometer");
	sprintf(paramNames[1], "%s", "Pluviometer");
	sprintf(paramNames[2], "%s", "Wetting");
	sprintf(paramNames[3], "%s", "Temperature");
	sprintf(paramNames[4], "%s", "Humidity");
	sprintf(paramNames[5], "%s", "Soil temperature");
	sprintf(paramNames[6], "%s", "Soil humidity");
	sprintf(paramNames[7], "%s", "Solar radiation");
	sprintf(paramNames[8], "%s", "Battery voltage");
}

ReadingData::~ReadingData() {

	/* Nothing to do */
}

ReadingData * ReadingData::create(ReadingData *data_1, ReadingData *data_2, ReadingData *data_3) {

	ReadingData *reading = new ReadingData();

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

		if (data_1->getParameterValue(i) == data_2->getParameterValue(i))
			reading->setParameterValue(i, data_1->getParameterValue(i));
		else if (data_1->getParameterValue(i) == data_3->getParameterValue(i))
			reading->setParameterValue(i, data_1->getParameterValue(i));
		else if (data_2->getParameterValue(i) == data_3->getParameterValue(i))
			reading->setParameterValue(i, data_2->getParameterValue(i));
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

ReadingData* ReadingData::load(const char *file) {

	FILE *fp = fopen(file, "r");

	if (fp == NULL)
		return NULL;

	ReadingData *data = new ReadingData();

//	fread(data, sizeof(ReadingData), 1, fp);

	fread(&(data->tm), sizeof(data->tm), 1, fp);
	fread(data->paramNames, sizeof(data->paramNames), 1, fp);
	fread(data->paramValues, sizeof(data->paramValues), 1, fp);
	fread(&(data->crc), sizeof(data->crc), 1, fp);

	fclose(fp);

	return data;
}

bool ReadingData::save(const char *file) {

	FILE *fp = fopen(file, "w");

	if (fp == NULL)
		return false;

//	fwrite(this, sizeof(ReadingData), 1, fp);

	fwrite(&tm, sizeof(tm), 1, fp);
	fwrite(paramNames, sizeof(paramNames), 1, fp);
	fwrite(paramValues, sizeof(paramValues), 1, fp);
	fwrite(&crc, sizeof(crc), 1, fp);

	fclose(fp);

	return true;
}

uint32_t ReadingData::calculateCRC() {

	FloatUInt value;
	uint32_t crc = tm;

	for (int i = 0; i < NUMBER_OF_PARAMETERS; i++) {
		value.float_t = paramValues[i];
		crc ^= value.uint_t;
	}

	return crc;
}

bool ReadingData::checkCRC() {
	return checkCRC(this->crc);
}

bool ReadingData::checkCRC(uint32_t crc) {

//	if (calculateCRC() == crc)
//		return true;

	if ((calculateCRC() ^ crc) == 0)
		return true;

	return false;
}
