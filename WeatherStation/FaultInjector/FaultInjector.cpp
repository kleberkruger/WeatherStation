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

MemRegion FaultInjector::memoryRegions[] =
{
		{ "32 KB local SRAM", 0x10004000, 0x10008000 - 1, (0x10008000 - 0x10004000) },
		{ "16 KB AHB SRAM", 0x20080000,	0x20084000 - 1, (0x20084000 - 0x20080000) },
		{ NULL, 0, 0, 0 }
};

/**
 * Constructor
 */
FaultInjector::FaultInjector() {

	memorySize = 0;

	srand(time(NULL));

	for(int i = 0; memoryRegions[i].size; i++)
		memorySize += memoryRegions[i].size;
}

/**
 * Destructor
 */
FaultInjector::~FaultInjector() {

	/* Nothing to do */
}

void FaultInjector::start() {

	/* TODO: Implement this method */
}

/**
 * Inject faults in the system.
 * 
 * @param changedBytes - number of bytes changed in within the address range
 * @param changedBits  - number of bits changed (0 (auto)... 8)
 */
void FaultInjector::injectFaults(unsigned int changedBytes, unsigned int changedBits) {

	unsigned int addrStart, addrEnd;
	DigitalOut led3(LED3);

	addrStart = (unsigned) getRandomUInt(0, memorySize - 1);
	addrEnd = getRandomUInt(addrStart, memorySize - 1);
	led3 = 1;
	wait(0.3);
	led3 = 0;
	injectFaults(addrStart, addrEnd, changedBytes, changedBits);
}

unsigned int FaultInjector::getByteMemory(unsigned int startAddr, unsigned int endAddr) {

	unsigned int number, limit;
	int i = 0;

	number = getRandomUInt(startAddr, endAddr);
	limit = memoryRegions[0].size - 1;

	while (number > limit) {
		limit += memoryRegions[i].size;
		i++;
	}

	return memoryRegions[i].startAddr + (number - limit);
}

/**
 * Inject faults in the system.
 *
 * @param startAddr    - start address
 * @param endAddr      - end address
 * @param changedBytes - number of bytes changed in within the address range
 * @param changedBits  - number of bits changed (0 (auto)... 8)
 */
void FaultInjector::injectFaults(unsigned int startAddr, unsigned int endAddr, unsigned int changedBytes,
		unsigned int changedBits) {

	int count, temp, position;
	unsigned char *addr;
	char buffer[32];
	bool drawn[8]; // bits sorteados (para não alterar o mesmo bit mais de uma vez).

	if (changedBytes < 1 || changedBytes > (endAddr - startAddr))
		changedBytes = DEFAULT_CHANGED_BYTES;

	if (changedBits > 8) // 0 para automático, 8 para alterar todos os bits
		changedBits = DEFAULT_CHANGED_BITS;

	temp = changedBits;

	LocalFileSystem local("local");
	FILE *fp = fopen("/local/falhas.txt", "a");

	if (fp != NULL) {
		time_t seconds = time(NULL);
		strftime(buffer, 32, "%d/%m/%Y %H:%M:%S", localtime(&seconds));

		fprintf(fp, "%s\n", buffer);
//      fprintf(fp, "start address: %d\n", startAddr);
//      fprintf(fp, "end address: %d\n", endAddr);
//      fprintf(fp, "changed bytes: %d\n", changedBytes);
//      fprintf(fp, "changed bits: %d\n\n", changedBits);

		for (unsigned int i = 0; i < changedBytes; i++) {
			char value[9];
			addr = (unsigned char *) getByteMemory(startAddr, endAddr);
			itoa(*addr, value, 2);

			fprintf(fp, "%d\n", i + 1);
			fprintf(fp, "address (byte): %p\n", addr);
			fprintf(fp, "correct value: %s\n", value);

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
			itoa(*addr, value, 2);
			fprintf(fp, "Valor alterado: %s\n\n", value);
		}

		fclose(fp);
	}
}

void FaultInjector::crash(CrashType crash) {

	if (crash == CRASH_INVALID_MEMORY_ACCESS) {
		char *ptr = 0;
		*ptr = 0;
	}

	while (true)
		;
}
