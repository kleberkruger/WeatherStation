/*
 =======================================================================================================================
 File       : TestMonitor.h
 -----------------------------------------------------------------------------------------------------------------------
 Author     : Kleber Kruger
 Email      : kleberkruger@gmail.com
 Date       : 2014-04-02
 Version    : 1.0
 Copyright  : Faculty of Computing, FACOM - UFMS
 -----------------------------------------------------------------------------------------------------------------------
 Description: Fault-Injection Monitor System
 =======================================================================================================================
 */

#ifndef TESTMONITOR_H
#define	TESTMONITOR_H

#include <iomanip>
#include <sstream>

#include "ConfigFile/ConfigFile.h"
#include "Logger/Logger.h"

#include "MbedConnector.h"
#include "ReadingData.h"

using namespace std;

class TestMonitor {
public:

    typedef enum {
        
        TEST_UNPERFORMED = -1,
        TEST_OK = 0,

        ERROR_READY_NOT_FOUND,
        ERROR_DATA_NOT_FOUND,
        ERROR_INCORRECT_CRC,
        ERROR_INCORRECT_DATA

    } TestResult;

    TestMonitor();
    virtual ~TestMonitor();

    bool start();
    bool start(unsigned short n);

private:

    static const char* const FILEPATH_CONFIG;

    /*
     * Default configuration parameters
     */
    static const char* const DEFAULT_MBEDPORT_STATION;
    static const char* const DEFAULT_MBEDPORT_MONITOR;
    
    static const char* const DEFAULT_FILEPATH_READY;
    static const char* const DEFAULT_FILEPATH_DATA_MBED;
    static const char* const DEFAULT_FILEPATH_DATA_ORIG;
    static const char* const DEFAULT_FILEPATH_RESULT;
    static const char* const DEFAULT_FILEPATH_LOG;

    static const unsigned int DEFAULT_TEST_TIME = 10;
    static const unsigned int DEFAULT_TEST_NUMBER = 1000;
    static const float DEFAULT_PERCENTAGE_DEVIATION = 5;

    /*
     * Configuration parameters
     */
    const char *mbedPortStation;
    const char *mbedPortMonitor;
    
    const char *filePathReady;
    const char *filePathDataMbed;
    const char *filePathDataOrig;
    const char *filePathResult;
    const char *filePathLog;

    unsigned int testTime;
    unsigned int testNumber;
    float deviation;

    MbedConnector mbedStation;
    MbedConnector mbedMonitor;

    ReadingData *orig;

    Logger *logger;

    /**
     * Read configurations.
     * 
     * @return 
     */
    bool loadConfig();

    /**
     * Save configurations.
     * 
     * @return 
     */
    bool saveConfig();

    /**
     * Ready original data file.
     * 
     * @param path
     * @return 
     */
    bool readOriginal(const char *path);

    /**
     * Read ready file.
     */
    bool readReady(const char *path);

    /**
     * 
     */
    int readData(const char *path);

    /**
     * 
     * @param data
     * @param crcp
     * @return 
     */
    int checkData(ReadingData *data);

    /**
     * 
     * @param n1
     * @param n2
     * @param perc
     * @return 
     */
    bool checkValue(const float n1, const float n2, const float perc);

    /**
     * 
     * @param result
     * @param testNumber
     */
    void printResult(const int result, const int testNumber);

    /**
     * 
     */
    void resetMbed();
};


#endif	/* TESTMONITOR_H */

