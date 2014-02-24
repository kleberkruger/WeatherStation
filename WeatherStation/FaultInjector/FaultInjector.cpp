/* 
 * File:   FaultsInjector.cpp
 * Author: Kleber Kruger
 * 
 * Created on 11 de Junho de 2013, 09:20
 */

#include "FaultInjector.h"

MemoryRegion FaultInjector::memoryRegions[] = {
    {"32 kB local SRAM", 0x10004000, 0x10008000 - 1, (0x10008000 - 0x10004000)},
    {"16 kB AHB SRAM1", 0x20080000, 0x20084000 - 1, (0x20084000 - 0x20080000)},
    {"END MEMORY", 0, 0, 0}
};

/**
 * Construtor da classe.
 * Configura as classes que podem ser criadas dinamicamente.
 */
FaultInjector::FaultInjector() {
    srand(time(NULL));
    memorySize = 0;
    for (int i = 0; memoryRegions[i].size; i++) {
        memorySize += memoryRegions[i].size;
    }
}

/**
 * Destrutor da classe.
 */
FaultInjector::~FaultInjector() {
}

/**
 * Função que injeta falhas no sistema.
 * 
 * @param changedBytes - quantidade de bytes alterados dentro da faixa de endereço.
 * @param changedBits  - quantidade de bits que foram alterados (0 (auto)... 8).
 */
void FaultInjector::injectFaults(unsigned int changedBytes, unsigned int changedBits) {
    unsigned int addrStart, addrEnd;
    DigitalOut led3(LED3);
    //addrStart = (unsigned) getRandomIntPositive(0x10004000, 0x10008000 - 1);
    //addrEnd = getRandomIntPositive(addrStart, 0x10008000 - 1);
    addrStart = (unsigned) getRandomIntPositive(0, memorySize - 1);
    addrEnd = getRandomIntPositive(addrStart, memorySize - 1);
    led3 = 1;
    wait(0.3);
    led3 = 0;
    injectFaults(addrStart, addrEnd, changedBytes, changedBits);
}

unsigned int FaultInjector::getByteMemory(unsigned int startAddr, unsigned int endAddr) {
    int i;
    unsigned int number, limit;
    number = getRandomIntPositive(startAddr, endAddr);
    limit = memoryRegions[0].size - 1;
    i = 0;
    while (number > limit) {
        limit += memoryRegions[i].size;
        i++;
    }
    return memoryRegions[i].startAddr + (number - limit);
}

/**
 * Função que injeta falhas no sistema.
 *
 * @param addrStart    - faixa de endereço inicial.
 * @param addrEnd      - faixa de endereço final.
 * @param changedBytes - quantidade de bytes alterados dentro da faixa de endereço.
 * @param changedBits  - quantidade de bits que foram alterados (0 (auto)... 8).
 */
void FaultInjector::injectFaults(unsigned int addrStart, unsigned int addrEnd,
        unsigned int changedBytes, unsigned int changedBits) {

    int count, temp, position;
    unsigned char *addr;
    char buffer[32];
    bool drawn[8]; // bits sorteados (para não alterar o mesmo bit mais de uma vez).
    
    if (changedBytes < 1 || changedBytes > (addrEnd - addrStart))
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
//      fprintf(fp, "Endereco inicial: %d\n", addrStart);
//      fprintf(fp, "Endereco final:   %d\n", addrEnd);
//      fprintf(fp, "Bytes alterados:  %d\n", changedBytes);
//      fprintf(fp, "Bits alterados:   %d\n\n", changedBits);

        for (int i = 0; i < changedBytes; i++) {
            char value[9];
            addr = (unsigned char *) getByteMemory(addrStart, addrEnd);
            itoa(*addr, value, 2);

            fprintf(fp, "%d\n", i + 1);
            fprintf(fp, "Endereco (em byte): %p\n", addr);
            fprintf(fp, "Valor correto:  %s\n", value);

            if (changedBits == 0)
                temp = getRandomIntPositive(1, 8); // Valor 0 altera n bits automaticamente.

            for (count = 0; count < 8; count++)
                drawn[count] = false;

            count = 0;
            while (count < temp) {
                position = getRandomIntPositive(0, 7);
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
