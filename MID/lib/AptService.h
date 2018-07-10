//
// Created by Angel Zaprianov on 31.5.2018 г..
//

#ifndef ARDUINOMID_ADTFUNC_H
#define ARDUINOMID_ADTFUNC_H

#include <Arduino.h>

///////////////////////////////////////////////////////////
//// DRL
///////////////////////////////////////////////////////////
#ifndef DRL_PWM_PIN
#define DRL_PWM_PIN 7
#endif
#ifndef DRL_PWR_PIN
#define DRL_PWR_PIN 10
#endif

///////////////////////////////////////////////////////////
//// RTC
///////////////////////////////////////////////////////////

#ifdef USE_CLOCK_MODULE
#include <Wire.h>
#include <RTClib.h>
#endif

#include "../glob.h"

boolean flashTgl = false;
uint8_t fadeIDrl = 0;


class AptService {

    AmpTime *amp;
    CarSens *car;
//
// Clock
    boolean noClock = true;
#ifdef USE_CLOCK_MODULE
    DS3231 rtc;
#endif

public:
/**
 *
 * @param _amp
 * @param _car
 */
    AptService(AmpTime &_amp, CarSens &_car) : amp(&_amp), car(&_car) {
#ifdef USE_CLOCK_MODULE
        rtc = DS3231();
#endif
    }

    void begin() {
        setupDRL();
        setupRTC();

    }


    void listener() {
        listenDRL();
    }

/*******************************************************************************
 *      RTC
 */

    void setupRTC() {
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
        if (noClock)
            return 0;
#ifdef USE_CLOCK_MODULE
        return rtc.getTemp();
#endif
    }

protected:

/*******************************************************************************
 *      DRL
 */
    void setupDRL() {
        pinMode(DRL_PWM_PIN, OUTPUT);
        pinMode(DRL_PWR_PIN, OUTPUT);
    }

/**
 * Testing method
 * @param amp
 * @param car
 */
    void listenDRL() {


        //
        // Initial demo
        if (millis() < 3000) {
            analogWrite(DRL_PWR_PIN, 255);
            if (amp->isBig() && fadeIDrl < 255) {
                analogWrite(DRL_PWM_PIN, fadeIDrl);
                fadeIDrl++;
            } else if (fadeIDrl > 254) {
                analogWrite(DRL_PWR_PIN, 0);
            }
        }

        //
        // Regular work
        if (amp->isSecond()) {
            if (car->isRunEng()) {
                if (car->isDimOn()) {
                    analogWrite(DRL_PWR_PIN, 0);
                    analogWrite(DRL_PWM_PIN, 0);
                } else if (car->getVss() > 1) {
                    analogWrite(DRL_PWR_PIN, 255);
                    analogWrite(DRL_PWM_PIN, 255);
                } else {
                    analogWrite(DRL_PWR_PIN, 255);
                    analogWrite(DRL_PWM_PIN, 230);
                }
            } else {
                analogWrite(DRL_PWR_PIN, 255);
                if (amp->is5Seconds()) {
                    analogWrite(DRL_PWM_PIN, 255);
                } else if (amp->isBig())
                    analogWrite(DRL_PWM_PIN, 0);
            }
        }

        //
        // Flash DRL attention
        if (amp->isSec() && car->isEmgBreak()) {
            analogWrite(DRL_PWR_PIN, 255);
            if (flashTgl) {
                analogWrite(DRL_PWM_PIN, 0);
                flashTgl = false;
            } else {
                analogWrite(DRL_PWM_PIN, 255);
                flashTgl = true;
            }
        }
    }

};


#endif //ARDUINOMID_ADTFUNC_H
