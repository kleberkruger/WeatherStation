/* 
 * File:   MbedConnector.h
 * Author: Kleber
 *
 * Created on 23 de Março de 2014, 05:31
 */

#ifndef MBEDCONNECTOR_H
#define	MBEDCONNECTOR_H

#include <iostream>
#include <windows.h> // biblioteca geral do windows contendo CreateFile, ReadFile, etc

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

private:

    HANDLE port; // Handle COM port
};

#endif	/* MBEDCONNECTOR_H */
