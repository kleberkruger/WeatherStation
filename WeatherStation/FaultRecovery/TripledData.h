/*
 * TripledData.h
 *
 *  Created on: 23/06/2014
 *      Author: Kleber
 */

#ifndef TRIPLEDDATA_H_
#define TRIPLEDDATA_H_

#include "mbed.h"

template <class T>
class TripledData {
public:

	TripledData();
	virtual ~TripledData();

	T getData();
	void setData(T data);

private:

	T d1, d2, d3;

	T getByVotting();
};

#endif /* TRIPLEDDATA_H_ */
