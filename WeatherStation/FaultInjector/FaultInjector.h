/* 
 * File:   FaultsInjector.h
 * Author: Kleber Kruger
 *
 * Created on 11 de Junho de 2013, 09:20
 */

#ifndef FAULTINJECTOR_H
#define FAULTINJECTOR_H

#include "Utils.h"

typedef struct {
    char name[32];
    unsigned int startAddr;
    unsigned int endAddr;
    unsigned int size;
} MemoryRegion;

class FaultInjector {
public:

    static const int DEFAULT_CHANGED_BYTES = 1; // Quantidade de bytes alterados em cada injeção de falha.
    static const int DEFAULT_CHANGED_BITS = 0; // Quantidade de bits alterados dentro do byte. (1 - 8); 0 = Aleatório.

    FaultInjector();
    virtual ~FaultInjector();
    
    void injectFaults(unsigned int changedBytes, unsigned int changedBits);

private:

    unsigned int memorySize;
    
    static MemoryRegion memoryRegions[];
    
    unsigned int getByteMemory(unsigned int startAddr, unsigned int endAddr);

    void injectFaults(unsigned int addrStart, unsigned int addrEnd,
            unsigned int changedBytes, unsigned int changedBits);
};

#endif  /* FAULTSINJECTOR_H */
