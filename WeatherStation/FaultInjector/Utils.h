/*
 =======================================================================================================================
 File       : Utils.h
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

#ifndef UTILS_H_
#define UTILS_H_

#include "mbed.h"

unsigned int getRandomUInt(unsigned int min, unsigned int max);
double getRandomDouble(double min, double max);
float getRandomFloat(float min, float max);

char *itoa(int value, char *result, int base);

#endif  /* UTILS_H_ */
