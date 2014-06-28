/*
 =======================================================================================================================
 File       : Logger.cpp
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

#include "Logger.h"

/**
 * Creates log file
 *
 * @param path 			- log file path
 * @param serial_debug	- print debug on serial port
 */
Logger::Logger(const char *log_path, bool serial_debug) :
		log_path(log_path), serial_debug(serial_debug), pc(USBTX, USBRX) {

	pc.baud(115200);
}

Logger::~Logger() {

	/* Nothing to do */
}

int Logger::printLoggerMsg(LoggerEventType type, const char *msg) {

	FILE *fp = fopen(log_path, "a");

	if (!fp)
		return -1;

	int count = 0;

	char *line = strtok((char *) msg, "\n");
	for (int i = 0; line != NULL; i++) {

		if (i == 0) {
			char time_str[32];
			const char *type_str = (type == EV_LOG) ? "(Log)" : (type == EV_WARN) ? "(Warning)" :
									(type == EV_ERR) ? "(Error)" : "";

			time_t tm = time(NULL);
			strftime(time_str, 32, "%F | %T", localtime(&tm)); // 2014-03-25 | 08:35:00

			fprintf(fp, "%s | %-9s | %s\n", time_str, type_str, line);
			if (serial_debug)
				pc.printf("%s | %-9s | %s\n", time_str, type_str, line);

		} else {
			fprintf(fp, "%35s %s\n", "|", line);
			if (serial_debug)
				pc.printf("%35s %s\n", "|", line);
		}

		count += strlen(line);
		line = strtok(NULL, "\n");
	}

	fclose(fp);
	return count;
}

int Logger::log(const char *fmt, ...) {

	char msg[MAX_MSG_SIZE];
	BUILD_LOGGER_STRING(msg, MAX_MSG_SIZE, fmt);

	return printLoggerMsg(EV_LOG, msg);
}

int Logger::warn(const char *fmt, ...) {

	char msg[MAX_MSG_SIZE];
	BUILD_LOGGER_STRING(msg, MAX_MSG_SIZE, fmt);

	return printLoggerMsg(EV_WARN, msg);
}

int Logger::err(const char *fmt, ...) {

	char msg[MAX_MSG_SIZE];
	BUILD_LOGGER_STRING(msg, MAX_MSG_SIZE, fmt);

	return printLoggerMsg(EV_ERR, msg);
}
