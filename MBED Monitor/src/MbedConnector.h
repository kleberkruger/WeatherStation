/*
 =======================================================================================================================
 File       : MbedConnector.h
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

#ifndef MBEDCONNECTOR_H
#define	MBEDCONNECTOR_H

#include <iostream>
#include <windows.h>

using namespace std;

class MbedConnector {
public:

    MbedConnector();
    virtual ~MbedConnector();

    /**
     * Connect in mbed.
     * 
     * @param devicePort
     * @return true if it succed
     */
    bool connect(const char *devicePort);
    
    /**
     * Write in serial port.
     * 
     * @param str
     * @return true if succed.
     */
    bool write(const char *str);

    bool isConnected() const { return connected; }

private:

    HANDLE port; // Handle COM port
    
    volatile bool connected;
};

#endif	/* MBEDCONNECTOR_H */
