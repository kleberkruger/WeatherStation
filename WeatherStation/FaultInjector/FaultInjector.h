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

#include "Utils.h"

typedef struct _MemRegion {
	const char *name;
	unsigned int startAddr;
	unsigned int endAddr;
	unsigned int size;
} MemRegion;

class FaultInjector {
public:

	typedef enum {
		CRASH_INVALID_MEMORY_ACCESS,
		CRASH_INFINITE_LOOP,
	} CrashType;

	static const int DEFAULT_CHANGED_BYTES = 1; // Number of bytes changed in each fault injection.
	static const int DEFAULT_CHANGED_BITS = 0;  // Number of bits changed in each byte: [0 <= n <= 8] (0 = random).

	FaultInjector();
	virtual ~FaultInjector();

	void injectFaults(unsigned int changedBytes, unsigned int changedBits);

	void start();

	static void crash(CrashType type);

private:

	unsigned int memorySize;
	static MemRegion memoryRegions[];

	unsigned int getByteMemory(unsigned int startAddr, unsigned int endAddr);
	void injectFaults(unsigned int addrStart, unsigned int addrEnd, unsigned int changedBytes,
			unsigned int changedBits);
};

#endif  /* FAULTSINJECTOR_H */
