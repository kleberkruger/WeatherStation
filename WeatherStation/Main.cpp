/*
 =======================================================================================================================
 File       : Main.cpp
 -----------------------------------------------------------------------------------------------------------------------
 Author     : Kleber Kruger
 Email      : kleberkruger@gmail.com
 Date       : 2014-03-01
 Version    : 1.0
 Copyright  : Faculty of Computing, FACOM - UFMS
 -----------------------------------------------------------------------------------------------------------------------
 Description: Weather station firmware
 =======================================================================================================================
 */

#include "mbed.h"

/*----------------------------------------------------------------------------------------------------------------------
 Weather Station Modules
 ---------------------------------------------------------------------------------------------------------------------*/
#define FAULT_TOLERANCE_ENABLED 			// (uncomment to enable).
/*--------------------------------------------------------------------------------------------------------------------*/

#ifdef FAULT_TOLERANCE_ENABLED
#include "WeatherStationFT.h"
#else
#include "WeatherStation.h"
#endif

#include "ReadingData.h"

void printDataInfo(ReadingData *d) {

	Serial pc(USBTX, USBRX);

	pc.printf("Time: %s (%ld)\n", d->getFormatedTime(), d->getTime());

	for (int i = 0; i < ReadingData::NUMBER_OF_PARAMETERS; i++)
		pc.printf("%s: %.6f\n", d->getParameterName(i), d->getParameterValue(i));

	pc.printf("CRC: %lu\n", d->getCRC());
}


void testCRC() {

	LocalFileSystem fs("local");

	ReadingData data;

	data.setTime(1396238400);

	data.setAnemometer(100.000000);
	data.setPluviometer(2.540000);
	data.setWetting(218.925247);
	data.setTemperature(26.000000);
	data.setHumidity(66.000000);
	data.setSoilTemperaure(28.914286);
	data.setSoilHumidity(3.253969);
	data.setSolarRadiation(INFINITY);
	data.setBatteryVoltage(8.893020);

	data.setCRC(data.calculateCRC());

	printDataInfo(&data);

	data.save("/local/data.dat");
}

void test(PinName pin) {

	Serial pc(USBTX, USBRX);
	DigitalOut led(pin);

	pc.printf("Start!\n");

	for (int i = 0; i < 10; i++) {
		led = 1;
		wait_ms(200);
		led = 0;
		wait_ms(200);
	}

	pc.printf("Finished!\n");
}

int main() {

//	WeatherStation station;
//	station.start();

#ifdef FAULT_TOLERANCE_ENABLED
	WeatherStationFT::getInstance()->start();
#else
	WeatherStation::getInstance()->start();
#endif

	return 0;
}
