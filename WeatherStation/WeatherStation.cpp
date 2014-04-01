/*
 =======================================================================================================================
 File       : WeatherStation.cpp
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

#include "WeatherStation.h"

static inline int compare(const void *n1, const void *n2);

static inline void safe_free(void *ptr);
static inline void safe_fclose(FILE *fp);

WeatherStation::WeatherStation(/* WeatherStationConfig *conf */) :
		fs(FILESYSTEM_NAME), logger(FILEPATH_LOG, true), gps(p13, p14) {

//	if (conf == NULL) {
//		conf = new WeatherStationConfig();
//		conf->loadFromFile(FILEPATH_CONFIG);
//		cfg = conf;
//	}

	init();
}

WeatherStation::~WeatherStation() {

	/* Nothing to do */
}

WeatherStation* WeatherStation::getInstance() {
	static WeatherStation *station = NULL;

	if (station == NULL)
		station = new WeatherStation();

	return station;
}

void WeatherStation::init() {

	/* Load configuration */
	cfg.loadFromFile(FILEPATH_CONFIG);

//	logger.log("%-18s: %u", "numberOfReadings", cfg.getNumberOfReadings());
//	logger.log("%-18s: %u", "minCorrectReadings", cfg.getMinCorrectReadings());
//	logger.log("%-18s: %s", "readingUnit",
//			(cfg.getReadingUnit() == WeatherStationConfig::READING_UNIT_SEC) ? "Sec" : "Min");
//	logger.log("%-18s: %.1f", "readingInterval",
//			(cfg.getReadingUnit() == WeatherStationConfig::READING_UNIT_SEC) ?
//					cfg.getReadingInterval() : cfg.getReadingInterval() / 60);
//	logger.log("%-18s: %s", "sendTime", cfg.getFormatedTime());
//	logger.log("%-18s: %.1f", "watchdogTime", cfg.getWatchdogTime());

	/* Save configuration */
	cfg.saveToFile(FILEPATH_CONFIG, "# Weather station with implementing fault tolerance");

	/* Reset by watchdog */
	if ((LPC_WDT->WDMOD >> 2) & 1) {

		logger.log("Reset by watchdog."); /* XXX: The log time may be incorrect */

		/* Blink LED 2 */
		blinkLED(LED2, 10, 100);

		char *state = cfg.getValue("state");

		/* Read state value from configuration file and go to state */
		goToState(state ? atoi(state) : STATE_NOT_CONFIGURED);

	} else {

		logger.log("Reset by power-button."); /* XXX: The log time may be incorrect */

		/* Blink LED 1 */
		blinkLED(LED1, 10, 100);

		config();
	}
}

void WeatherStation::goToState(int state) {

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

int WeatherStation::getStateByVoting() {

	if (state == state_copy_1)
		state_copy_2 = state; // restore correct state
	else if (state == state_copy_2)
		state_copy_1 = state; // restore correct state
	else if (state_copy_1 == state_copy_2)
		state = state_copy_1; // restore correct state
	else
		return -1;

	return state;
}

void WeatherStation::setState(int state) {

	/* Get state by voting */
	if (state != getStateByVoting()) {

		char state_str[8];

		/* Clean string and print state */
		memset(state_str, 0, sizeof(char) * 8);
		sprintf(state_str, "%d", state);

		/* Write a configuration value. */
		cfg.setValue("state", state_str);

		/* Write a configuration file to a mbed. */
		cfg.saveToFile(FILEPATH_CONFIG);
		this->state = state_copy_1 = state_copy_2 = state;

		logger.log("set state: %d.", state);
	}
}

void WeatherStation::config() {

	setState(STATE_NOT_CONFIGURED);

	logger.log("config() - initializing configuration.");

	powerMbed(POWER_ON); 	// Power on mbed
	powerGPS(POWER_OFF); 	// Power off GPS
	PHY_PowerDown(); 		// Disable ethernet to reduce consumption
	configTimer();			// Configures timer

	wdt.kick(cfg.getWatchdogTime());	// Configures watchdog timer

//	weak.attach(this, &WeatherStation::reloadWatchdog, (cfg.getWatchdogTime() - 1.0)); /* XXX */

//	saveInfoFile();

	logger.log("config() - successfully configured.");
}

void WeatherStation::configTimer() {

	set_time(1396238400); 	// 31 March 2014 (00:00:00) /* XXX */

//	int year, mounth;
//	struct tm t;
//	Timer tm;
//	Serial pc(USBTX, USBRX);
//
//	pc.printf("Acertar RTC? (qualquer tecla = sim)\n");
//	tm.start();
//
//	while (!pc.readable() && tm.read() < 2);
//
//	if (pc.readable()) {
//
//		while (pc.getc() != 0x0D);
//
//		pc.printf("Entre com dd/mm/aaaa hh:mm\n");
//		t.tm_sec = 0;
//		pc.scanf("%d/%d/%d %d:%d", &t.tm_mday, &mounth, &year, &t.tm_hour, &t.tm_min);
//		t.tm_year = year - 1900;
//		t.tm_mon = mounth - 1;
//		set_time(mktime(&t));
//
//	} else {
//		//set_time(1256729737); //28 Oct 2009 11:35:37
//	}
}

bool WeatherStation::saveInfoFile() {

	FILE *fp = fopen(FILEPATH_INFO, "w"); // Se o arquivo existir, grava por cima.

	if (fp == NULL)
		return false;

	fprintf(fp, "Parameter \t Unit \t Min value \t Max value\n");

	fprintf(fp, "Air temperature \t degrees C \t ? \t ?\n");
	fprintf(fp, "Air humidity \t %% U.R. \t ? \t ?\n");
	fprintf(fp, "Rainfall \t mm \t 0.0 \t ?\n");
	fprintf(fp, "Wind speed \t m/s \t 0.0 \t ?\n");
	fprintf(fp, "Soil humidity \t Epsilon root \t 1.1 \t 5.54\n");
	fprintf(fp, "Soil temperature \t degrees C \t 5.0 \t 50.0\n");
	fprintf(fp, "Solar radiation \t W/m2 \t 0.0 \t 1500\n");
	fprintf(fp, "Leaf wetness \t kohms \t 1.0 \t 3000\n");
	fprintf(fp, "Battery voltage \t Volts \t 0.0 \t 15.0\n\n");

	fprintf(fp, "Serial Number = \t\t %d\n", SERIAL_NUMBER);
	fprintf(fp, "Firmware version = \t %s \t %s \t %s\n", __TIME__, __DATE__, __FILE__);

#ifdef GPS_ENABLE
	if (readGPS())
		fprintf(fp, "Latitude = %f \t Longitude= %f\n", gps.latitude, gps.longitude);
	else
		fprintf(fp, "Latitude = ? \t Longitude= \t ?\n");
#endif

	fclose(fp);

	return true;
}

bool WeatherStation::readGPS() {

	Timer tm;

	powerGPS(POWER_ON); // Habilita GPS
	tm.start();

	do {

		if (gps.sample()) {
			logger.log("Lock OK");
			powerGPS(POWER_ON); // Desabilita GPS
			return true;
		} else {
			logger.log("No lock.");
		}

		wait(1.0);

	} while (!gps.lock && tm.read() < 3);

	powerGPS(POWER_OFF); // Habilita GPS

	return false;
}

/**
 * Reload watchdog and blink LED 3
 */
void WeatherStation::reloadWatchdog() {

	wdt.kick();

	blinkLED(LED3, 1, 50);
}

void WeatherStation::start() {

	while (true) {

		setState(STATE_CONFIGURED);

		reloadWatchdog();

		if (isTimeToRead()) {

			int att;

			readSensors();

			for (att = 1; att <= 3 && !saveData(); att++)
				;

			if (att <= 3)
				setState(STATE_DATA_SAVED);

#ifdef FAULT_INJECTOR_ENABLE
			injector.start(0.1, cfg.getReadingInterval() - 0.1);
#endif
		}

		if (isTimeToSend())
			send();

		if (cfg.getReadingInterval() >= 60)
			Sleep();
		else
			wait(0.5);
	}
}

bool WeatherStation::isTimeToRead() {

	struct tm *timest;
	time_t time_sec;
	static bool reading = false;
	int tmval;

	time(&time_sec);
	timest = localtime(&time_sec);

	tmval = (cfg.getReadingInterval() >= 60) ? timest->tm_min : timest->tm_sec;

	if (tmval % (int) cfg.getReadingInterval())
		reading = false;
	else if (!reading)
		reading = true;
	else
		return false;

	return reading;
}

bool WeatherStation::isTimeToSend() {

	struct tm *timest;
	time_t time_sec;
	static bool sending = false;

	time(&time_sec);
	timest = localtime(&time_sec);

	if ((timest->tm_hour != cfg.getSendTime().tm_hour) || (timest->tm_min != cfg.getSendTime().tm_min))
		sending = false;
	else if (!sending)
		sending = true;
	else
		return false;

	return sending;
}

void WeatherStation::printDataInfo(ReadingData *d, const char *prefix) {
	logger.log("%s Time: %ld", prefix, d->getTime());
	for (int i = 0; i < ReadingData::NUMBER_OF_PARAMETERS; i++)
		logger.log("%s %s: %.6f", prefix, d->getParameterName(i), d->getParameterValue(i));
	logger.log("%s CRC: %ld", prefix, d->getCRC());
}

void WeatherStation::readSensors() {

//	int count;
//	float samples[cfg.getNumberOfReadings()];
	Anemometer vel(p21);
	Wetting wet(p19, p26, p25);

	wet.config(100, 1000, 1, 3000); 	// 100kohms, 1000us, 1kohms, 3000k = 3Mohms

	setState(STATE_READ_SENSORS);

	logger.log("readSensors() iniciada");

//	powerBattery(POWER_ON); 	// Liga Vbat e 5Vc
//	wait_ms(200); 	// Tempo para Vbat estabilizar, pois o acionamento do MOSFET é lento (+/- 23ms)
//
//	SHTx::SHT15 sensorTE_UR(p29, p30); 	// DATA, SCK
//	sensorTE_UR.setOTPReload(false);
//	sensorTE_UR.setResolution(true);
//
//	data.setTime(time(NULL));
//
//	sensorTE_UR.update();
//	sensorTE_UR.setScale(false);
//
//	for (count = 0; count < cfg.getNumberOfReadings(); count++)
//		samples[count] = sensorTE_UR.getTemperature();
//	data.setTemperature(avg(samples, cfg.getNumberOfReadings(), cfg.getMinCorrectReadings(), 5));
//
//	for (count = 0; count < cfg.getNumberOfReadings(); count++)
//		samples[count] = sensorTE_UR.getHumidity();
//	data.setHumidity(avg(samples, cfg.getNumberOfReadings(), cfg.getMinCorrectReadings(), 5));
//
//	for (count = 0; count < cfg.getNumberOfReadings(); count++)
//		samples[count] = pluv.read();
//	data.setPluviometer(avg(samples, cfg.getNumberOfReadings(), cfg.getMinCorrectReadings(), 5));
//
//	for (count = 0; count < cfg.getNumberOfReadings(); count++)
//		samples[count] = vel.read();
//	data.setAnemometer(avg(samples, cfg.getNumberOfReadings(), cfg.getMinCorrectReadings(), 5));
//
//	// Umidade do solo [raiz de epsilon]
//	for (count = 0; count < cfg.getNumberOfReadings(); count++)
//		samples[count] = readSensor(1.1, 0, 5.54, 1.0, 16);
//	data.setSoilHumidity(avg(samples, cfg.getNumberOfReadings(), cfg.getMinCorrectReadings(), 5));
//
//	// Temperatura do solo [C]
//	for (count = 0; count < cfg.getNumberOfReadings(); count++)
//		samples[count] = readSensor(5, 0.320512821, 50, 3.205128205, 17);
//	data.setSoilTemperaure(avg(samples, cfg.getNumberOfReadings(), cfg.getMinCorrectReadings(), 5));
//
//	// Irradiação solar [W/m2]
//	for (count = 0; count < cfg.getNumberOfReadings(); count++)
//		samples[count] = readSensor(0, 0, 1500, 1.5, 18);
//	data.setSolarRadiation(avg(samples, cfg.getNumberOfReadings(), cfg.getMinCorrectReadings(), 5));
//
//	// Molhamento [kohms]
//	for (count = 0; count < cfg.getNumberOfReadings(); count++)
//		samples[count] = wet.read() / 1000;
//	data.setWetting(avg(samples, cfg.getNumberOfReadings(), cfg.getMinCorrectReadings(), 5));
//
//	// Tensão da bateria [V]
//	for (count = 0; count < cfg.getNumberOfReadings(); count++)
//		samples[count] = readSensor(0, 0, 15.085714286, 3.3, 15);
//	data.setBatteryVoltage(avg(samples, cfg.getNumberOfReadings(), cfg.getMinCorrectReadings(), 5));
//
//	// Calcula CRC
//	data.setCRC(data.calculateCRC());

	data.setTime(1395753010);
	for (int i = 0; i < ReadingData::NUMBER_OF_PARAMETERS; i++)
		data.setParameterValue(i, 25);
	data.setCRC(data.calculateCRC());

	memcpy(&data_copy_1, &data, sizeof(ReadingData));
	memcpy(&data_copy_2, &data, sizeof(ReadingData));

//	printDataInfo(&data, "A");
//	printDataInfo(&data_copy_1, "B");
//	printDataInfo(&data_copy_2, "C");

	powerBattery(POWER_OFF); 	// Desliga Vbat e 5Vc
	logger.log("readSensors() concluida");
}

float WeatherStation::readSensor(int num_pino, float v_ini_par, float v_ini_volts, float v_fim_par, float v_fim_volts) {

	AnalogIn ea[] = { (p15), (p16), (p17), (p18), (p19), (p20) };
	float ca, cl, par, lcad;

	ca = (v_fim_par - v_ini_par) / (v_fim_volts - v_ini_volts);
	cl = v_ini_par - ca * v_ini_volts;
	lcad = ea[num_pino - 15].read();
	par = ca * lcad * 3.3 + cl;

	if (ca > 0) {
		if (par < v_ini_par || lcad == 0.0)
			return (-INFINITY);
		if (par > v_fim_par || lcad == 1.0)
			return (INFINITY);
	}

	if (ca < 0) {
		if (par > v_ini_par || lcad == 0.0)
			return (INFINITY);
		if (par < v_fim_par || lcad == 1.0)
			return (-INFINITY);
	}

	return (par);
}

float WeatherStation::avg(float data[], int n, int n2, float variation) {

	int i, j, left, right, lc;
	float result = 0;

#ifdef FAULT_INJECTOR_ENABLE
	for (int i = 0; i < n; i++) {
		int x = FaultInjector::getRandomUInt(1, 4);
		if (x == 4) {
			data[i] = FaultInjector::getRandomFloat(0.0, 1000000.0);
		}
	}
#endif

	qsort(data, n, sizeof(int), compare);

	// Mínimo 50% de chances...
	left = right = n / 2;
	lc = 1;
	for (i = left; i >= 0; i--) {
		for (j = i + 1; j < n && data[j] <= data[i] + (variation * 2); j++)
			;
		if (j - i > lc) {
			lc = j - i;
			left = i;
			right = (j - 1); // A última comparação é falsa.
		}
	}

	if (lc < n2)
		return NAN;

	for (i = left; i <= right; i++) {
		result += data[i];
	}

	result = result / lc;
	return result;
}

bool WeatherStation::saveData() {

	setState(STATE_SAVE_DATA);

	ReadingData *temp = ReadingData::create(&data, &data_copy_1, &data_copy_2);

	if (!temp) {
		logger.log("DATA ERROR!");
		return false;
	}

//	printDataInfo(temp, "Temp");

	bool status = true;

	if (!temp->save(FILEPATH_DATA_1))
		status = false;
	if (!temp->save(FILEPATH_DATA_2))
		status = false;
	if (!temp->save(FILEPATH_DATA_3))
		status = false;

	/* Creates ready file */
	FILE *fp = fopen(FILEPATH_READY, "w");
	if (fp)	fclose(fp);

	free(temp);

//	ReadingData *d = ReadingData::load(FILEPATH_DATA_1);
//	printDataInfo(d, "Conferencia");
//	free(d);

	return status;
}

bool WeatherStation::send() {

	setState(STATE_SEND_DATA);

	logger.log("send() iniciada: nao implementada ainda");

	return false;
}

void WeatherStation::fatalError(ErrorType error) {
	int count;
	DigitalOut led2(LED2);

	if (error <= 0)
		return;

	while (true) {

		for (count = 0; count < 6; count++) {
			led2 = 1;
			wait(0.05);
			led2 = 0;
			wait(0.05);
		}

		for (count = 0; count < error; count++) {
			led2 = 0;
			wait(1.0);
			led2 = 1;
			wait(0.1);
			led2 = 0;
			wait(1.0);
		}
	}
}

void WeatherStation::powerMbed(PowerOpt action) {

	DigitalOut mbed(p23);

	mbed = (action == POWER_ON) ? 0 : 1;
}

void WeatherStation::powerBattery(PowerOpt action) {

	DigitalOut battery(p24);

	battery = (action == POWER_ON) ? 0 : 1;
}

void WeatherStation::powerGPS(PowerOpt action) {

	DigitalOut gps(p9);

	gps = (action == POWER_ON) ? 1 : 0;
}

void WeatherStation::powerLED(PowerOpt action, PinName pin) {

	if (pin == LED1 || pin == LED2 || pin == LED3 || pin == LED4) {

		DigitalOut led(pin);

		led = (action == POWER_ON) ? 1 : 0;
	}
}

void WeatherStation::blinkLED(PinName pin, uint8_t count, int interval) {

	if (pin == LED1 || pin == LED2 || pin == LED3 || pin == LED4) {

		DigitalOut led(pin);

		for (uint8_t i = 0; i < count; i++) {
			led = 1;
			wait_ms(interval);
			led = 0;
			wait_ms(interval);
		}
	}
}

static inline int compare(const void *n1, const void *n2) {
	return (*(float*) n1 - *(float*) n2);
}

static inline void safe_free(void *ptr) {
	if (ptr)
		free(ptr);
}

static inline void safe_fclose(FILE *fp) {
	if (fp)
		fclose(fp);
}

