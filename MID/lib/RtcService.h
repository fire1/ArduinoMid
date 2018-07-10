//
// Created by Angel Zaprianov on 7/9/2018.
//

#ifndef ARDUINOMID_RTCSERVICE_H
#define ARDUINOMID_RTCSERVICE_H
#ifdef USE_CLOCK_MODULE
#include <Wire.h>
#include <RTClib.h>
#endif

#include "../glob.h"


class RtcService {

    boolean noClock = true;
#ifdef USE_CLOCK_MODULE
    DS3231 rtc;
#endif

public:
    RtcService() {
#ifdef USE_CLOCK_MODULE
        rtc = DS3231();
#endif
    }

    void begin() {

#ifdef USE_CLOCK_MODULE
        if (!rtc.begin()) {
            Serial.println(F("Couldn't find RTC"));
        }

        if (!rtc.isrunning()) {
            Serial.println(F("RTC lost power, resetting the  time!"));
            // following line sets the RTC to the date & time this sketch was compiled
            rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        }
        noClock = (!rtc.isrunning()) ? true : false;
#endif
    }

/**
 *
 * @param h
 * @param m
 */
    void setClock(byte h, byte m) {
        if (noClock)
            return;
#ifdef USE_CLOCK_MODULE
        rtc.adjust(DateTime(0, 0, 0, h, m, 0));
#endif
    }

/**
 *
 * @return char
 */
   const char *getClock() {

        if (noClock)
            return getPGM(0);

        char buf[60];
#ifdef USE_CLOCK_MODULE
        //
        // Get clock
        DateTime now = rtc.now();
        strncpy(buf, pgm7, 60);
        Serial.println(now.format(buf));
        return now.format(buf);
#endif
    }

/**
 *
 * @return float
 */
    float getTemperature() {
        if(noClock)
            return 0;
#ifdef USE_CLOCK_MODULE
        return rtc.getTemp();
#endif
    }

};

#endif //ARDUINOMID_RTCSERVICE_H