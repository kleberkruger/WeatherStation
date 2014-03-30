/* 
 * File:   TestMonitor.cpp
 * Author: Kleber
 * 
 * Created on 23 de Março de 2014, 05:27
 */

#include "TestMonitor.h"

const char* const TestMonitor::FILEPATH_CONFIG = "./config/config.properties";

const char* const TestMonitor::DEFAULT_DEVICE_PORT = "COM3";
const char* const TestMonitor::DEFAULT_FILEPATH_READY = "F:/READY";
const char* const TestMonitor::DEFAULT_FILEPATH_DATA_MBED = "F:/DATA_1.DAT";
const char* const TestMonitor::DEFAULT_FILEPATH_DATA_ORIG = "DATA.DAT";
const char* const TestMonitor::DEFAULT_FILEPATH_RESULT = "TEST.TXT";
const char* const TestMonitor::DEFAULT_FILEPATH_LOG = "LOG.TXT";

TestMonitor::TestMonitor() {

    /* Load configurations from file */
    loadConfig();
    
    cout << "[" << filePathReady << "]" << endl;

    logger = new Logger(filePathLog, true);
    
//    mbed.connect(devicePort);
}

TestMonitor::~TestMonitor() {

    /* Nothing to do */
}

void TestMonitor::start() {
    start(testNumber);
}

void TestMonitor::start(unsigned short n) {
    unsigned short attempt, count, faults;
    bool found;
    int result;
    char time_str[32];
    time_t tm;
    ostringstream buffer;

//    buffer << setiosflags(ios::left);
//    buffer << resetiosflags(ios::left);

    buffer << setiosflags(ios::left);
    faults = count = 0;

    time(&tm);
    strftime(time_str, 32, "(%F %T)", localtime(&tm));

    buffer << "================================================================================" << endl;
    buffer << " START " << time_str << endl;
    buffer << "--------------------------------------------------------------------------------" << endl;

    logger->log(buffer.str().c_str());

    buffer.str().clear();
    buffer.str("");

    while (count++ < n) {

        attempt = 0;
        found = false;

        logger->log("STEP 1 - TRYING TO FOUND READINGS FILE:");

        do {
            logger->log("\tTrying to start test %hu...", count);
            if (readReady(filePathReady))
                found = true;
            Sleep(1000);
        } while (!found && attempt++ < testTime);

        if (found) {
            logger->log("\tTest %hu started.", count);
            result = readData(filePathDataMbed);
            if (result != TEST_OK) {
                faults++;
            }
        } else {
            result = ERROR_READY_NOT_FOUND;
            faults++;
        }
        writeResult(result, count);
    }

    time(&tm);
    strftime(time_str, 32, "(%F %T)", localtime(&tm));

    buffer << setiosflags(ios::left);

    buffer << "================================================================================" << endl;
    buffer << " RESULT " << time_str << endl;
    buffer << "--------------------------------------------------------------------------------" << endl;
    buffer << setw(18) << "Number of tests" << ": " << testNumber << endl;
    buffer << setw(18) << "Number of hits" << ": " << (testNumber - faults) << endl;
    buffer << setw(18) << "Number of failures" << ": " << faults << endl;
    buffer << "================================================================================" << endl;
    buffer << endl;

    logger->log(buffer.str().c_str());
}

bool TestMonitor::loadConfig() {

    char *param;
    bool loaded;

    ConfigFile cfg(FILEPATH_CONFIG, "# MBED Monitor Config File", ConfigFile::DOS);

    loaded = cfg.load();

    param = cfg.getValue("devicePort");
    devicePort = (param) ? param : DEFAULT_DEVICE_PORT;

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

    saveConfig();

    return loaded;
}

bool TestMonitor::saveConfig() {

    char buf[16];

    ConfigFile cfg(FILEPATH_CONFIG, "# MBED Monitor Config File", ConfigFile::DOS);

    cfg.setValue("devicePort", devicePort);
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

bool TestMonitor::readReady(const char *path) {
//    cout << "[" << filePathReady << "]" << endl;
    ifstream readyFile(path);
    if (readyFile.is_open()) {
        readyFile.close();
        //        remove(path);
        return true;
    }
    return false;
}

int TestMonitor::readData(const char *path) {

    ReadingData *data;
    ostringstream buffer;

    fstream file(path, ios::binary | ios::in);
    if (!file.is_open())
        return ERROR_DATA_NOT_FOUND;

//    memset(&data, 0, sizeof (ReadingData));
    
    data = ReadingData::load(path);

//    file.read(reinterpret_cast<char *> (&data), sizeof (ReadingData));

    buffer << setiosflags(ios::left);

    /*
     * Print reading data
     */
    buffer << "STEP 2 - READING THE READINGS FILE:" << endl;
    buffer << "\tTime: " << data->getFormatedTime() << "(" << data->getTime() << ")" << endl;
    for (int i = 0; i < ReadingData::NUMBER_OF_PARAMETERS; i++)
        buffer << "\t" << data->getParameterName(i) << ": " << data->getParameterValue(i) << endl;
    buffer << "\tCRC: " << data->getCRC();

    file.close();
    //    remove(path);

    logger->log(buffer.str().c_str());

    return checkData(data);
}

int TestMonitor::checkData(ReadingData *data) {

    ostringstream buffer;
    long crc = 0;
    
    crc = data->calculateCRC();

    buffer << setiosflags(ios::left);

    buffer << "STEP 3 - CHECKING CRC:" << endl;
    buffer << "\t" << data->getCRC() << "(current)" << endl;
    buffer << "\t" << crc << "(correct)" << endl;

    logger->log(buffer.str().c_str());
    buffer.str().clear();
    buffer.str("");

    if (!data->checkCRC())
        return ERROR_INCORRECT_CRC;       

    logger->log("STEP 4 - CHECKING PARAMETER VALUES:");
    
    ReadingData *orig = ReadingData::load(filePathDataOrig);
    
    if (orig == NULL)
        return TEST_UNPERFORMED;
    
    int status = TEST_OK;

    for (int i = 0; (i < 9 && status == TEST_OK); i++) {
        buffer << "\t" << orig->getParameterName(i) << ": " << orig->getParameterValue(i) << " (original)" << endl;
        buffer << "\t" << data->getParameterName(i) << ": " << data->getParameterValue(i) << " (weather station)" << endl;  
        
        if (!checkValue(orig->getParameterValue(i), data->getParameterValue(i), deviation)) // vou adicionar uma porcentagem de erro ainda...
            status = ERROR_INCORRECT_DATA;
    }
    
    logger->log(buffer.str().c_str());
    
    delete(orig);

    return status;
}

bool TestMonitor::checkValue(float n1, float n2, float perc) {
    float desvio = n1 * perc / 100;
    if (n2 < (n1 - desvio) || n2 > (n1 + desvio))
        return false;
    return true;
}

void TestMonitor::writeResult(int result, int testNumber) {

    ofstream file(filePathResult, ios::app);

    ostringstream buffer;

    buffer << setiosflags(ios::left);

    if (!file.is_open()) {
        buffer << "Nao foi possivel escrever o resultado do teste " << testNumber
                << " em " << filePathResult << "." << endl;
        exit(EXIT_FAILURE);
    }

    switch (result) {
        case TEST_OK:
            buffer << "Teste " << testNumber << ": OK." << endl;
            break;
        case TEST_UNPERFORMED:
            buffer << "Teste %d: FALHOU. Verifique se o arquivo DADOS.DAT original "
                    "existe em " << filePathDataOrig << ".";
            exit(EXIT_FAILURE);
        case ERROR_READY_NOT_FOUND:
            buffer << "Teste " << testNumber << ": FALHOU. Arquivo PRONTO.TXT nao encontrado.";
            resetMbed();
            break;
        case ERROR_DATA_NOT_FOUND:
            buffer << "Teste " << testNumber << ": FALHOU. Arquivo DADOS.DAT nao encontrado.";
            resetMbed();
            break;
        case ERROR_INCORRECT_CRC:
            buffer << "Teste " << testNumber << ": FALHOU. CRC incorreto.";
            resetMbed();
            break;
        case ERROR_INCORRECT_DATA:
            buffer << "Teste " << testNumber << ": FALHOU. Dados incorretos.";
            resetMbed();
            break;
    }

    if (testNumber == testNumber) {
        file << "\n\n";
    }
    file.close();
}

void TestMonitor::resetMbed() {
    mbed.write("R");
}
