/*
 =======================================================================================================================
 File       : MemoryRegion.h
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

#ifndef MEMORYREGION_H_
#define MEMORYREGION_H_

class MemoryRegion {
public:

	MemoryRegion(unsigned int startAddr, unsigned int endAddr) :
			startAddr(startAddr), endAddr(endAddr), size(endAddr - startAddr) {}

	MemoryRegion(const char *name = NULL, unsigned int startAddr, unsigned int endAddr) :
			name(name), startAddr(startAddr), endAddr(endAddr), size(endAddr - startAddr) {}

	virtual ~MemoryRegion() {}

	inline char *getName() const { return name; }
	inline void setName(char *name) { this->name = name; }

	inline unsigned int getStartAddr() const { return startAddr; }
	inline void setStartAddr(unsigned int startAddr) { this->startAddr = startAddr; }

	inline unsigned int getEndAddr() const { return endAddr; }
	inline void setEndAddr(unsigned int endAddr) { this->endAddr = endAddr; }

private:

	const char *name;
	unsigned int startAddr;
	unsigned int endAddr;
	unsigned int size;
};

#endif  /* MEMORYREGION_H_ */
