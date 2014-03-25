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

#ifndef _CONFIG_FILE_H_
#define _CONFIG_FILE_H_

#include "mbed.h"

#include "ConfigList.h"

/**
 * Configuration File Class.
 */
class ConfigFile: public ConfigList {
public:

	typedef enum {
		UNIX, MAC, DOS
	} FileFormat;

	/**
	 * Create a configuration file object.
	 *
	 * @param file 		- a pointer to a file name
	 * @param header 	- a pointer to a header
	 * @param format 	- a file format
	 */
	ConfigFile(const char *file, const char *header = NULL, FileFormat format = UNIX);

	/**
	 * Destroy a configuration file object.
	 */
	~ConfigFile();

	/**
	 * Load configuration. Read from the target file.
	 */
	bool load();

	/**
	 * Save configuration. Write from the target file.
	 */
	bool save();

private:

	static const char* const NEWLINE_UNIX;
	static const char* const NEWLINE_DOS;
	static const char* const NEWLINE_MAC;

	const char *filepath;
	const char *header;
	FileFormat format;
};

#endif
