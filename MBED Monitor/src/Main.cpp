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

//#define IOPIN0         (*((volatile unsigned long *) 0x10040b010))// assim funciona!

//int var __attribute__((at(0x10040b010)));

/*
 * 
 */
int main(int argc, char** argv) {

    TestMonitor test;
    
    unsigned short amount = 30;

    for (int i = 1; i <= amount; i++) {

        if (!test.start())
            return EXIT_FAILURE;

        if (i < amount)
            test.resetMbed();
    }

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
