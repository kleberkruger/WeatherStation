/*
 =======================================================================================================================
 File       : Serializable.h
 -----------------------------------------------------------------------------------------------------------------------
 Author     : Kleber Kruger
 Email      : kleberkruger@gmail.com
 Date       : 2014-03-29
 Version    : 1.0
 Copyright  : Faculty of Computing, FACOM - UFMS
 -----------------------------------------------------------------------------------------------------------------------
 Description: Weather station with implementing fault tolerance
 =======================================================================================================================
 */

#ifndef SERIALIZABLE_H_
#define SERIALIZABLE_H_

template<class T>
class Serializable {
public:

	/**
	 * Load object from file.
	 *
	 * @param file  - the file path
	 * @return      - the object
	 */
	static T* load(const char *file);

	/**
	 * Save object in the file.
	 *
	 * @param file 	- the file path
	 * @return      - true if it succeed.
	 */
	virtual bool save(const char *file) = 0;
};

#endif /* SERIALIZABLE_H_ */
