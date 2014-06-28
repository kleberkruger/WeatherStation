/*
 * TripledData.cpp
 *
 *  Created on: 23/06/2014
 *      Author: Kleber
 */

#include "TripledData.h"

template<class T>
TripledData<T>::TripledData() {
	setData(NULL);
}

template<class T>
TripledData<T>::~TripledData() {
}

template<class T>
T TripledData<T>::getByVotting() {

	if (memcmp(&d1, &d2, sizeof(T)) == 0) {

		if (memcmp(&d1, &d3, sizeof(T)) != 0) {
			memcpy(&d3, &d1, sizeof(T));
		}

	} else if (memcmp(&d1, &d3, sizeof(T)) == 0)
		memcpy(&d2, &d1, sizeof(T));
	else if (memcmp(&d2, &d3, sizeof(T)) == 0)
		memcpy(&d1, &d2, sizeof(T));
	else
		return NULL;

	return d1;
}

template<class T>
T TripledData<T>::getData() {
	return getByVotting();
}

template<class T>
void TripledData<T>::setData(T data) {

	memcpy(&d1, &data, sizeof(T));
	memcpy(&d2, &data, sizeof(T));
	memcpy(&d3, &data, sizeof(T));
}

