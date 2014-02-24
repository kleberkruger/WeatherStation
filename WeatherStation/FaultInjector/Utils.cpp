/*
 =======================================================================================================================
 File       : Utils.cpp
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

#include "Utils.h"

/**
 * Generates a random number (unsigned int) between the minimum 
 * and maximum range.
 *
 * @param min - minimum
 * @param max - maximum
 */
unsigned int getRandomUInt(unsigned int min, unsigned int max) {
	int k;
	double d;
	d = (double) rand() / ((double) RAND_MAX + 1);
	k = d * (max - min + 1);
	return min + k;
}

/**
 * Generates a random number (double) between the minimum and 
 * maximum range.
 *
 * @param min - minimum
 * @param max - maxinum
 */
double getRandomDouble(double min, double max) {
	double d, k;
	d = (double) rand() / ((double) RAND_MAX + 1);
	k = d * (max - min + 1);
	return min + k;
}

/**
 * Generates a random number (float) between the minimum and 
 * maximum range.
 *
 * @param min - minimum
 * @param max - maxinum
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

	// Check that the base if valid
	if (base < 2 || base > 36) {
		*result = '\0';
		return result;
	}

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35
				+ (tmp_value - value * base)];
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
