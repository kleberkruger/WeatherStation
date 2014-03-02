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
}

Logger::~Logger() {

	/* Nothing to do */
}

int Logger::log(const char *fmt, ...) {
	PRINT_LOGGER_MSG(EV_LOG, fmt);
}

int Logger::warn(const char *fmt, ...) {
	PRINT_LOGGER_MSG(EV_WARN, fmt);
}

int Logger::err(const char *fmt, ...) {
	PRINT_LOGGER_MSG(EV_ERR, fmt);
}
