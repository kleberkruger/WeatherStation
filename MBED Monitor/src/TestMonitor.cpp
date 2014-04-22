/*
 =======================================================================================================================
 File       : TestMonitor.cpp
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

#include "TestMonitor.h"

const char* const TestMonitor::FILEPATH_CONFIG = "./config/config.properties";

const char* const TestMonitor::DEFAULT_MBEDPORT_STATION = "COM3";
const char* const TestMonitor::DEFAULT_MBEDPORT_MONITOR = "COM4";

const char* const TestMonitor::DEFAULT_FILEPATH_READY = "F:/READY";
const char* const TestMonitor::DEFAULT_FILEPATH_DATA_MBED = "F:/DATA.DAT";
const char* const TestMonitor::DEFAULT_FILEPATH_DATA_ORIG = "DATA.DAT";
const char* const TestMonitor::DEFAULT_FILEPATH_RESULT = "TEST.TXT";
const char* const TestMonitor::DEFAULT_FILEPATH_LOG = "LOG.TXT";

TestMonitor::TestMonitor() {

    /* Load configurations from file */
    loadConfig();

    /* Save configurations */
    saveConfig();

    orig = NULL;

    /* Initialize logger system */
    logger = new Logger(filePathLog, true);

    /* Connect to mbed */
    //    mbedStation.connect(mbedPortStation);
    mbedMonitor.connect(mbedPortMonitor);
}

TestMonitor::~TestMonitor() {
    if (logger) delete(logger);
    if (orig) delete(orig);
}

bool TestMonitor::connectMbeds() {
    int att = 0;
    bool connected = false;
    do {
        if (mbedStation.connect(mbedPortStation)) {
            mbedMonitor.connect(mbedPortMonitor);
            connected = true;
        }
    } while (!connected && att++ < 100);
}

bool TestMonitor::loadConfig() {

    ConfigFile cfg(FILEPATH_CONFIG, "# MBED Monitor Config File", ConfigFile::DOS);

    bool loaded = cfg.load();

    if (!loaded) {

        /* 
         * Set to default configuration 
         */
        mbedPortStation = DEFAULT_MBEDPORT_STATION;
        mbedPortMonitor = DEFAULT_MBEDPORT_MONITOR;

        filePathReady = DEFAULT_FILEPATH_READY;
        filePathDataMbed = DEFAULT_FILEPATH_DATA_MBED;
        filePathDataOrig = DEFAULT_FILEPATH_DATA_ORIG;
        filePathResult = DEFAULT_FILEPATH_RESULT;
        filePathLog = DEFAULT_FILEPATH_LOG;

        testTime = DEFAULT_TEST_TIME;
        testNumber = DEFAULT_TEST_NUMBER;
        deviation = DEFAULT_PERCENTAGE_DEVIATION;

    } else {

        /* 
         * Read configurations from configuration file 
         */
        char *param;

        param = cfg.getValue("mbedPortStation");
        mbedPortStation = (param) ? param : DEFAULT_MBEDPORT_STATION;

        param = cfg.getValue("mbedPortMonitor");
        mbedPortMonitor = (param) ? param : DEFAULT_MBEDPORT_MONITOR;

        param = cfg.getValue("filePathReady");
        filePathReady = (param) ? param : DEFAULT_FILEPATH_READY;

        param = cfg.getValue("filePathDataMbed");
        filePathDataMbed = (param) ? param : DEFAULT_FILEPATH_DATA_MBED;

        param = cfg.getValue("filePathDataOrig");
        filePathDataOrig = (param) ? param : DEFAULT_FILEPATH_DATA_ORIG;

        param = cfg.getValue("filePathResult");
        filePathResult = (param) ? param : DEFAULT_FILEPATH_RESULT;

        param = cfg.getValue("filePathLog");
        filePathLog = (param) ? param : DEFAULT_FILEPATH_LOG;

        param = cfg.getValue("testTime");
        testTime = (param && (testTime = atoi(param)) > 0) ? testTime : DEFAULT_TEST_TIME;

        param = cfg.getValue("testNumber");
        testNumber = (param && (testNumber = atoi(param)) > 0) ? testNumber : DEFAULT_TEST_NUMBER;

        param = cfg.getValue("percentageDeviation");
        deviation = (param && (deviation = atoff(param)) > 0) ? deviation : DEFAULT_PERCENTAGE_DEVIATION;
    }

    return loaded;
}

bool TestMonitor::saveConfig() {

    char buf[16];

    ConfigFile cfg(FILEPATH_CONFIG, "# MBED Monitor Config File", ConfigFile::DOS);

    cfg.setValue("mbedPortStation", mbedPortStation);
    cfg.setValue("mbedPortMonitor", mbedPortMonitor);

    cfg.setValue("filePathReady", filePathReady);
    cfg.setValue("filePathDataMbed", filePathDataMbed);
    cfg.setValue("filePathDataOrig", filePathDataOrig);
    cfg.setValue("filePathResult", filePathResult);
    cfg.setValue("filePathLog", filePathLog);

    sprintf(buf, "%d", testTime);
    cfg.setValue("testTime", buf);

    sprintf(buf, "%d", testNumber);
    cfg.setValue("testNumber", buf);

    sprintf(buf, "%.1f", deviation);
    cfg.setValue("percentageDeviation", buf);

    return cfg.save();
}

bool TestMonitor::start() {
    return start(testNumber);
}

bool TestMonitor::start(unsigned short n) {

    char time_str[32];
    time_t tm;
    ostringstream buffer;
        
    if (/*!mbedStation.isConnected() ||*/ !mbedMonitor.isConnected())
        return false;

    //    buffer << setiosflags(ios::left);
    //    buffer << resetiosflags(ios::left);

    buffer << setiosflags(ios::left);

    time(&tm);
    strftime(time_str, 32, "(%F %T)", localtime(&tm));

    buffer << "================================================================================" << endl;
    buffer << " START " << time_str << endl;
    buffer << "--------------------------------------------------------------------------------" << endl;

    logger->log(buffer.str().c_str());

    buffer.clear();
    buffer.str("");

    if (!orig && !readOriginal(filePathDataOrig)) {

        time(&tm);
        strftime(time_str, 32, "(%F %T)", localtime(&tm));

        buffer << setiosflags(ios::left);

        buffer << "================================================================================" << endl;
        buffer << " RESULT " << time_str << endl;
        buffer << "--------------------------------------------------------------------------------" << endl;
        buffer << setw(18) << "Test Unperformed. Checks if the file " << filePathDataOrig << " exists." << endl;
        buffer << "================================================================================" << endl;
        buffer << endl;

    } else {

        unsigned short testNum = 0;
        unsigned short result[5];

        for (int i = 0; i < 5; i++)
            result[i] = 0;

        while (testNum++ < n) {

            int res = test(testNum);

            printResult(testNum, res);
            result[res]++;
        }

        unsigned short faults = result[ERROR_READY_NOT_FOUND] + result[ERROR_DATA_NOT_FOUND] +
                result[ERROR_INCORRECT_CRC] + result[ERROR_INCORRECT_DATA];

        time(&tm);
        strftime(time_str, 32, "(%F %T)", localtime(&tm));

        buffer << setiosflags(ios::left);

        buffer << "================================================================================" << endl;
        buffer << " RESULT " << time_str << endl;
        buffer << "--------------------------------------------------------------------------------" << endl;
        buffer << setw(18) << "Number of tests" << ": " << testNumber << endl;
        buffer << setw(18) << "Number of hits" << ": " << (testNumber - faults) << endl;
        buffer << setw(18) << "Number of failures" << ": " << faults << endl;
        buffer << setw(18) << " - Ready Not Found" << ": " << result[ERROR_READY_NOT_FOUND] << endl;
        buffer << setw(18) << " - Data Not Found" << ": " << result[ERROR_DATA_NOT_FOUND] << endl;
        buffer << setw(18) << " - Incorrect CRC" << ": " << result[ERROR_INCORRECT_CRC] << endl;
        buffer << setw(18) << " - Incorrect Data" << ": " << result[ERROR_INCORRECT_DATA] << endl;
        buffer << "================================================================================" << endl;
        buffer << endl;
    }

    logger->log(buffer.str().c_str());

    return true;
}

int TestMonitor::test(const int testNumber) {

    logger->log("STEP 1 - TRYING TO FOUND READINGS FILE (%s):", filePathDataMbed);

    unsigned short att = 0;
    bool found = false;

    do {
        logger->log("\tTrying to start test %hu...", testNumber);

        if (readReady(filePathReady))
            found = true;
        else
            Sleep(1000);

    } while (!found && att++ < testTime);

    if (!found)
        return ERROR_READY_NOT_FOUND;

    logger->log("\tTest %hu started.", testNumber);

    return readData(filePathDataMbed);
}

bool TestMonitor::readOriginal(const char *path) {

    orig = ReadingData::load("DATA.DAT");

    if (orig == NULL)
        return false;

    return true;
}

bool TestMonitor::readReady(const char *path) {
    ifstream readyFile("F:/READY");
    if (readyFile.is_open()) {
        readyFile.close();
        remove("F:/READY");
        return true;
    }
    return false;
}

int TestMonitor::readData(const char *path) {

    ReadingData *data;
    ostringstream buffer;

    data = ReadingData::load("F:/DATA.DAT");

    if (data == NULL)
        return ERROR_DATA_NOT_FOUND;

    //    file.read(reinterpret_cast<char *> (&data), sizeof (ReadingData));

    buffer << setiosflags(ios::left);

    /*
     * Print reading data
     */
    buffer << "STEP 2 - READING THE READINGS FILE (" << filePathDataMbed << "):" << endl;
    buffer << "\tTime: " << data->getFormatedTime() << "(" << data->getTime() << ")" << endl;
    for (int i = 0; i < ReadingData::NUMBER_OF_PARAMETERS; i++)
        buffer << "\t" << data->getParameterName(i) << ": " << data->getParameterValue(i) << endl;
    buffer << "\tCRC: " << data->getCRC();

    logger->log(buffer.str().c_str());

    remove(path);

    int result = checkData(data);

    delete(data);

    return result;
}

int TestMonitor::checkData(ReadingData *data) {

    ostringstream buffer;

    long crc = data->calculateCRC();

    buffer << setiosflags(ios::left);

    buffer << "STEP 3 - CHECKING CRC:" << endl;
    buffer << "\t" << data->getCRC() << "(current)" << endl;
    buffer << "\t" << crc << "(correct)" << endl;

    logger->log(buffer.str().c_str());

    buffer.clear();
    buffer.str("");

    if (!data->checkCRC())
        return ERROR_INCORRECT_CRC;

    logger->log("STEP 4 - CHECKING PARAMETER VALUES:");

    int status = TEST_OK;

    for (int i = 0; (i < ReadingData::NUMBER_OF_PARAMETERS && status == TEST_OK); i++) {

        //        if (i == ReadingData::INDEX_PLUVIOMETER && (data->getParameterValue(i) > 1.0) && data->getParameterValue(i) < 5.0) {
        //            logger->warn("Ajustando pluviometro.");
        //            data->setPluviometer(6.6);
        //        }

        if (!checkValue(orig->getParameterValue(i), data->getParameterValue(i), deviation)) {

            float value = orig->getParameterValue(i);
            float desvio = value * deviation / 100;

            if (desvio < 1.5)
                desvio = 1.5;

            buffer << "\tIncorrect parameter:" << value << endl;
            buffer << "\t\tMin value:" << (value - desvio) << endl;
            buffer << "\t\tMax value:" << (value + desvio) << endl;

            status = ERROR_INCORRECT_DATA;
        }
        buffer << "\t" << orig->getParameterName(i) << ": " << orig->getParameterValue(i) << " (original)" << endl;
        buffer << "\t" << data->getParameterName(i) << ": " << data->getParameterValue(i) << " (weather station)" << endl;
    }

    logger->log(buffer.str().c_str());

    return status;
}

bool TestMonitor::checkValue(float n1, float n2, float perc) {
    float desvio = n1 * perc / 100;
    if (desvio < 1.5)
        desvio = 1.5;
    if (n2 < (n1 - desvio) || n2 > (n1 + desvio))
        return false;
    return true;
}

void TestMonitor::printResult(int testNumber, int result) {

    ofstream file(filePathResult, ios::app);

    ostringstream buffer;

    buffer << setiosflags(ios::left);

    buffer.clear();
    buffer.str("");

    logger->log("STEP 5 - PRINT RESULT:");

    if (!file.is_open()) {
        buffer << "Nao foi possivel escrever o resultado do teste " << testNumber
                << " em " << filePathResult << "." << endl;

        logger->log("\t%s", buffer.str().c_str());

        exit(EXIT_FAILURE);
    }

    buffer << "Test " << testNumber << ": ";

    switch (result) {
        case TEST_OK:
            buffer << "OK." << endl;
            break;
        case TEST_UNPERFORMED:
            buffer << "FALHOU. Verifique se o arquivo DADOS.DAT original existe em " << filePathDataOrig << "." << endl;
            exit(EXIT_FAILURE);
        case ERROR_READY_NOT_FOUND:
            buffer << "FALHOU. File " << filePathReady << " not found." << endl;
            break;
        case ERROR_DATA_NOT_FOUND:
            buffer << "FALHOU. File " << filePathDataMbed << " not found." << endl;
            break;
        case ERROR_INCORRECT_CRC:
            buffer << "FALHOU. Incorrect CRC." << endl;
            break;
        case ERROR_INCORRECT_DATA:
            buffer << "FALHOU. Incorrect data." << endl;
            break;
    }

    logger->log("\t%s", buffer.str().c_str());

    file << buffer.str().c_str();

    if (testNumber == this->testNumber)
        file << endl << endl;

    file.close();

    if (result != TEST_OK)
        resetMbed();
}

void TestMonitor::resetMbed() {

    logger->log("Reseting mbed...");

    mbedMonitor.write("R");

    logger->log("Waiting %u seconds...", testTime * 2);

    Sleep(testTime * 2000);
}
