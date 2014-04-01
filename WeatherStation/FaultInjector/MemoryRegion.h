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

	MemoryRegion(unsigned long startAddr, unsigned long endAddr, const char *name = NULL) :
			startAddr(startAddr), endAddr(endAddr), size(endAddr - startAddr), name(name) {}

	virtual ~MemoryRegion() {}

	inline const char *getName() const { return name; }
	inline void setName(const char *name) { this->name = name; }

	inline unsigned long getStartAddr() const { return startAddr; }
	inline unsigned long getEndAddr() const { return endAddr; }

//	inline void setStartAddr(unsigned long startAddr) { this->startAddr = startAddr; }
//	inline void setEndAddr(unsigned long endAddr) { this->endAddr = endAddr; }

	inline unsigned long getSize() const { return size; }

private:

	/**
	 * Start address
	 */
	unsigned long startAddr;

	/**
	 * End address
	 */
	unsigned long endAddr;

	/**
	 * Size
	 */
	unsigned long size;

	/**
	 * Region name
	 */
	const char *name;
};

#endif  /* MEMORYREGION_H_ */
