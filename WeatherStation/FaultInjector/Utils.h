/* 
 * File:   Utils.h
 * Author: Kleber Kruger
 *
 * Created on 11 de Junho de 2013, 11:39
 */

#ifndef UTILS_H
#define UTILS_H

#include "mbed.h"

unsigned int getRandomIntPositive(unsigned int min, unsigned int max);

double getRandomDouble(double min, double max);

float getRandomFloat(float min, float max);

char *itoa(int value, char *result, int base);

#endif  /* UTILS_H */
