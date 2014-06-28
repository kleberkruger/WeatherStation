/*
 =======================================================================================================================
 File       : WeatherStationFTFT.cpp
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

#include "WeatherStationFT.h"

static inline int compare(const void *n1, const void *n2);

WeatherStationFT::WeatherStationFT() : WeatherStation() {
	init();
}

WeatherStationFT::~WeatherStationFT() {

	/* Nothing to do */
}

WeatherStationFT* WeatherStationFT::getInstance() {
	static WeatherStationFT *station = NULL;

	if (station == NULL)
		station = new WeatherStationFT();

	return station;
}

void WeatherStationFT::destroy() {
	this->~WeatherStationFT();
}

void WeatherStationFT::init() {

//	TripledData<PulseIn> d1;
//	DLinkedList<int> list;

	/* Load configuration */
	cfg.loadFromFile(FILEPATH_CONFIG);

	/* Save configuration */
	cfg.saveToFile(FILEPATH_CONFIG,
			"# Weather station with implementing fault tolerance");

	/* Create configuration backup */
	memcpy(&cfg_copy_1, &cfg, sizeof(WeatherStationConfig));
	memcpy(&cfg_copy_2, &cfg, sizeof(WeatherStationConfig));

	/* Reset by watchdog */
	if ((LPC_WDT->WDMOD >> 2) & 1) {

		logger.log("Reset by watchdog FT."); /* XXX: The log time may be incorrect */

		/* Blink LED 2 */
		blinkLED(LED2, 10, 100);

		char *state = cfg.getValue("state");

		/* Read state value from configuration file and go to state */
		goToState(state ? atoi(state) : STATE_NOT_CONFIGURED);

	} else {

		logger.log("Reset by power-button FT."); /* XXX: The log time may be incorrect */

		/* Blink LED 1 */
		blinkLED(LED1, 10, 100);

		config();
	}
}

void WeatherStationFT::goToState(int state) {

	logger.log("goToState() - restore from state %d.", state);

	switch (state) {

	case STATE_NOT_CONFIGURED:
	case STATE_CONFIGURED:
		config();
		break;

	case STATE_READ_SENSORS:
	case STATE_SAVE_DATA:
		config();
		readSensors();
		break;

	case STATE_DATA_SAVED:
		config();
		break;

	case STATE_SEND_DATA:
		config();
		send();
		break;

	default:
		config();
		break;
	}
}

int WeatherStationFT::getStateByVoting() {

	if ((state != state_copy_1) || (state != state_copy_2))
		logger.log("Recovery: state data redundancy.");

	if (state == state_copy_1)
		state_copy_2 = state; // restore correct state
	else if (state == state_copy_2)
		state_copy_1 = state; // restore correct state
	else if (state_copy_1 == state_copy_2)
		state = state_copy_1; // restore correct state
	else {
		logger.err("Inconsistent state");
		return -1;
	}

	return state;
}

void WeatherStationFT::setState(int state) {

	/* Get state by voting */
	if (state != getStateByVoting()) {

		char state_str[8];

		/* Clean string and print state */
		memset(state_str, 0, sizeof(state_str));
		sprintf(state_str, "%d", state);

		/* Write a configuration value. */
		cfg.setValue("state", state_str);

		/* Write a configuration file to a mbed. */
		cfg.saveToFile(FILEPATH_CONFIG);
		this->state = state_copy_1 = state_copy_2 = state;

		logger.log("set state: %d.", state);
	}
}

uint8_t WeatherStationFT::getNumberOfReadings() {

	if ((cfg.getNumberOfReadings() != cfg_copy_1.getNumberOfReadings())
			|| (cfg.getNumberOfReadings() != cfg_copy_2.getNumberOfReadings())) {

		logger.log("Recovery: numberOfReadings data redundancy.");
	}

	if (cfg.getNumberOfReadings() == cfg_copy_1.getNumberOfReadings())
		cfg_copy_2.setNumberOfReadings(cfg.getNumberOfReadings()); // restore correct numberOfreadings
	else if (cfg.getNumberOfReadings() == cfg_copy_2.getNumberOfReadings())
		cfg_copy_1.setNumberOfReadings(cfg.getNumberOfReadings()); // restore correct numberOfreadings
	else if (cfg_copy_1.getNumberOfReadings()
			== cfg_copy_2.getNumberOfReadings())
		cfg.setNumberOfReadings(cfg_copy_1.getNumberOfReadings()); // restore correct numberOfreadings
	else {
		logger.err("Inconsistent numberOfreadings");
		return 0;
	}

	return cfg.getNumberOfReadings();
}

uint8_t WeatherStationFT::getMinCorrectReadings() {

	if ((cfg.getMinCorrectReadings() != cfg_copy_1.getMinCorrectReadings())
			|| (cfg.getMinCorrectReadings()
					!= cfg_copy_2.getMinCorrectReadings())) {

		logger.log("Recovery: minCorrectReadings data redundancy.");
	}

	if (cfg.getMinCorrectReadings() == cfg_copy_1.getMinCorrectReadings())
		cfg_copy_2.setMinCorrectReadings(cfg.getMinCorrectReadings()); // restore correct numberOfreadings
	else if (cfg.getMinCorrectReadings() == cfg_copy_2.getMinCorrectReadings())
		cfg_copy_1.setMinCorrectReadings(cfg.getMinCorrectReadings()); // restore correct numberOfreadings
	else if (cfg_copy_1.getMinCorrectReadings()
			== cfg_copy_2.getMinCorrectReadings())
		cfg.setMinCorrectReadings(cfg_copy_1.getMinCorrectReadings()); // restore correct numberOfreadings
	else {
		logger.err("Inconsistent minCorrectReadings");
		return 0;
	}

	return cfg.getMinCorrectReadings();
}

float WeatherStationFT::getReadingInterval() {

	if ((cfg.getReadingInterval() != cfg_copy_1.getReadingInterval())
			|| (cfg.getReadingInterval() != cfg_copy_2.getReadingInterval())) {

		logger.log("Recovery: readingInterval data redundancy.");
	}

	if (cfg.getReadingInterval() == cfg_copy_1.getReadingInterval()) {
		cfg_copy_2.setReadingInterval(WeatherStationConfig::READING_UNIT_SEC,
				cfg.getReadingInterval()); // restore correct readingInterval
	} else if (cfg.getReadingInterval() == cfg_copy_2.getReadingInterval()) {
		cfg_copy_1.setReadingInterval(WeatherStationConfig::READING_UNIT_SEC,
				cfg.getReadingInterval()); // restore correct readingInterval
	} else if (cfg_copy_1.getReadingInterval()
			== cfg_copy_2.getReadingInterval())
		cfg.setReadingInterval(WeatherStationConfig::READING_UNIT_SEC,
				cfg_copy_1.getReadingInterval()); // restore correct readingInterval
	else {
		logger.err("Inconsistent readingInterval");
		return -1;
	}

	return cfg.getReadingInterval();
}

void WeatherStationFT::config() {

	setState(STATE_NOT_CONFIGURED);

	WeatherStation::config();
}

bool WeatherStationFT::readGPS() {
	return WeatherStation::readGPS();
}

void WeatherStationFT::start() {

	int att;
	long processtime, total = 0, n = 0;
	float media;

	while (true) {

		setState(STATE_CONFIGURED);

		reloadWatchdog();

		if (isTimeToRead()) {

			processtime = time(NULL);

			logger.log("TIME %ld", processtime);

			readSensors();

			for (att = 1; att <= 3 && !saveData(); att++)
				;

			if (att <= 3) {
				setState(STATE_DATA_SAVED);
				if (att > 1)
					logger.log("Recovery: Save data redundancy.");
			} else {
				logger.err("Unable to save data.");
			}

			processtime = time(NULL) - processtime;

			total += processtime;
			n++;

			media = total / n;

			logger.log("Process time: %ld", processtime);
			logger.log("Process time: %.2f", media);

#ifdef FAULT_INJECTION_IN_MEMORY_ENABLED
			injector.start(0.1, getReadingInterval() - 0.1);
#endif
		}

		if (isTimeToSend()) {

			for (att = 1; att <= 3 && !send(); att++)
				;

			if (att <= 3) {
				if (att > 1)
					logger.log("Recovery: Send redundancy.");
			} else {
				logger.err("Unable to send data.");
			}
		}

		if (getReadingInterval() >= 60)
			Sleep();
		else
			wait(0.5);
	}
}

bool WeatherStationFT::isTimeToRead() {

	struct tm *timest;
	time_t time_sec;
	static bool reading = false;
	int tmval;

	time(&time_sec);
	timest = localtime(&time_sec);

	tmval = (getReadingInterval() >= 60) ? timest->tm_min : timest->tm_sec;

	if (tmval % (int) getReadingInterval())
		reading = false;
	else if (!reading)
		reading = true;
	else
		return false;

	return reading;
}

bool WeatherStationFT::isTimeToSend() {

	struct tm *timest;
	time_t time_sec;
	static bool sending = false;

	time(&time_sec);
	timest = localtime(&time_sec);

	if ((timest->tm_hour != cfg.getSendTime().tm_hour)
			|| (timest->tm_min != cfg.getSendTime().tm_min))
		sending = false;
	else if (!sending)
		sending = true;
	else
		return false;

	return sending;
}

void WeatherStationFT::readSensors() {

	int i, att, rdIntv = 10;
	float result;
	float samples[getNumberOfReadings()];

	setState(STATE_READ_SENSORS);

	logger.log("readSensors() - initializing readings.");

	powerBattery(POWER_ON); // Liga Vbat e 5Vc
	wait_ms(200); // Tempo para Vbat estabilizar, pois o acionamento do MOSFET é lento (+/- 23ms)

	SHTx::SHT15 sensorTE_UR(p29, p30); 	// DATA, SCK
	sensorTE_UR.setOTPReload(false);
	sensorTE_UR.setResolution(true);

	Anemometer anem(p21);
	Wetting wet(p19, p26, p25);

	wet.config(100, 1000, 1, 3000); // 100kohms, 1000us, 1kohms, 3000k=3Mohms

	data.setTime(time(NULL));

	sensorTE_UR.update();
	sensorTE_UR.setScale(false);

	att = 1;
	do {
		for (i = 0; i < getNumberOfReadings(); i++) {
			samples[i] = anem.read();
			if (rdIntv > 0)
				wait_ms(rdIntv);
		}
		result = avg(samples, getNumberOfReadings(), getMinCorrectReadings(),
				10);
	} while (isnan(result) && att++ < 3);
	data.setAnemometer(result);

	if (att > 1)
		logger.log("Recovery: AVG Redundancy. [Anemometer]");

	att = 1;
	do {
		for (i = 0; i < getNumberOfReadings(); i++) {
			samples[i] = pluv.read();
			if (rdIntv > 0)
				wait_ms(rdIntv);
		}
		result = avg(samples, getNumberOfReadings(), getMinCorrectReadings(),
				10);
	} while (isnan(result) && att++ < 3);
	data.setPluviometer(result);
	pluv.resetCount();

	if (!isnan(result) && att > 1)
		logger.log("Recovery: AVG Redundancy. [Pluviometer]");

	// Molhamento [kohms]
	att = 1;
	do {
		for (i = 0; i < getNumberOfReadings(); i++) {
			samples[i] = wet.read() / 1000;
			if (rdIntv > 0)
				wait_ms(rdIntv);
		}
		result = avg(samples, getNumberOfReadings(), getMinCorrectReadings(),
				10);
	} while (isnan(result) && att++ < 3);
	data.setWetting(result);

	if (!isnan(result) && att > 1)
		logger.log("Recovery: AVG Redundancy. [Wetting]");

	att = 1;
	do {
		for (i = 0; i < getNumberOfReadings(); i++) {
//			samples[i] = sensorTE_UR.getTemperature();
			samples[i] = 25;
			if (rdIntv > 0)
				wait_ms(rdIntv);
		}
		result = avg(samples, getNumberOfReadings(), getMinCorrectReadings(),
				10);
	} while (isnan(result) && att++ < 3);
	data.setTemperature(result);

	if (!isnan(result) && att > 1)
		logger.log("Recovery: AVG Redundancy. [Temperature]");

	att = 1;
	do {
		for (i = 0; i < getNumberOfReadings(); i++) {
//			samples[i] = sensorTE_UR.getHumidity();
			samples[i] = 65.8;
			if (rdIntv > 0)
				wait_ms(rdIntv);
		}
		result = avg(samples, getNumberOfReadings(), getMinCorrectReadings(),
				10);
	} while (isnan(result) && att++ < 3);
	data.setHumidity(result);

	if (!isnan(result) && att > 1)
		logger.log("Recovery: AVG Redundancy. [Humidity]");

	att = 1;
	do {
		for (i = 0; i < getNumberOfReadings(); i++) {
			samples[i] = readSensor(5, 0.320512821, 50, 3.205128205, 17);
			if (rdIntv > 0)
				wait_ms(rdIntv);
		}
		result = avg(samples, getNumberOfReadings(), getMinCorrectReadings(),
				10);
	} while (isnan(result) && att++ < 3);
	data.setSoilTemperaure(result);

	if (!isnan(result) && att > 1)
		logger.log("Recovery: AVG Redundancy. [Soil Temperature]");

	att = 1;
	do {
		for (i = 0; i < getNumberOfReadings(); i++) {
			samples[i] = readSensor(1.1, 0, 5.54, 1.0, 16);
			if (rdIntv > 0)
				wait_ms(rdIntv);
		}
		result = avg(samples, getNumberOfReadings(), getMinCorrectReadings(),
				10);
	} while (isnan(result) && att++ < 3);
	data.setSoilHumidity(result);

	if (!isnan(result) && att > 1)
		logger.log("Recovery: AVG Redundancy. [Soil Humidity]");

	// Irradiação solar [W/m2]
	att = 1;
	do {
		for (i = 0; i < getNumberOfReadings(); i++) {
			samples[i] = readSensor(0, 0, 1500, 1.5, 18);
			if (rdIntv > 0)
				wait_ms(rdIntv);
		}
		result = avg(samples, getNumberOfReadings(), getMinCorrectReadings(),
				10);
	} while (isnan(result) && att++ < 3);
	data.setSolarRadiation(result);

	if (!isnan(result) && att > 1)
		logger.log("Recovery: AVG Redundancy. [Solar Radiation]");

	// Tensão da bateria [V]
	att = 1;
	do {
		for (i = 0; i < getNumberOfReadings(); i++) {
			samples[i] = readSensor(0, 0, 15.085714286, 3.3, 15);
			if (rdIntv > 0)
				wait_ms(rdIntv);
		}
		result = avg(samples, getNumberOfReadings(), getMinCorrectReadings(),
				10);
	} while (isnan(result) && att++ < 3);
	data.setBatteryVoltage(result);

	if (!isnan(result) && att > 1)
		logger.log("Recovery: AVG Redundancy. [Battery Voltage]");

	data.setSoilTemperaure(readSensor(17, 5, 0.320512821, 50, 3.205128205)); // Temperatura do solo [C]
	data.setSoilHumidity(readSensor(16, 1.1, 0, 5.54, 1.0)); // Umidade do solo [raiz de epsilon]
	data.setSolarRadiation(readSensor(18, 0, 0, 1500, 1.5)); // Irradiação solar [W/m2]
	data.setBatteryVoltage(readSensor(15, 0, 0, 15.085714286, 3.3)); // Tensão da bateria [V]

	// Calculates CRC
	data.setCRC(data.calculateCRC());

	powerBattery(POWER_OFF); 	// Desliga Vbat e 5Vc

	memcpy(&data_copy_1, &data, sizeof(ReadingData));
	memcpy(&data_copy_2, &data, sizeof(ReadingData));

//	printDataInfo(&data, "\t");
//	printDataInfo(&data_copy_1, "B");
//	printDataInfo(&data_copy_2, "C");

	logger.log("readSensors() finished.");
}

float WeatherStationFT::avg(float data[], int n, int n2, float var) {

	int i, j, left, right, cr;
	float result = 0;

#ifdef FAULT_INJECTION_IN_SENSOR_ENABLED
	for (int x, i = 0; i < n; i++) {
		x = FaultInjector::getRandomUInt(1, 4);
		if (x == 4) {
			data[i] = FaultInjector::getRandomFloat(0.0, 1000000.0);
		}
	}
#endif

//	for (int i = 0; i < n; i++)
//		logger.log("%.5f ", data[i]);

	qsort(data, n, sizeof(int), compare);

	left = right = n / 2;

	for (cr = 1, i = left; i >= 0; i--) {
		for (j = i + 1; j < n && data[j] <= data[i] + (var * 2); j++);
		if (j - i > cr) {
			cr = j - i;
			left = i;
			right = j - 1;
		}
	}

	if (cr < n2)
		return NAN;

	for (i = left; i <= right; i++) {
		result += data[i];
	}

	if (cr < n)
		logger.log("Recovery: AVG");

	result = result / cr;
	return result;
}

bool WeatherStationFT::allDataIsConsistent() {

	if ((data.getTime() != data_copy_1.getTime())
			|| (data.getTime() != data_copy_2.getTime()))
		return false;

	for (int i = 0; i < ReadingData::NUMBER_OF_PARAMETERS; i++) {

		if ((data.getParameterValue(i) != data_copy_1.getParameterValue(i))
				|| (data.getParameterValue(i)
						!= data_copy_2.getParameterValue(i))) {

			return false;
		}
	}

	if ((data.getCRC() != data_copy_1.getCRC())
			|| (data.getCRC() != data_copy_2.getCRC()))
		return false;

	return true;
}

bool WeatherStationFT::saveData() {

	setState(STATE_SAVE_DATA);

	logger.log("saveData() - saving binary file.");

	ReadingData *temp = ReadingData::create(&data, &data_copy_1, &data_copy_2);

	if (!temp) {
		logger.err("Data Error!");
		return false;
	} else {
		logger.log("Readings OK.");
		if (!allDataIsConsistent())
			logger.log("Recovery: Data Redundancy.");
	}

//	printDataInfo(temp, "Temp");

	bool status = true;

	if (!temp->save(FILEPATH_DATA))
		status = false;
	if (!temp->save(FILEPATH_DATA_COPY_1))
		status = false;
	if (!temp->save(FILEPATH_DATA_COPY_2))
		status = false;

	/* Creates ready file */
	FILE *fp = fopen(FILEPATH_READY, "w");
	if (fp)
		fclose(fp);

	free(temp);

//	ReadingData *d = ReadingData::load(FILEPATH_DATA_1);
//	printDataInfo(d, "Checking");
//	free(d);

	logger.log("saveData() - binary file saved.");

	return status;
}

bool WeatherStationFT::send() {
	return WeatherStation::send();
}

static inline int compare(const void *n1, const void *n2) {
	return (*(float*) n1 - *(float*) n2);
}
