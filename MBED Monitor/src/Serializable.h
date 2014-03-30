/* 
 * File:   Serializable.h
 * Author: Kleber
 *
 * Created on 29 de Março de 2014, 14:16
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
