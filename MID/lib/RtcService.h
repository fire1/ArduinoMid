//
// Created by Angel Zaprianov on 7/9/2018.
//

#ifndef ARDUINOMID_RTCSERVICE_H
#define ARDUINOMID_RTCSERVICE_H

//
// https://github.com/NorthernWidget/DS3231
#include <Wire.h>
#include <RTClib.h>
#include "../glob.h"


class RtcService {

    boolean noClock = true;
    DS3231 rtc;

public:
    RtcService() {
        rtc = DS3231();
    }

    void begin() {


        if (!rtc.begin()) {
            Serial.println(F("Couldn't find RTC"));
        }

        if (!rtc.isrunning()) {
            Serial.println(F("RTC lost power, resetting the  time!"));
            // following line sets the RTC to the date & time this sketch was compiled
            rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        }
        noClock = (!rtc.isrunning()) ? true : false;

    }

/**
 *
 * @param h
 * @param m
 */
    void setClock(byte h, byte m) {
        if (noClock)
            return;
        rtc.adjust(DateTime(0, 0, 0, h, m, 0));
    }

/**
 *
 * @return char
 */
   const char *getClock() {
        char buf[60];
        if (noClock)
            return getPGM(0);

        //
        // Get clock
        DateTime now = rtc.now();
        strncpy(buf, pgm7, 60);
        Serial.println(now.format(buf));
        return now.format(buf);
    }

/**
 *
 * @return float
 */
    float getTemperature() {
        if(noClock)
            return 0;
        return rtc.getTemp();
    }

};

#endif //ARDUINOMID_RTCSERVICE_H