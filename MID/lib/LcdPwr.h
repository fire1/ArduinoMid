//
// Created by Angel Zaprianov on 3.5.2017 г..
//

#ifndef ARDUINO_MID_LCD_PWR_H
#define ARDUINO_MID_LCD_PWR_H

#include <Arduino.h>

/**
 * 16x2 && 240x64 power pins
 */
class LcdPwr {
private:
    uint8_t pwr[5];
    uint8_t adt[2];
    bool additional = false;

    void pwmBackLight() {
        /*
         TIMER 5          (Pin 44, 45, 46)
        Value                             Divisor                      Frequency
        0x01                                  1                         31.374 KHz
        0x02                                  8                          3.921 Khz
        0x03                                 64                          490.1 Hz            // default
        0x04                                 256                         122.5 Hz
        0x05                                 1024                        30.63 Hz
        Code:                 TCCR5B = (TCCR5B & 0xF8) | value ;
         */

        //
        //  OC5B for pin 45
        TCCR5B = (TCCR5B & 0xF8) | 0x01;
    }


public:
/**
 *
 * @param pinGnd
 * @param pinVcc
 * @param pinBlA
 * @param pinBlB
 */
    LcdPwr(uint8_t pinGnd, uint8_t pinVcc, uint8_t pinBlA, uint8_t pinBlB, uint8_t pinRD) {
        pwr[0] = pinGnd;
        pwr[1] = pinVcc;
        pwr[2] = pinBlA;
        pwr[3] = pinBlB;
        pwr[4] = pinRD;
    }

/**
 *
 * @param pinGnd    Ground
 * @param pVcc    5V+
 * @param pBLn    BackLight Ground
 * @param pBLp    BackLight 5v+
 * @param pRD     Display's chip read
 * @param pFS     Display's Font Select
 * @param pGnd2   Second ground
 */
    LcdPwr(uint8_t pinGnd, uint8_t pVcc, uint8_t pBLn, uint8_t pBLp, uint8_t pRD, uint8_t pFS, uint8_t pGnd2 = 0) {
        pwr[0] = pinGnd;
        pwr[1] = pVcc;
        pwr[2] = pBLn;
        pwr[3] = pBLp;
        pwr[4] = pRD;
        adt[0] = pFS;
        adt[1] = pGnd2;
        additional = true;
    }


    void begin() {

        pwmBackLight();

        analogWrite(pwr[0], 0); // GND
        analogWrite(pwr[1], 255); // 5V
        analogWrite(pwr[3], 0); // back light
        analogWrite(pwr[2], 255); // back light
        analogWrite(pwr[4], 255); // RD

        if (additional) {
            analogWrite(adt[0], 0); // fs
            if (adt[1] > 0) analogWrite(adt[2], 0); // GND
        }

//        delete this;
    }

    ~LcdPwr() {};
};


#endif //ARDUINOMID_LCDPWR_H
