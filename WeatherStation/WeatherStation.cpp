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

//const char *WeatherStation::FILESYSTEM_NAME = "local";

//const char *WeatherStation::CONFIG_HEADER_TXT = "# Weather station with implementing fault tolerance.";

const char *WeatherStation::DEFAULT_SEND_TIME = "11:00:00";

static inline void safe_free(void *ptr);
static inline void safe_fclose(FILE *fp);

WeatherStation::WeatherStation() :
		fs(FILESYSTEM_NAME), cfg(FILEPATH_CONFIG, CONFIG_HEADER_TXT), logger(FILEPATH_LOG), gps(p13, p14) {

	init();
}

WeatherStation::~WeatherStation() {

	/* Nothing to do */
}

void WeatherStation::init() {

	/* Reset by watchdog */
	if ((LPC_WDT->WDMOD >> 2) & 1) {

		logger.log("Reset by watchdog.");

		/* Blink LED 2 */
		blinkLED(LED2, 10, 100);

		/* Read state value from configuration file and go to state */
		goToState(atoi(cfg.getValue("state")));

	} else {

		logger.log("Reset by power-button.");

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

void WeatherStation::readConfigFromFile() {

//	int value_int;
//	float value_flt;
//	char *value_str;
//
//	/*
//	 * Read watchdog time
//	 */
//	if ((value_str = cfg.getValue("watchdogTime")))
//		watchdogTime = ((value_flt = atoff(value_str)) > 0) ? value_flt : DEFAULT_WATCHDOG_TIME;
//	else
//		watchdogTime = DEFAULT_WATCHDOG_TIME;
//
//	/*
//	 * Read number of readings
//	 */
//	if ((value_str = cfg.getValue("numberOfReadings")))
//		numberReadings = ((value_int = atoi(value_str)) > 0) ? value_int : DEFAULT_READINGS_AMOUNT;
//	else
//		numberReadings = DEFAULT_READINGS_AMOUNT;
//
//	/*
//	 * Read minimum correct readings
//	 */
//	if ((value_str = cfg.getValue("minCorrectReadings")))
//		minCorrectReadings = ((value_int = atoi(value_str)) > 0) ? value_int : DEFAULT_READINGS_MIN_CORRECT;
//	else
//		minCorrectReadings = DEFAULT_READINGS_MIN_CORRECT;
//
//	/*
//	 * Read interval of readings
//	 */
//	if ((value_str = cfg.getValue("readingsInterval")))
//		readingInterval = ((value_int = atoi(value_str)) > 0) ? value_int : DEFAULT_READINGS_INTERVAL;
//	else
//		readingInterval = DEFAULT_READINGS_INTERVAL;
//
//	/*
//	 * Read reading unit
//	 */
//	if ((value_str = cfg.getValue("readingsUnit")))
//		readingUnit = (atoi(value_str) > 0) ? WeatherStationConfig::READING_UNIT_MIN : WeatherStationConfig::READING_UNIT_SEC;
//	else
//		readingUnit = WeatherStationConfig::READING_UNIT_SEC;
//
//	/*
//	 * Read send time
//	 */
//	const char *tm_str;
//
//	if ((value_str = cfg.getValue("sendTime")))
//		tm_str = (checkTime(value_str)) ? value_str : DEFAULT_SEND_TIME;
//	else
//		tm_str = DEFAULT_SEND_TIME;
//
//	sscanf(tm_str, "%d:%d:%d", &sendTime.tm_hour, &sendTime.tm_min, &sendTime.tm_sec);
}

void WeatherStation::config() {

	setState(STATE_NOT_CONFIGURED);

	logger.log("config() - initializing configuration.");

	readConfigFromFile();

	powerMbed(POWER_ON); 	// Power on mbed
	powerGPS(POWER_OFF); 	// Power off GPS

	PHY_PowerDown(); 		// Disable ethernet to reduce consumption.

	configRTC();

	wdt.kick(watchdogTime);	// Configure watchdog timer.

	logger.log("config() - successfully configured.");
}

void WeatherStation::configRTC() {

	set_time(1256729737); 	// Set time to: 28 October 2009 11:35:37 /* XXX */

//	int year, mounth;
//	struct tm t;
//	Timer tm;
//	Serial pc(USBTX, USBRX);
//
//	pc.printf("Acertar RTC? (qualquer tecla = sim)\n");
//	tm.start();
//
//	while (!pc.readable() && tm.read() < 2)
//		;
//
//	if (pc.readable()) {
//
//		while (pc.getc() != 0x0D)
//			;
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

void WeatherStation::writeConfigData() {

	FILE *fp = fopen("/local/config.txt", "w"); // Se o arquivo existir, grava por cima.

	if (!fp)
		fatalError(ERROR_OPEN_FILE);

	fprintf(fp, "Parametro \t Unidade \t Valor minimo valido \t Valor maximo valido\n");
	fprintf(fp, "Temperatura do ar \t graus C \t ? \t ?\n");
	fprintf(fp, "Umidade do ar \t %% U.R. \t ? \t ?\n");
	fprintf(fp, "Pluviometria \t mm \t 0.0 \t ?\n");
	fprintf(fp, "Velocidade do vento \t m/s \t 0.0 \t ?\n");
	fprintf(fp, "Umidade do solo \t Raiz de epsilon \t 1.1 \t 5.54\n");
	fprintf(fp, "Temperatura do solo \t graus C \t 5.0 \t 50.0\n");
	fprintf(fp, "Irradiacao solar \t W/m2 \t 0.0 \t 1500\n");
	fprintf(fp, "Molhamento foliar \t kohms \t 1.0 \t 3000\n");
	fprintf(fp, "Tensao da bateria \t Volts \t 0.0 \t 15.0\n\n");
	fprintf(fp, "Numero de serie = \t\t %d\n", SERIAL_NUMBER);
	fprintf(fp, "Versao do firmware = \t %s \t %s \t %s\n", __TIME__, __DATE__, __FILE__);

#ifdef GPS_ENABLE
	if (readGPS())
		fprintf(fp, "Latitude= \t %f \t Longitude= \t %f\n", gps.latitude, gps.longitude);
	else
		fprintf(fp, "Latitude= \t ? \t Longitude= \t ?\n");
#endif

	fclose(fp);
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

void WeatherStation::start() {

	int attempts;
	FILE *fp;

	while (true) {

		setState(STATE_CONFIGURED);

		/* Reload watchdog and blink LED 3 */
		wdt.kick();

		blinkLED(LED4, 1, 200);

		if (isTimeToRead()) {
			readSensors();

			for (attempts = 0; !saveData() && attempts < 3; attempts++)
				;

			if (attempts < 3)
				setState(STATE_DATA_SAVED);

			if ((fp = fopen(FILEPATH_READY, "w"))) // Se o arquivo existir, grava por cima
				fclose(fp);

#ifdef FAULTS_INJECTOR_MODE
			DigitalOut led2(LED2);
			led2 = 1;
			wait(0.3);
			led2 = 0;
			timer.attach(this, &WeatherStation::generateFaults, getRandomFloat(0.1, 9.9));
#endif
		}

		if (isTimeToSend()) {
			send();
		}

		if (readingInterval >= 60)
			Sleep();
		else
			wait(0.5);
	}
}

#ifdef FAULTS_INJECTOR_MODE
void WeatherStation::generateFaults() {
	injector.injectFaults(FaultInjector::DEFAULT_CHANGED_BYTES, FaultInjector::DEFAULT_CHANGED_BITS);
}
#endif

bool WeatherStation::isTimeToRead() {

	struct tm *timest;
	time_t time_sec;
	static bool reading = false;
	int tmval;

	time(&time_sec);
	timest = localtime(&time_sec);

	tmval = (readingInterval >= 60) ? timest->tm_min : timest->tm_sec;

	if (tmval % readingInterval)
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

	if ((timest->tm_hour != sendTime.tm_hour) || (timest->tm_min != sendTime.tm_min))
		sending = false;
	else if (!sending)
		sending = true;
	else
		return false;

	return sending;
}

void WeatherStation::readSensors() {

	int count;
	float samples[numberReadings];
	Anemometer vel(p21);
	Wetting wet(p19, p26, p25);

	wet.config(100, 1000, 1, 3000); 	// 100kohms, 1000us, 1kohms, 3000k = 3Mohms

	setState(STATE_READ_SENSORS);

	logger.log("readSensors() iniciada");

	powerBattery(POWER_ON); 	// Liga Vbat e 5Vc
	wait_ms(200); 	// Tempo para Vbat estabilizar, pois o acionamento do MOSFET é lento (+/- 23ms)

	SHTx::SHT15 sensorTE_UR(p29, p30); 	// DATA, SCK
	sensorTE_UR.setOTPReload(false);
	sensorTE_UR.setResolution(true);

	data.setTime(time(NULL));

	sensorTE_UR.update();
	sensorTE_UR.setScale(false);

	for (count = 0; count < numberReadings; count++)
		samples[count] = sensorTE_UR.getTemperature();
	data.setTemperature(avg(samples, numberReadings, minCorrectReadings, 5));

	for (count = 0; count < numberReadings; count++)
		samples[count] = sensorTE_UR.getHumidity();
	data.setHumidity(avg(samples, numberReadings, minCorrectReadings, 5));

	for (count = 0; count < numberReadings; count++)
		samples[count] = pluv.read();
	data.setPluviometer(avg(samples, numberReadings, minCorrectReadings, 5));

	for (count = 0; count < numberReadings; count++)
		samples[count] = vel.read();
	data.setAnemometer(avg(samples, numberReadings, minCorrectReadings, 5));

	// Umidade do solo [raiz de epsilon]
	for (count = 0; count < numberReadings; count++)
		samples[count] = readSensor(1.1, 0, 5.54, 1.0, 16);
	data.setSoilHumidity(avg(samples, numberReadings, minCorrectReadings, 5));

	// Temperatura do solo [C]
	for (count = 0; count < numberReadings; count++)
		samples[count] = readSensor(5, 0.320512821, 50, 3.205128205, 17);
	data.setSoilTemperaure(avg(samples, numberReadings, minCorrectReadings, 5));

	// Irradiação solar [W/m2]
	for (count = 0; count < numberReadings; count++)
		samples[count] = readSensor(0, 0, 1500, 1.5, 18);
	data.setSolarRadiation(avg(samples, numberReadings, minCorrectReadings, 5));

	// Molhamento [kohms]
	for (count = 0; count < numberReadings; count++)
		samples[count] = wet.read() / 1000;
	data.setWetting(avg(samples, numberReadings, minCorrectReadings, 5));

	// Tensão da bateria [V]
	for (count = 0; count < numberReadings; count++)
		samples[count] = readSensor(0, 0, 15.085714286, 3.3, 15);
	data.setBatteryVoltage(avg(samples, numberReadings, minCorrectReadings, 5));

	// Calcula CRC
	data.setCRC(data.calculateCRC());

	memcpy(&data, &data_copy_1, sizeof(ReadingData));
	memcpy(&data, &data_copy_2, sizeof(ReadingData));

	powerBattery(POWER_OFF); 	// Desliga Vbat e 5Vc
	logger.log("readSensors() concluida");
}

float WeatherStation::readSensor(float v_ini_par, float v_ini_volts, float v_fim_par, float v_fim_volts, int num_pino) {

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

int WeatherStation::compare(const void *n1, const void *n2) {
	return (*(float*) n1 - *(float*) n2);
}

bool WeatherStation::saveData() {

	setState(STATE_SAVE_DATA);

	FILE *fp1 = fopen(FILEPATH_DATA_1, "ab");
	FILE *fp2 = fopen(FILEPATH_DATA_2, "ab");
	FILE *fp3 = fopen(FILEPATH_DATA_3, "ab");

	bool status = (fp1 && fp2 && fp3) ? true : false;

	if (status) {

		ReadingData *temp = ReadingData::create(&data, &data_copy_1, &data_copy_2);

		if (temp == NULL)
			status = false;

		else {

			fwrite(temp, sizeof(ReadingData), 1, fp1);
			fwrite(temp, sizeof(ReadingData), 1, fp2);
			fwrite(temp, sizeof(ReadingData), 1, fp3);

			free(temp);
		}
	}

	safe_fclose(fp1);
	safe_fclose(fp2);
	safe_fclose(fp3);

	return status;
}

void WeatherStation::send() {

	setState(STATE_SEND_DATA);

	logger.log("send() iniciada: nao implementada ainda");
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
		cfg.save();
		this->state = state_copy_1 = state_copy_2 = state;

		logger.log("set state: %d.", state);
	}
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

static inline void safe_free(void *ptr) {
	if (ptr)
		free(ptr);
}

static inline void safe_fclose(FILE *fp) {
	if (fp)
		fclose(fp);
}
