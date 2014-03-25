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

#define PRINT_LOGGER_MSG(type, fmt)															\
	va_list args;																			\
	int status = 0;																			\
	char msg_str[MAX_MSG_SIZE];																\
	char time_str[32];																		\
	const char *type_str = 	(type == EV_LOG) ? "(Log)" : (type == EV_WARN) ? "(Warning)" : 	\
							(type == EV_ERR) ? "(Error)" : ""; 								\
	FILE *fp = fopen(log_path, "a");														\
	if (!fp) return -1;																		\
																							\
	if (fmt) {																				\
		time_t seconds = time(NULL);														\
		strftime(time_str, 32, "%F | %T", localtime(&seconds)); /* 2014-03-25 | 08:35:00 */ \
		va_start(args, fmt);																\
		vsnprintf((char*) &msg_str, MAX_MSG_SIZE - 1, fmt, args);							\
		status = fprintf(fp, "%s | %-9s | %s\n", time_str, type_str, msg_str);				\
		if (serial_debug) pc.printf("%s | %-9s | %s\n", time_str, type_str, msg_str);		\
		va_end(args);																		\
	}																						\
																							\
	fclose(fp);																				\
	return status

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
};

#endif /* LOGGER_H_ */
