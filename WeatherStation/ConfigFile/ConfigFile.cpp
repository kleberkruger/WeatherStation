/*
 =======================================================================================================================
 File       : ConfigFile.cpp
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

#include "ConfigFile.h"

const char* const ConfigFile::NEWLINE_UNIX = "\n";
const char* const ConfigFile::NEWLINE_DOS = "\r\n";
const char* const ConfigFile::NEWLINE_MAC = "\r";

/**
 * Create a configuration file object.
 *
 * @param file 		- a pointer to a file name
 * @param header 	- a pointer to a header
 * @param format 	- a file format
 */
ConfigFile::ConfigFile(const char *file, const char *header, FileFormat format) :
		filepath(file), header(header), format(format) {

	/* Load configuration (key, value) from file */
	load();
}

/**
 * Destroy a configuration file object.
 */
ConfigFile::~ConfigFile() {

	/* Nothing to do */
}

/**
 * Load configuration. Read from the target file.
 */
bool ConfigFile::load() {

	/* Open the target file */
	FILE *fp = fopen(filepath, "r");
	if (fp == NULL) {
		return false;
	}

	/* Remove all configuration */
	removeAll();

	/* Read from a file */
	char buf[MAXLEN_KEY + 8 + MAXLEN_VALUE];
	while (fgets(buf, sizeof(buf), fp) != NULL) {

		/* Ignore a comment */
		if (buf[0] == '#') {
			continue;
		}

		/* Trim a return */
		const size_t len = strlen(buf);
		for (size_t i = 0; i < len; i++) {
			if ((buf[i] == '\r') || (buf[i] == '\n')) {
				buf[i] = '\0';
			}
		}

		/* Separate key and value */
		char k[MAXLEN_KEY];
		char v[MAXLEN_VALUE];
		char *sp = strchr(buf, SEPARATOR);
		if (sp != NULL) {
			strcpy(v, sp + 1);
			*sp = '\0';
			strcpy(k, buf);
			setValue(k, v);
		}
	}

	fclose(fp);
	return true;
}

/**
 * Save configuration. Write from the target file.
 */
bool ConfigFile::save() {

	/* Open the target file */
	FILE *fp = fopen(filepath, "w");
	if (fp == NULL) {
		return false;
	}

	/* Set a type of new line */
	const char *newline = NEWLINE_UNIX;
	switch (format) {
		case UNIX:
			newline = NEWLINE_UNIX;
			break;
		case MAC:
			newline = NEWLINE_MAC;
			break;
		case DOS:
			newline = NEWLINE_DOS;
			break;
		default:
			newline = NEWLINE_UNIX;
			break;
	}

	/* Write the header */
	if (header != NULL) {
		fprintf(fp, "%s%s", header, newline);
	}

	/* Write the data */
	for (int i = 0; i < MAXCONFIG; i++) {
		config_t *cfg = configlist[i];
		if (cfg != NULL) {
			fprintf(fp, "%s=%s%s", cfg->key, cfg->value, newline);
		}
	}

	fclose(fp);
	return true;
}

