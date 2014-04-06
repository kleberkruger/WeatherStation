/*
 =======================================================================================================================
 File       : ConfigList.h
 -----------------------------------------------------------------------------------------------------------------------
 Author     : Shinichiro Nakamura
 Modified by: Kleber Kruger
 Date       : 2014-03-01
 Version    : 1.0
 Copyright  : Copyright (C) 2010 Shinichiro Nakamura (CuBeatSystems)
 Reference	: <http://shinta.main.jp/>
 -----------------------------------------------------------------------------------------------------------------------
 Description: Configuration file interface class (Original Version 0.0.1)
 =======================================================================================================================
 */

#ifndef CONFIGLIST_H_
#define CONFIGLIST_H_

#include "mbed.h"

/**
 * Configuration List Class.
 */
class ConfigList {
public:

	/**
	 * Create a configuration list.
	 */
	ConfigList();

	/**
	 * Destroy the configuration list.
	 */
	~ConfigList();

	/*
	 * Get a value for a key.
	 *
	 * @param key 	- a target key name
	 *
	 * @return 		- a value or NULL
	 */
	char *getValue(const char *key);

	/**
	 * Get a value for a key.
	 *
	 * @param key 	- a target key name
	 * @param value - a pointer to a value storage
	 * @param size 	- a size of a value storage
	 *
	 * @return 		- a value or NULL
	 */
	bool getValue(const char *key, char *value, size_t size);

	/**
	 * Set a set of a key and value.
	 *
	 * @param key 	- a key
	 * @param value - a value
	 *
	 * @return 		- true if it succeed
	 */
	bool setValue(const char *key, const char *value);

	/**
	 * Remove a configuration.
	 *
	 * @param key 	- a key
	 *
	 * @return 		- true if it succeed
	 */
	bool remove(const char *key);

	/**
	 * Remove all configuration
	 *
	 * @return - true if it succeed
	 */
	bool removeAll(void);

	/**
	 * Get a number of configuration sets.
	 *
	 * @return - number of configuration sets
	 */
	int getCount();

	/**
	 * Get a key and a value.
	 *
	 * @param index 		- index number of this list
	 * @param key 			- a pointer to a buffer for key
	 * @param keybufsiz 	- a size of the key buffer
	 * @param value 		- a pointer to a buffer for value
	 * @param valuebufsiz 	- a size of the value buffer
	 *
	 * @return 				- true if it succeed
	 */
	bool getKeyAndValue(int index, char *key, size_t keybufsiz, char *value, size_t valuebufsiz);

protected:

	typedef struct {
		char *key;
		char *value;
	} config_t;

	config_t **configlist;

	static const char SEPARATOR = '=';

	static const int MAXCONFIG = 64;
	static const unsigned int MAXLEN_KEY = 64;
	static const unsigned int MAXLEN_VALUE = 128;

private:

	config_t *search(const char *key);
	bool add(config_t *cfg);
};

#endif /* CONFIGLIST_H_ */
