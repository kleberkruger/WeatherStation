/*
 =======================================================================================================================
 File       : ConfigFile.h
 -----------------------------------------------------------------------------------------------------------------------
 Author     : Shinichiro Nakamura
 Modified by: Kleber Kruger
 Date       : 2014-03-01
 Version    : 1.0
 Copyright  : Copyright (C) 2010 Shinichiro Nakamura (CuBeatSystems) <http://shinta.main.jp/>
 -----------------------------------------------------------------------------------------------------------------------
 Description: Configuration file interface class (Original Version 0.0.1)
 =======================================================================================================================
 */

#include "mbed.h"

#ifndef _CONFIG_FILE_H_
#define _CONFIG_FILE_H_

#define NEWLINE_UNIX 		"\n"
#define NEWLINE_DOS 		"\r\n"
#define NEWLINE_MAC 		"\r"

/**
 * Configuration File Class.
 */
class ConfigFile {
public:

	typedef enum {
		UNIX, MAC, DOS
	} FileFormat;

	/**
	 * Create a configuration file object.
	 *
	 * @param file - a pointer to a file name
	 */
	ConfigFile(const char *file);

	/**
	 * Create a configuration file object.
	 *
	 * @param file 		- a pointer to a file name
	 * @param header 	- a pointer to a header
	 */
	ConfigFile(const char *file, const char *header);

	/**
	 * Create a configuration file object.
	 *
	 * @param file 		- a pointer to a file name
	 * @param header 	- a pointer to a header
	 * @param format 	- a file format
	 */
	ConfigFile(const char *file, const char *header, FileFormat format);

	/**
	 * Destroy a configuration file object.
	 */
	~ConfigFile();

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
	bool setValue(const char *key, char *value);

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

	/**
	 * Load configuration. Read from the target file.
	 */
	bool load();

	/**
	 * Save configuration. Write from the target file.
	 */
	bool save();

private:

	typedef struct {
		char *key;
		char *value;
	} config_t;

	config_t **configlist;

	static const char SEPARATOR = '=';

	static const int MAXCONFIG 				= 64;
	static const unsigned int MAXLEN_KEY 	= 64;
	static const unsigned int MAXLEN_VALUE 	= 128;

	const char *filepath;
	const char *header;
	FileFormat format;

	char valuetemp[MAXLEN_VALUE];

	void init();

	config_t *search(const char *key);
	bool add(config_t *cfg);
};

#endif
