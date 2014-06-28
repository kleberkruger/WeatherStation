/*
 =======================================================================================================================
 File       : WeatherStationConfig.cpp
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

#include "WeatherStationConfig.h"

/*----------------------------------------------------------------------------------------------------------------------
 Define Constant Values
 ---------------------------------------------------------------------------------------------------------------------*/
const char* const WeatherStationConfig::NEWLINE_UNIX = "\n";
const char* const WeatherStationConfig::NEWLINE_DOS = "\r\n";
const char* const WeatherStationConfig::NEWLINE_MAC = "\r";

const char* const WeatherStationConfig::CONFIG_HEADER_TXT = "# Weather station with implementing fault tolerance";
const char* const WeatherStationConfig::DEFAULT_SEND_TIME = "11:00:00";
/*--------------------------------------------------------------------------------------------------------------------*/

WeatherStationConfig::WeatherStationConfig() {

	/* Initialize attributes */
	numberReadings = DEFAULT_READINGS_AMOUNT;
	minCorrectReadings = getDefaultMinCorrectReadings();
	readingInterval = DEFAULT_READINGS_INTERVAL;
	readingUnit = READING_UNIT_SEC;
	watchdogTime = DEFAULT_WATCHDOG_TIME;

	resetToDefaultSendTime();
}

WeatherStationConfig::~WeatherStationConfig() {

	/* Nothing to do */
}

WeatherStationConfig::ReadingUnitType WeatherStationConfig::convertUnit(const char *value) {

	if ((value) && (atoi(value) == READING_UNIT_MIN || strcasecmp(value, "m") == 0 || strcasecmp(value, "min") == 0))
		return READING_UNIT_MIN;

	return READING_UNIT_SEC;
}

void WeatherStationConfig::loadFromList() {

	char *arg1, *arg2;

	setReadingConfig((arg1 = getValue("numberOfReadings")) ? atoi(arg1) : 0,
			(arg2 = getValue("minCorrectReadings")) ? atoi(arg2) : 0);
	if (arg1) free(arg1);
	if (arg2) free(arg2);


	arg1 = getValue("readingUnit");
	setReadingInterval(convertUnit(arg1), (arg2 = getValue("readingInterval")) ? atoff(arg2) : 0);
	if (arg1) free(arg1);
	if (arg2) free(arg2);

	setSendTime((arg1 = getValue("sendTime")));
	if (arg1) free(arg1);

	setWatchdogTime((arg1 = getValue("watchdogTime")) ? atoff(arg1) : 0);
	if (arg1) free(arg1);
}

void WeatherStationConfig::saveToList() {

	char buffer[64];

	/* Save number of readings */
	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "%d", numberReadings);
	setValue("numberOfReadings", buffer);

	/* Save minimum correct readings */
	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "%d", minCorrectReadings);
	setValue("minCorrectReadings", buffer);

	/* Save reading unit */
	setValue("readingUnit", (readingUnit == READING_UNIT_MIN) ? "min" : "sec");

	/* Save interval of readings */
	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "%.1f", (readingUnit == READING_UNIT_MIN) ? readingInterval * 60 : readingInterval);
	setValue("readingInterval", buffer);

	/* Save watchdog */
	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "%.1f", watchdogTime);
	setValue("watchdogTime", buffer);

	/* Save send time */
	memset(buffer, 0, sizeof(buffer));
	strftime(buffer, 32, "%H:%M:%S", &sendTime);
	setValue("sendTime", buffer);

	/* Save serial number */
	memset(buffer, 0, sizeof(buffer));
//	sprintf(buffer, "%d", WeatherStation::SERIAL_NUMBER);
	sprintf(buffer, "%d", 123456789); /* XXX */
	setValue("serialNumber", buffer);

	/* Save software version */
	memset(buffer, 0, sizeof(buffer));
//	sprintf(buffer, "%.1f", WeatherStation::SOFTWARE_VERSION);
	sprintf(buffer, "%.1f", 0.1); /* XXX */
	setValue("softwareInfo", buffer);

	/* Save software info */
	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "%s-%s-%s", __TIME__, __DATE__, __FILE__);
	setValue("softwareInfo", buffer);
}

bool WeatherStationConfig::loadFromFile(const char *file) {

	/* Open the target file */
	FILE *fp = fopen(file, "r");
	if (fp == NULL) {
		return false;
	}

	/* Remove all configuration */
	removeAll();

	/* Read from a file */
	char buf[MAXLEN_KEY + 8 + MAXLEN_VALUE];
	while (fgets(buf, sizeof(buf), fp) != NULL) {

		/* Ignore a comment */
		if (buf[0] == '#') {
			continue;
		}

		/* Trim a return */
		const size_t len = strlen(buf);
		for (size_t i = 0; i < len; i++) {
			if ((buf[i] == '\r') || (buf[i] == '\n')) {
				buf[i] = '\0';
			}
		}

		/* Separate key and value */
		char k[MAXLEN_KEY];
		char v[MAXLEN_VALUE];
		char *sp = strchr(buf, SEPARATOR);
		if (sp != NULL) {
			strcpy(v, sp + 1);
			*sp = '\0';
			strcpy(k, buf);
			setValue(k, v);
		}
	}

	fclose(fp);

	/* Load configuration values from the list */
	loadFromList();

	return true;
}

bool WeatherStationConfig::saveToFile(const char *file, const char *header, FileFormat format) {

	/* Save configuration in the list */
	saveToList();

	/* Open the target file */
	FILE *fp = fopen(file, "w");
	if (fp == NULL) {
		return false;
	}

	/* Set a type of new line */
	const char *newline = NEWLINE_UNIX;
	switch (format) {
		case UNIX:
			newline = NEWLINE_UNIX;
			break;
		case MAC:
			newline = NEWLINE_MAC;
			break;
		case DOS:
			newline = NEWLINE_DOS;
			break;
		default:
			newline = NEWLINE_UNIX;
			break;
	}

	/* Write the header */
	if (header != NULL) {
		fprintf(fp, "%s%s", header, newline);
	}

	/* Write the data */
	for (int i = 0; i < MAXCONFIG; i++) {
		config_t *cfg = configlist[i];
		if (cfg != NULL) {
			fprintf(fp, "%s=%s%s", cfg->key, cfg->value, newline);
		}
	}

	fclose(fp);
	return true;
}

bool WeatherStationConfig::checkTime(int hour, int min, int sec) {
	if (hour < 0 || hour > 23)
		return false;
	if (min < 0 || min > 59)
		return false;
	if (sec < 0 || sec > 59)
		return false;

	return true;
}

bool WeatherStationConfig::checkTime(const char *timeStr) {

	if (!timeStr)
		return false;

	int hour = -1;
	int min = -1;
	int sec = 0;

	sscanf(timeStr, "%d:%d:%d", &hour, &min, &sec);

	return checkTime(hour, min, sec);
}

bool WeatherStationConfig::checkTime(const struct tm *tmInfo) {

	if (!tmInfo)
		return false;

	return checkTime(tmInfo->tm_hour, tmInfo->tm_min, tmInfo->tm_sec);
}
