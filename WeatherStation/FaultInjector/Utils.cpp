/* 
 * File:   Utils.cpp
 * Author: Kleber Kruger
 *
 * Created on 11 de Junho de 2013, 11:39
 */

#include "Utils.h"

/**
 * Gera um número aleatório entre o intervalo mínimo e máximo.
 *
 * @param min - número mínimo
 * @param max - número máximo
 */
unsigned int getRandomIntPositive(unsigned int min, unsigned int max) {
    int k;
    double d;
    d = (double) rand() / ((double) RAND_MAX + 1);
    k = d * (max - min + 1);
    return min + k;
}

/**
 * Gera um número aleatório entre o intervalo mínimo e máximo.
 *
 * @param min - número mínimo
 * @param max - número máximo
 */
double getRandomDouble(double min, double max) {
    double d, k;
    d = (double) rand() / ((double) RAND_MAX + 1);
    k = d * (max - min + 1);
    return min + k;
}

/**
 * Gera um número aleatório entre o intervalo mínimo e máximo.
 *
 * @param min - número mínimo
 * @param max - número máximo
 */
float getRandomFloat(float min, float max) {
    int rd = (int) getRandomDouble((int) (min * 10), (int) (max * 10) + 1);
    return (float) rd / 10;
}

/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 */
char *itoa(int value, char *result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) {
        *result = '\0';
        return result;
    }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while (value);

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}
