/*
 =======================================================================================================================
 File       : Main.cpp
 -----------------------------------------------------------------------------------------------------------------------
 Author     : Kleber Kruger
 Email      : kleberkruger@gmail.com
 Date       : 2014-04-02
 Version    : 1.0
 Copyright  : Faculty of Computing, FACOM - UFMS
 -----------------------------------------------------------------------------------------------------------------------
 Description: Fault-Injection Monitor System
 =======================================================================================================================
 */

#include "TestMonitor.h"

using namespace std;

#include <cstdio>
#include <cstring>

//#define IOPIN0         (*((volatile unsigned long *) 0x10040b010))// assim funciona!

//int var __attribute__((at(0x10040b010)));

/**
 * Creates reading data to test this system.
 * 
 * @return 
 */
bool createReadingData() {

    ReadingData data, data_copy_1;

    data.setTime(1396238400);

    for (int i = 0; i < ReadingData::NUMBER_OF_PARAMETERS; i++)
        data.setParameterValue(i, 25);

    data.setCRC(data.calculateCRC());

    printf("AC Time: %ld\n", data.getTime());
    for (int i = 0; i < ReadingData::NUMBER_OF_PARAMETERS; i++)
        printf("AC %s: %.6f\n", data.getParameterName(i), data.getParameterValue(i));
    printf("AC CRC: %ld\n\n", data.getCRC());

    memcpy(&data_copy_1, &data, sizeof (ReadingData));

    printf("AD Time: %ld\n", data.getTime());
    for (int i = 0; i < ReadingData::NUMBER_OF_PARAMETERS; i++)
        printf("AD %s: %.6f\n", data.getParameterName(i), data.getParameterValue(i));
    printf("AD CRC: %ld\n\n", data.getCRC());

    printf("B Time: %ld\n", data_copy_1.getTime());
    for (int i = 0; i < ReadingData::NUMBER_OF_PARAMETERS; i++)
        printf("B %s: %.6f\n", data_copy_1.getParameterName(i), data_copy_1.getParameterValue(i));
    printf("B CRC: %ld\n\n", data_copy_1.getCRC());

    if (!data.save("test_t"))
        return false;

    ReadingData *data_a = ReadingData::load("test_t");

    printf("Conf Time: %ld\n", data_a->getTime());
    for (int i = 0; i < ReadingData::NUMBER_OF_PARAMETERS; i++)
        printf("Conf %s: %.6f\n", data_a->getParameterName(i), data_a->getParameterValue(i));
    printf("Conf CRC: %ld\n\n", data_a->getCRC());

    return data.save("./resources/test.dat");
}

void testTime() {

    char time_str[32];

    //    time_t t = time(NULL);
    time_t t = 1396238400;
    struct tm *timeinfo = localtime(&t);

    sprintf(time_str, "%s", "kkkkkkkkkkkkkkkkkkkkkkkkkkkkkk");

    strftime(time_str, 32, "%F | %T", timeinfo);

    printf("current time: %lu\n", t);

    printf("%s | %-9s | %s\n", time_str, "(Log)", "Message 1.");
    printf("%s | %-9s | %s\n", time_str, "(Log)", "Message 2.");
}

#include <stdio.h>

#include <iostream>
#include <iomanip>

static inline int compare(const void *n1, const void *n2) {
    return (*(float*) n1 - *(float*) n2);
}

float avg(float data[], int n, int n2, float variation) {

    int i, j, left, right, lc;
    float result = 0;

    //	Serial pc(USBTX, USBRX);
    //	for (int i = 0; i < n; i++)
    //		pc.printf("%.5f ", data[i]);
    //	pc.printf("\n");

#ifdef FAULT_INJECTOR_ENABLE
    for (int i = 0; i < n; i++) {
        int x = FaultInjector::getRandomUInt(1, 4);
        if (x == 4) {
            data[i] = FaultInjector::getRandomFloat(0.0, 1000000.0);
        }
    }
#endif

    qsort(data, n, sizeof (int), compare);

    // Mínimo 50% de chances...
    left = right = n / 2;
    lc = 1;
    for (i = left; i >= 0; i--) {
        for (j = i + 1; j < n && data[j] <= data[i] + (variation * 2); j++)
            ;
        if (j - i > lc) {
            lc = j - i;
            left = i;
            right = (j - 1); // A última comparação é falsa.
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

void fatalError(int error) {

    if (error < 0)
        error *= -1;

    printf("error: %hu\n", (uint16_t) (error * 3));
}

unsigned int getRandomUInt(unsigned int min, unsigned int max) {
    int k;
    double d;
    d = (double) rand() / ((double) RAND_MAX + 1);
    k = d * (max - min + 1);
    return min + k;
}

/*
 * 
 */
int main(int argc, char** argv) {

    unsigned short testNumber = 30;
    
    TestMonitor test;    

    for (int i = 1; i <= testNumber; i++) {

        if (!test.start())
            return EXIT_FAILURE;

        if (i < testNumber)
            test.resetMbed();
    }

    //    srand(time(NULL));   
    //    for (int i = 0; i < 30; i++)
    //        cout << getRandomUInt(05, 21) << " ";

    //    fatalError(2);

    //    const char *addr = "10000";
    //
    //    IOPIN0 = 67;
    //
    //    printf("%d (%p)\n", var, &var);
    //    printf("%d (%p)\n", IOPIN0, &IOPIN0);
    //    
    //    int i, att, rdIntv;
    //    float samples[8], result;
    //
    //    ReadingData data;
    //
    //    att = 0;
    //    do {
    //        for (i = 0; i < 8; i++) {
    //            samples[i] = 100;
    //            printf("amostra[%d]: %.6f\n", i, samples[i]);
    //            if (rdIntv > 0)
    //                Sleep(rdIntv);
    //        }
    //        result = avg(samples, 8, 6, 10);
    //    } while (isnan(result) && att++ < 3);
    //    data.setAnemometer(result);
    //    printf("anemometro: (%.5f)\n", data.getAnemometer());

    //    printf("Fault: %lu\n\tAddress: %p\n\t%s: %s\n\t%s: %s", 1, &addr, "Correct value", addr, "ChangedValue", addr);

    //    test();

    //    testTime();

    //    createReadingData();

    //    uint32_t t;
    //    t = -751244552;
    //    t = 751244551;
    //    printf("t: %u\n", t);

    return EXIT_SUCCESS;
}

//#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>
//
//typedef union {
//    float f;
//    long l;
//} float_long;
//
//struct srt_registro {
//    time_t data_hora;
//    float_long param[9];
//    float_long crc;
//};
//
//char nom_param[9][30] = {
//    {"Temperatura do ar"},
//    {"Umidade do ar"},
//    {"Pluviometria"},
//    {"Velocidade do vento"},
//    {"Umidade do solo"},
//    {"Temperatura do solo"},
//    {"Irradiacao solar"},
//    {"Molhamento foliar"},
//    {"Tensao da bateria"}
//};
//
//int main(void) {
//    
//    struct srt_registro reg;
//    char cont;
//    long crcp;
//
//    printf("Inicio\n");
//    FILE *fp = fopen("DADOS.DAT", "rb");
//    if (!fp) {
//        printf("Arquivo nao pode ser aberto!\n");
//        return (0);
//    }
//    printf("Lendo dados no arquivo...\n");
//
//    while (true) {
//        
//        if (fread(&reg, sizeof (reg), 1, fp) != 1) {
//            printf("Final do arquivo\n");
//            break;
//        }
//        
//        printf("\nData_hora: %s\n", ctime(&reg.data_hora));
//        crcp = reg.data_hora;
//        
//        for (cont = 0; cont < 9; cont++) {
//            printf("\t%s: %f\n", &nom_param[cont][0], reg.param[cont].f);
//            crcp = crcp ^ reg.param[cont].l;
//        }
//        
//        printf("\tcrc: %ld\n", reg.crc.l);
//        crcp = crcp ^ reg.crc.l;
//        printf("\tVerifica CRC=%ld\n", crcp);
//        getchar();
//    }
//    printf("Fechando arquivo...\n");
//    fclose(fp);
//    return (0);
//}
