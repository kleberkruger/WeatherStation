/* 
 * File:   Main.cpp
 * Author: Kleber
 *
 * Created on 23 de Março de 2014, 05:18
 */

#include "TestMonitor.h"

using namespace std;

/**
 * Creates reading data to test this system.
 * 
 * @return 
 */
bool createReadingData() {

    ReadingData data;

    data.setTime(1395753010);

    for (int i = 0; i < ReadingData::NUMBER_OF_PARAMETERS; i++)
        data.setParameterValue(i, 25);

    data.setCRC(data.calculateCRC());

    return data.save("test.dat");
}

void testTime() {
    
    char time_str[32];
    
    time_t t = time(NULL);
    struct tm *timeinfo = localtime(&t);
    
    sprintf(time_str, "%s", "kkkkkkkkkkkkkkkkkkkkkkkkkkkkkk");

    strftime(time_str, 32, "%F | %T", timeinfo);
    
    printf("current time: %lu\n", t);

    printf("%s | %-9s | %s\n", time_str, "(Log)", "Message 1.");
    printf("%s | %-9s | %s\n", time_str, "(Log)", "Message 2.");
}

#include <stdio.h>

/*
 * 
 */
int main(int argc, char** argv) {
    
    createReadingData();

    TestMonitor test;
    //    test.start();
    
    testTime();

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
