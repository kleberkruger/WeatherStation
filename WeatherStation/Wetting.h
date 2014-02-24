/*
 =======================================================================================================================
 File       : Wetting.h
 -----------------------------------------------------------------------------------------------------------------------
 Author     : Kleber Kruger
 Email      : kleberkruger@gmail.com
 Reference  : Fábio Iaione
 Date       : 2014-02-02
 Version    : 1.0
 Copyright  : Faculty of Computing, FACOM - UFMS
 -----------------------------------------------------------------------------------------------------------------------
 Description: Weather station with implementing fault tolerance
 =======================================================================================================================
 */

#ifndef WETTING_H_
#define WETTING_H_

#include "mbed.h"

class Wetting {
public:

    /**
     * Constructor
     *
     * @param pin_ad
     * @param pin_s
     * @param pin_i
     */
    Wetting(PinName pin_ad, PinName pin_s, PinName pin_i);

    /**
     * Constructor
     *
     * @param pin_ad
     * @param pin_s
     * @param pin_i
     * @param res
     * @param time
     * @param r_min
     * @param r_max
     */
    Wetting(PinName pin_ad, PinName pin_s, PinName pin_i, float res, int time, float r_min, float r_max);

    /**
     * Destructor
     */
    virtual ~Wetting();

    void config(float res, int time, float rmin, float rmax);
    float read();

private:

    AnalogIn pin_ad;
    DigitalOut pin_s, pin_i;

    float r, r_min, r_max;
    int tm;
};

#endif /* WETTING_H_ */
