/*
 =======================================================================================================================
 File       : WeatherStation.cpp
 -----------------------------------------------------------------------------------------------------------------------
 Author     : Kleber Kruger
 Email      : kleberkruger@gmail.com
 Reference  : Fábio Iaione
 Date       : 2013-06-11
 Version    : 1.0
 Copyright  : Faculty of Computing, FACOM - UFMS
 -----------------------------------------------------------------------------------------------------------------------
 Description: Weather station without implementing fault tolerance
 =======================================================================================================================
 */

#include "WeatherStation.h"

DigitalOut WeatherStation::led1(LED1);
DigitalOut WeatherStation::led2(LED2);
DigitalOut WeatherStation::led3(LED3);
DigitalOut WeatherStation::led4(LED4);
DigitalOut WeatherStation::WDI(p23);
DigitalOut WeatherStation::LDBATT(p24);
DigitalOut WeatherStation::gpsPower(p9);
GPS WeatherStation::gps(p13, p14);
Serial WeatherStation::pc(USBTX, USBRX);

LocalFileSystem WeatherStation::fileSystem("local");
Ticker WeatherStation::ticker;
Pluviometer WeatherStation::pluv;
Watchdog WeatherStation::wdt;

#ifdef FAULTS_INJECTOR_MODE
FaultInjector WeatherStation::injector;
Timeout WeatherStation::timer;
#endif

WeatherStation::WeatherStation() {
	config();
}

WeatherStation::~WeatherStation() {
}

void WeatherStation::config() {

	if ((LPC_WDT->WDMOD >> 2) & 1) { // Reset por watchdog.
		log(">>>> config(): Reset por watchdog.");
		flashLed(led3);
		return;
	}

	// Reset por push-button ou Power On.
	log(">>>> config(): Reset por push-button ou power on.");
	flashLed(led1);

	ticker.attach(&loadWatchdog, 5.0); 	// Função resetWatchdog chamada a cada 5s.
	wdt.kick(6.0); 						// Tempo de 6s do watchdog.
	WDI = 1; 							// Desliga mbed e Bat.
	LDBATT = 1;
	wait(0.5);
	WDI = 0; 							// Liga mbed.
	gpsPower = 0; 						// Desabilita GPS.
	PHY_PowerDown(); 					// Desativa ethernet para reduzir consumo.
	pluv.resetCount(); 					// Zera o contador de pulsos recebidos do pluviômetro.

	configRTC(); 						// Sincronizar RTC e outras coisas com o site.
	writeConfigData();
}

void WeatherStation::flashLed(DigitalOut led) {
	for (int i = 0; i < 6; i++) {
		led = 1;
		wait(0.1);
		led = 0;
		wait(0.1);
	}
}

void WeatherStation::configRTC() {
	int year, mounth;
	struct tm t;
	Timer tm;

	pc.printf("Acertar RTC? (qualquer tecla = sim)\n");
	tm.start();

	while (!pc.readable() && tm.read() < 2)
		;

	if (pc.readable()) {

		while (pc.getc() != 0x0D)
			;

		pc.printf("Entre com dd/mm/aaaa hh:mm\n");
		t.tm_sec = 0;
		pc.scanf("%d/%d/%d %d:%d", &t.tm_mday, &mounth, &year, &t.tm_hour, &t.tm_min);
		t.tm_year = year - 1900;
		t.tm_mon = mounth - 1;
		set_time(mktime(&t));

	} else {
		//set_time(1256729737); //28 Oct 2009 11:35:37
	}
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

	gpsPower = 1; // Habilita GPS.
	tm.start();

	do {
		if (gps.sample()) {
			log("Lock OK");
			gpsPower = 0; // Desabilita GPS
			return true;
		} else {
			log("No lock.");
		}
		wait(1.0);
	} while (!gps.lock && tm.read() < 3);
	gpsPower = 0; // Desabilita GPS

	return false;
}

void WeatherStation::start() {

	ReadingData data;

	while (true) {

		if (checkTime(ACTION_READ, READ_UNIT, READ_INTERVAL)) {
			readSensors(&data);
			saveData(&data);

			FILE *fp = fopen("/local/pronto.txt", "w"); // Se o arquivo existir, grava por cima
			if (fp)
				fclose(fp);

#ifdef FAULTS_INJECTOR_MODE
			led2 = 1;
			wait(0.3);
			led2 = 0;
			timer.attach(&generateFaults, getRandomFloat(0.1, 9.9));
#endif
		}
		if (checkTime(ACTION_SEND, 0, 0))
			send();
		if (READ_UNIT == READ_UNIT_MIN)
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

bool WeatherStation::checkTime(ActionType action, int unit, int interval) {
	struct tm * timest;
	time_t time_sec;
	static char readFlag = false;
	static char sendFlag = false;
	int valtm;

	time(&time_sec);
	timest = localtime(&time_sec);

	switch (action) {

		case ACTION_READ:
			if (unit == READ_UNIT_MIN) {
				valtm = timest->tm_min;
			} else {
				valtm = timest->tm_sec;
			}
			if ((valtm % interval) == 0) {
				if (!readFlag) {
					readFlag = true;
					return true;
				} else
					return false;
			} else {
				readFlag = false;
				return false;
			}

		case ACTION_SEND:
			if ((timest->tm_min == SEND_TIME_MIN) && (timest->tm_hour == SEND_TIME_HOUR)) {
				if (!sendFlag) {
					sendFlag = true;
					return true;
				} else
					return false;
			} else {
				sendFlag = false;
				return false;
			}
	}

	return false;
}

void WeatherStation::readSensors(ReadingData *data) {

	int count;
	Anemometer vel(p21);
	Wetting wet(p19, p26, p25);

	wet.config(100, 1000, 1, 3000); 	// 100kohms, 1000us, 1kohms, 3000k = 3Mohms

	LDBATT = 0; 						// Liga Vbat e 5Vc
	wait_ms(200); 						// Tempo para Vbat estabilizar, pois o acionamento do MOSFET é lento (+/- 23ms)

	log("readSensors() iniciada");

	SHTx::SHT15 sensorTE_UR(p29, p30); // DATA, SCK
	sensorTE_UR.setOTPReload(false);
	sensorTE_UR.setResolution(true);

	data->data_hora = time(NULL);

	sensorTE_UR.update();
	sensorTE_UR.setScale(false);

	data->param[0].f = sensorTE_UR.getTemperature();
	data->param[1].f = sensorTE_UR.getHumidity();
	data->param[2].f = pluv.read() * INC_PLUV;
	data->param[3].f = vel.read() * CONV_ANEM;
	data->param[4].f = readSensor(1.1, 0, 5.54, 1.0, 16); 				// Umidade do solo [raiz de epsilon]
	data->param[5].f = readSensor(5, 0.320512821, 50, 3.205128205, 17); // Temperatura do solo [C]
	data->param[6].f = readSensor(0, 0, 1500, 1.5, 18); 				// Irradiação solar [W/m2]
	data->param[7].f = wet.read() / 1000; 								// Molhamento [kohms]
	data->param[8].f = readSensor(0, 0, 15.085714286, 3.3, 15); 		// Tensão da bateria [V]
	data->crc.l = data->data_hora; 										// Calcula CRC

	for (count = 0; count < NUMBER_OF_PARAMETERS; count++)
		data->crc.l = data->crc.l ^ data->param[count].l;

	LDBATT = 1; 						// Desliga Vbat e 5Vc
	log("readSensors() concluida");
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

float WeatherStation::calculateAverage(float data[], int n, int n2, float variation) {

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
			right = (j - 1); // A última comparacao é falsa.
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

void WeatherStation::saveData(ReadingData *data) {
	log("saveData() iniciada");

	FILE *fp = fopen("/local/dados.dat", "ab");

	if (!fp)
		fatalError(ERROR_OPEN_FILE);

	fwrite(data, sizeof(ReadingData), 1, fp);

	fclose(fp);
	log("saveData() concluida");
}

void WeatherStation::send() {
	log("send() iniciada: nao implementada ainda");
}

void WeatherStation::fatalError(ErrorType error) {
	int count;

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

void WeatherStation::log(const char *msg) {
	char buffer[32];
	FILE *fp = fopen("/local/log.txt", "a");

	if (!fp)
		fatalError(ERROR_OPEN_FILE);

	time_t seconds = time(NULL);
	strftime(buffer, 32, "%d/%m/%Y %H:%M:%S", localtime(&seconds));
#ifdef SERIAL_DEBUG
	pc.printf("%s >>> %s\n", buffer, msg);
#endif
	fprintf(fp, "%s >>> %s\n", buffer, msg);
	fclose(fp);
}

void WeatherStation::loadWatchdog() {
	wdt.kick();
	led4 = 1;
	wait(0.03);
	led4 = 0;
}
