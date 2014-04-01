/* 
 * File:   Main.cpp
 * Author: Kleber
 *
 * Created on 23 de Março de 2014, 05:18
 */

#include "TestMonitor.h"

using namespace std;

#include <cstdio>
#include <cstring>

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

bool saveData() {
    
    static int i = 0;
    
    i++;
    
    if (i < 3) {
        cout << "retornando false" << endl;
        return false;
    }
    
    cout << "retornando true" << endl;
    return true;
}

void test() {
    
    int att;
    
    for (att = 1; att <= 3 && !saveData(); att++); /* XXX: Test! */

    if (att <= 3) {
        cout << "salvou a info" << endl;
    }
}

/*
 * 
 */
int main(int argc, char** argv) {
    
    const char *addr = "10000";

//    printf("Fault: %lu\n\tAddress: %p\n\t%s: %s\n\t%s: %s", 1, &addr, "Correct value", addr, "ChangedValue", addr);

//    test();

    //    testTime();

    //    createReadingData();

    TestMonitor test;
    test.start();

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
