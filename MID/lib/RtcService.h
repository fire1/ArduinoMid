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
    RTC_DS3231 rtc;

public:
    RtcService() {
        rtc = RTC_DS3231();
    }

    void begin() {


        if (!rtc.begin()) {
            Serial.println(F("Couldn't find RTC"));
        }

        if (rtc.lostPower()) {
            Serial.println(F("RTC lost power, resetting the  time!"));
            // following line sets the RTC to the date & time this sketch was compiled
            rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        }
        noClock = (rtc.lostPower()) ? true : false;

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
    char *getClock() {
        char *dspTime;
        if (noClock)
            return dspTime;
        DateTime now = rtc.now();

        sprintf_P(dspTime, pgm1 /*"%02d:%02d"*/, now.hour(), now.minute());
        return dspTime;
    }

/**
 *
 * @return float
 */
    float getTemperature() {
//        return rtc.getTemperature();
    }

};

#endif //ARDUINOMID_RTCSERVICE_H