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

    logger = new Logger(filePathLog, true);

    //    mbed.connect(devicePort);

    cout << "Resultado: " << readData(filePathDataMbed) << endl;
}

TestMonitor::~TestMonitor() {

    /* Nothing to do */
}

void TestMonitor::start() {
    start(testNumber);
}

void TestMonitor::start(unsigned short n) {
    int attempt, count, faults;
    bool found;
    int result;

    faults = count = 0;

    logger->log("------------------------------ %-6s ------------------------------", "START");
    while (count++ < n) {
        attempt = 0;
        found = false;
        do {
            logger->log("Trying to start test %u...", count);
            if (readReady(filePathReady))
                found = true;
            Sleep(1000);
        } while (!found && attempt++ < testTime);

        if (found) {
            logger->log("Test %u started.");
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

    logger->log("------------------------------ %-6s ------------------------------", "RESULT");
    logger->log("%-18s: %u", "Number of tests", testNumber);
    logger->log("%-18s: %u", "Number of hits", (testNumber - faults));
    logger->log("%-18s: %u", "Number of failures", faults);
    logger->log("------------------------------ %-6s ------------------------------", "END");
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
    ifstream readyFile(path);
    if (readyFile.is_open()) {
        readyFile.close();
        remove(path);
        return true;
    }
    return false;
}

int TestMonitor::readData(const char *path) {

    ReadingData data;
    char buffer[1048];
    int buflen = 0;

    fstream file(path, ios::binary | ios::in);
    if (!file.is_open())
        return ERROR_DATA_NOT_FOUND;

    memset(&data, 0, sizeof (ReadingData));
    memset(buffer, 0, sizeof (char) * 256);

    file.read(reinterpret_cast<char *> (&data), sizeof (ReadingData));

    /*
     * Print reading data
     */
    buflen += sprintf(buffer, "Reading the readings file:\n%-36sTime: %s (%lu)", "", data.getFormatedTime(), data.getTime());
    
    for (int i = 0; i < ReadingData::NUMBER_OF_PARAMETERS; i++)
        buflen += sprintf(buffer + buflen, "\n%-36s%s: %f", "", data.getParameterName(i), data.getParameterValue(i));
    
    buflen += sprintf(buffer + buflen, "\n%-36sCRC: %lu", "", data.getCRC());

    file.close();
    //    remove(path);
    
    logger->log(buffer);

    return checkData(&data);
}

int TestMonitor::checkData(ReadingData *data) {

    char buffer[2048];
    int buflen = 0;
    long crc = 0;
    ReadingData orig;
    
    crc = data->calculateCRC();
    
    buflen += sprintf(buffer, "Checking CRC:");
    buflen += sprintf(buffer + buflen, "\n%-36s%lu (current)", "", data->getCRC());
    buflen += sprintf(buffer + buflen, "\n%-39s%lu (correct)", "", crc);
    
    logger->log(buffer);  

    if (data->checkCRC())
        return ERROR_INCORRECT_CRC;

    fstream file(filePathDataOrig, ios::binary | ios::in);
    if (!file.is_open())
        return TEST_UNPERFORMED;

    file.read(reinterpret_cast<char *> (&orig), sizeof (ReadingData));

    for (int i = 0; i < 9; i++) {
        cout << orig.getParameterName(i) << " - .dat original: \t" << orig.getParameterValue(i) << endl;
        cout << data->getParameterName(i) << " - .dat estacao: \t" << data->getParameterValue(i) << "\n" << endl;
        if (!checkValue(orig.getParameterValue(i), data->getParameterValue(i), deviation)) // vou adicionar uma porcentagem de erro ainda...
            return ERROR_INCORRECT_DATA;
    }
    return TEST_OK;
}

bool TestMonitor::checkValue(float n1, float n2, float perc) {
    float desvio = n1 * perc / 100;
    if (n2 < (n1 - desvio) || n2 > (n1 + desvio))
        return false;
    return true;
}

void TestMonitor::writeResult(int result, int testNumber) {
    char timeStr[32];
    time_t seconds = time(NULL);
    ofstream file(filePathResult, ios::app);
    if (!file.is_open()) {
        cout << "Nao foi possivel escrever o resultado do teste " << testNumber
                << " em " << filePathResult << "." << endl;
        exit(EXIT_FAILURE);
    }

    strftime(timeStr, 32, "%d/%m/%Y %H:%M:%S", localtime(&seconds));
    file << timeStr << "\t\t";

    switch (result) {
        case TEST_OK:
            file << "Teste " << testNumber << " OK.\n";
            cout << "Teste " << testNumber << ": OK." << endl;
            break;
        case TEST_UNPERFORMED:
            file << "Teste %d: FALHOU. Verifique se o arquivo DADOS.DAT original "
                    "existe em " << filePathDataOrig << ".\n";
            cout << "Teste %d: FALHOU. Verifique se o arquivo DADOS.DAT original "
                    "existe em " << filePathDataOrig << "." << endl;
            exit(EXIT_FAILURE);
        case ERROR_READY_NOT_FOUND:
            file << "Teste " << testNumber << ": FALHOU. Arquivo PRONTO.TXT nao encontrado.\n";
            cout << "Teste " << testNumber << ": falhou. Arquivo PRONTO.TXT nao encontrado." << endl;
            resetMbed();
            break;
        case ERROR_DATA_NOT_FOUND:
            file << "Teste " << testNumber << ": FALHOU. Arquivo DADOS.DAT nao encontrado.\n";
            cout << "Teste " << testNumber << ": falhou. Arquivo DADOS.DAT nao encontrado." << endl;
            resetMbed();
            break;
        case ERROR_INCORRECT_CRC:
            file << "Teste " << testNumber << ": FALHOU. CRC incorreto.\n";
            cout << "Teste " << testNumber << ": falhou. CRC incorreto." << endl;
            resetMbed();
            break;
        case ERROR_INCORRECT_DATA:
            file << "Teste " << testNumber << ": FALHOU. Dados incorretos.\n";
            cout << "Teste " << testNumber << ": falhou. Dados incorretos." << endl;
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
