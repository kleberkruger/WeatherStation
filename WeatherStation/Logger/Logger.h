/*
 =======================================================================================================================
 File       : Logger.h
 -----------------------------------------------------------------------------------------------------------------------
 Author     : Kleber Kruger
 Email      : kleberkruger@gmail.com
 Date       : 2014-02-02
 Version    : 1.0
 Copyright  : Faculty of Computing, FACOM - UFMS
 -----------------------------------------------------------------------------------------------------------------------
 Description: Logger Library
 =======================================================================================================================
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "mbed.h"
#include <stdarg.h>

#define DEFAULT_FILEPATH			"/local/log.txt"

/*----------------------------------------------------------------------------------------------------------------------
 Macro that print the messages
 ---------------------------------------------------------------------------------------------------------------------*/

#define BUILD_LOGGER_STRING(buf, bufsz, fmt)            \
	memset(buf, 0, sizeof(char) * bufsz);               \
	if (fmt) {                                          \
		va_list args;                                   \
		va_start(args, fmt);                            \
		vsnprintf((char*) &buf, bufsz - 1, fmt, args);  \
		va_end(args);                                   \
	}

/*--------------------------------------------------------------------------------------------------------------------*/

class Logger {
public:

	typedef enum {
		EV_LOG = 1, EV_WARN = 2, EV_ERR = 3
	} LoggerEventType;

	/**
	 * Creates log file
	 *
	 * @param path 			- log file path
	 * @param serial_debug	- print debug on serial port
	 */
	Logger(const char *path = DEFAULT_FILEPATH, bool serial_debug = false);

	virtual ~Logger();

	int log(const char *fmt, ...);
	int warn(const char *fmt, ...);
	int err(const char *fmt, ...);

private:

	static const unsigned int MAX_MSG_SIZE = 4096;

	const char *log_path;
	bool serial_debug;
	Serial pc;

	int printLoggerMsg(LoggerEventType type, const char *msg);
};

#endif /* LOGGER_H_ */
