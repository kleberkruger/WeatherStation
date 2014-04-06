/*
 =======================================================================================================================
 File       : Serializable.h
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

#ifndef SERIALIZABLE_H
#define	SERIALIZABLE_H

template<class T>
class Serializable {
public:

    /**
     * Load object from file.
     * 
     * @param file  - the file path
     * @return      - the object
     */
    static T* load(const char *file);

    /**
     * Save object in the file.
     * 
     * @param file  - the file path
     * @return      - true if it succeed
     */
    virtual bool save(const char *file) = 0;    
};

#endif	/* SERIALIZABLE_H */
