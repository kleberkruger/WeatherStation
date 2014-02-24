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

Logger::Logger() :
		log_path(DEFAULT_FILEPATH), serial_debug(false), pc(USBTX, USBRX) {
}

Logger::Logger(const char *log_path) :
		log_path(log_path), serial_debug(false), pc(USBTX, USBRX) {
}

Logger::Logger(const char *log_path, bool serial_debug) :
		log_path(log_path), serial_debug(serial_debug), pc(USBTX, USBRX) {
}

Logger::~Logger() {
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
