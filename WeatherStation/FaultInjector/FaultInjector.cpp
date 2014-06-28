/*
 =======================================================================================================================
 File       : FaultInjector.cpp
 -----------------------------------------------------------------------------------------------------------------------
 Author     : Kleber Kruger
 Email      : kleberkruger@gmail.com
 Date       : 2013-06-15
 Version    : 1.0
 Copyright  : Faculty of Computing, FACOM - UFMS
 -----------------------------------------------------------------------------------------------------------------------
 Description: Fault Injector System
 =======================================================================================================================
 */

#include "FaultInjector.h"

/**
 * Constructor
 */
FaultInjector::FaultInjector() :
		logger(FILEPATH_FAULTS, false) {

	int n = 1, i = 0;

	memoryRegions = (MemoryRegion **) calloc(1, sizeof(MemoryRegion *));

//	memoryRegions[i++] = new MemoryRegion(0x00040000, 0x00080000 - 1, "512 kB on-chip flash");
	memoryRegions[i++] = new MemoryRegion(0x10004000, 0x10008000 - 1, "32 KB local SRAM");
	memoryRegions[i++] = new MemoryRegion(0x2007C000, 0x20080000 - 1, "16 KB AHB SRAM0");
	memoryRegions[i++] = new MemoryRegion(0x20080000, 0x20084000 - 1, "16 kB AHB SRAM1");

	srand(time(NULL));

	memorySize = 0;

	for (i = 0; i < n; i++)
		memorySize += memoryRegions[i]->getSize();
}

/**
 * Destructor
 */
FaultInjector::~FaultInjector() {

	int n = 1;

	for (int i = 0; i < n; i++)
		delete (memoryRegions[i]);

	free(memoryRegions);
}

void FaultInjector::start(float t) {
	timer.attach(this, &FaultInjector::generateFaults, t);
}

void FaultInjector::start(float tMin, float tMax) {
	timer.attach(this, &FaultInjector::generateFaults, getRandomFloat(tMin, tMax));
}

void FaultInjector::generateFaults() {

//	WeatherStation::getInstance()->getLogger()->log("Inserting faults");

	Logger l = Logger("/local/log.txt", true);

	l.log("Inserting %u faults:", DEFAULT_CHANGED_BYTES);

	int a = 50;

	l.log("Address A: %p", &a);

	DigitalOut led4(LED4);

	led4 = 1;
	injectFaults(DEFAULT_CHANGED_BYTES, DEFAULT_CHANGED_BITS);
	wait(0.5);
	led4 = 0;
}

/**
 * Inject faults in the system.
 * 
 * @param changedBytes - number of bytes changed in within the address range
 * @param changedBits  - number of bits changed (0 (auto)... 8)
 */
void FaultInjector::injectFaults(unsigned long changedBytes, uint8_t changedBits) {

	unsigned long addrStart, addrEnd;

	addrStart = getRandomUInt(0, memorySize - 1);
	addrEnd = getRandomUInt(addrStart, memorySize - 1);

	injectFaults(addrStart, addrEnd, changedBytes, changedBits);
}

unsigned long FaultInjector::getByteMemory(unsigned long startAddr, unsigned long endAddr) {

	unsigned long number, limit;
	int i = 0;

	number = getRandomUInt(startAddr, endAddr);
	limit = memoryRegions[0]->getSize() - 1;

	while (number > limit) {
		limit += memoryRegions[i]->getSize();
		i++;
	}

	return memoryRegions[i]->getStartAddr() + (number - limit);
}

/**
 * Inject faults in the system.
 *
 * @param startAddr    - start address
 * @param endAddr      - end address
 * @param changedBytes - number of bytes changed in within the address range
 * @param changedBits  - number of bits changed (0 (auto)... 8)
 */
void FaultInjector::injectFaults(unsigned long startAddr, unsigned long endAddr, unsigned long changedBytes,
		uint8_t changedBits) {

	int count, temp, position;
	unsigned char *addr;
//	char buffer[32];
	bool drawn[8]; // bits sorteados (para não alterar o mesmo bit mais de uma vez).

	if (changedBytes < 1 || changedBytes > (endAddr - startAddr))
		changedBytes = DEFAULT_CHANGED_BYTES;

	if (changedBits > 8) // 0 para automático, 8 para alterar todos os bits
		changedBits = DEFAULT_CHANGED_BITS;

	temp = changedBits;

//	Logger l = Logger("/local/log.txt", true);

//	LocalFileSystem local("local");
//	FILE *fp = fopen("/local/falhas.txt", "a");
//	if (fp != NULL) {
//		time_t seconds = time(NULL);
//		strftime(buffer, 32, "%d/%m/%Y %H:%M:%S", localtime(&seconds));
//      fprintf(fp, "start address: %d\n", startAddr);
//      fprintf(fp, "end address: %d\n", endAddr);
//      fprintf(fp, "changed bytes: %d\n", changedBytes);
//      fprintf(fp, "changed bits: %d\n\n", changedBits);

	for (unsigned long i = 0; i < changedBytes; i++) {

		char value[9];
		addr = (unsigned char *) getByteMemory(startAddr, endAddr);

		itoa(value, *addr, 2);

//		logger.log("%lu\n\tAddress: %p\n\tCorrect value: %s", i + 1, addr, value);
//		l.log("%lu\n\tAddress: %p\n\tCorrect value: %s", i + 1, addr, value);

		if (changedBits == 0)
			temp = getRandomUInt(1, 8); // Valor 0 altera n bits automaticamente.

		for (count = 0; count < 8; count++)
			drawn[count] = false;

		count = 0;
		while (count < temp) {
			position = getRandomUInt(0, 7);
			if (drawn[position] != true) {
				drawn[position] = true;
				count++;
				(*addr) ^= (1 << position); // Altera um bit de um endereço (byte) de memória.
			}
		}

		itoa(value, *addr, 2);

//		fprintf(fp, "Valor alterado: %s\n\n", value);
//		logger.log("\tChanged value: %s\n", value);
//		l.log("\tChanged value: %s\n", value);
	}

//		fclose(fp);
//	}

}

void FaultInjector::crash(CrashType crash) {

	if (crash == CRASH_INVALID_MEMORY_ACCESS) {
		char *ptr = 0;
		*ptr = 0;
	}

	while (true);
}

/**
 * Generates a random number (unsigned int) between the minimum
 * and maximum range.
 *
 * @param min - minimum
 * @param max - maximum
 */
unsigned int FaultInjector::getRandomUInt(unsigned int min, unsigned int max) {
	int k;
	double d;
	d = (double) rand() / ((double) RAND_MAX + 1);
	k = d * (max - min + 1);
	return min + k;
}

/**
 * Generates a random number (float) between the minimum and
 * maximum range.
 *
 * @param min - minimum
 * @param max - maximum
 */
float FaultInjector::getRandomFloat(float min, float max) {
	int rd = (int) getRandomDouble((int) (min * 10), (int) (max * 10) + 1);
	return (float) rd / 10;
}

/**
 * Generates a random number (double) between the minimum and
 * maximum range.
 *
 * @param min - minimum
 * @param max - maximum
 */
double FaultInjector::getRandomDouble(double min, double max) {
	double d, k;
	d = (double) rand() / ((double) RAND_MAX + 1);
	k = d * (max - min + 1);
	return min + k;
}

/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 */
char* FaultInjector::itoa(char *result, int value, int base) {

	// Check that the base if valid
	if (base < 2 || base > 36) {
		*result = '\0';
		return result;
	}

	char *ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35
				+ (tmp_value - value * base)];
	} while (value);

	// Apply negative sign
	if (tmp_value < 0)
		*ptr++ = '-';
	*ptr-- = '\0';

	while (ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr-- = *ptr1;
		*ptr1++ = tmp_char;
	}

	return result;
}
