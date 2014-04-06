/*
 =======================================================================================================================
 File       : MbedConnector.cpp
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

#include "MbedConnector.h"

MbedConnector::MbedConnector() {
    connected = false;
}

MbedConnector::~MbedConnector() {
    //    if (port) 
    //        free(port);
}

bool MbedConnector::connect(const char *device) {

    if (!connected) {

        port = NULL; // Handle COM port
        DCB settings; // Armazena configuracoes da porta
        COMMTIMEOUTS timeouts; // Armazena configuracoes de timeouts

        if ((port = CreateFile(device, // Cria a porta COMX:
                GENERIC_READ | GENERIC_WRITE, // for reading and writing
                0, // exclusive access
                NULL, // no security attributes
                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE) {

            cout << "Nao foi possivel estabelecer comunicacao na porta serial "
                    << device << "." << endl;
        }

        // configura os  timeouts em milisegundos
        timeouts.ReadIntervalTimeout = 0;
        timeouts.ReadTotalTimeoutMultiplier = 0;
        timeouts.ReadTotalTimeoutConstant = 100;
        timeouts.WriteTotalTimeoutMultiplier = 0;
        timeouts.WriteTotalTimeoutConstant = 100;

        if (!SetCommTimeouts(port, &timeouts)) {
            // error processing code goes here
            cout << "Falha na funcao SetCommTimeouts" << endl;
            return false;
        }
        // configura os parametros d comunicação
        if (!GetCommState(port, &settings)) { // obtem os parametros atuais
            // error processing code goes here
            cout << "Falha na funcao GetCommState" << endl;
        }

        settings.BaudRate = 9600; // Set Port parameters.
        settings.StopBits = ONESTOPBIT;
        settings.ByteSize = 8;
        settings.Parity = NOPARITY;
        settings.fParity = FALSE;

        if (!SetCommState(port, &settings)) {
            cout << "Falha na funcao SetCommState" << endl;
            return false;
        }
        if (!GetCommState(port, &settings)) {
            // error processing code goes here
            cout << "Falha na funcao GetCommState" << endl;
            return false;
        }

        connected = true;
    }

    return true;
}

bool MbedConnector::write(const char* str) {

    DWORD bytes_written = 0; // numero de bytes escritos

    if (port == NULL)
        return false;

    if (!WriteFile(port, // Handle
            str, // string de escrita
            strlen(str), // Number of bytes to write
            &bytes_written, // Number of bytes written
            NULL)) {

        return false;
    }

    return true;
}
