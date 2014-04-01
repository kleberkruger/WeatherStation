/*
 =======================================================================================================================
 File       : FaultInjector.h
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

#ifndef FAULTINJECTOR_H
#define FAULTINJECTOR_H

#include "mbed.h"

#include "MemoryRegion.h"
#include "Logger.h"

//#include "WeatherStation.h"

#define FILEPATH_FAULTS					"/local/faults.txt"

class FaultInjector {
public:

	typedef enum {
		CRASH_INVALID_MEMORY_ACCESS, CRASH_INFINITE_LOOP,
	} CrashType;

	static const int DEFAULT_CHANGED_BYTES = 100; // Number of bytes changed in each fault injection.
	static const int DEFAULT_CHANGED_BITS = 0;  // Number of bits changed in each byte: [0 <= n <= 8] (0 = random).

	FaultInjector();
	virtual ~FaultInjector();

	void injectFaults(unsigned long changedBytes, uint8_t changedBits);

	void start(float t);
	void start(float tMin, float tMax);

	static void crash(CrashType type);

	/**
	 * Generates a random number (unsigned int) between the minimum
	 * and maximum range.
	 *
	 * @param min - minimum
	 * @param max - maximum
	 */
	static unsigned int getRandomUInt(unsigned int min, unsigned int max);

	/**
	 * Generates a random number (float) between the minimum and
	 * maximum range.
	 *
	 * @param min - minimum
	 * @param max - maximum
	 */
	static float getRandomFloat(float min, float max);

	/**
	 * Generates a random number (double) between the minimum and
	 * maximum range.
	 *
	 * @param min - minimum
	 * @param max - maximum
	 */
	static double getRandomDouble(double min, double max);

private:

	unsigned long memorySize;

	MemoryRegion **memoryRegions;

	Timeout timer;

	Logger logger;

	unsigned long getByteMemory(unsigned long startAddr, unsigned long endAddr);

	void injectFaults(unsigned long addrStart, unsigned long addrEnd, unsigned long changedBytes, uint8_t changedBits);

	void generateFaults();

	/**
	 * C++ version 0.4 char* style "itoa":
	 * Written by Lukás Chmela
	 * Released under GPLv3.
	 */
	static char *itoa(int value, char *result, int base);
};

#endif  /* FAULTSINJECTOR_H */
