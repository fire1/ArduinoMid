//
// Created by Angel Zaprianov on 29.3.2018 г..
//

#ifndef ARDUINOMID_SCREEN_H_H
#define ARDUINOMID_SCREEN_H_H

#include <Arduino.h>
#include <U8g2lib.h>

#ifndef _U8G2LIB_HH

#include "../../libraries/U8g2/src/U8g2lib.h"

#endif


const uint8_t DSP_PIN_GD1 = 24; // Power GND
const uint8_t DSP_PIN_GD2 = 25; // Power GND
const uint8_t DSP_PIN_VCC = 27; // Power positive               // 3
const uint8_t DSP_PIN_WR = 29;  // Write Signal
const uint8_t DSP_PIN_RD = 28;  // Read Signal
const uint8_t DSP_PIN_CE = 31;  // Chip Enable Signal
const uint8_t DSP_PIN_CD = 30;  // Instruction Code
const uint8_t DSP_PIN_FS = 43;  // Font selection
const uint8_t DSP_PIN_RST = 32; // Reset signal
const uint8_t DSP_PIN_LDA = 45; // Black-light Anode (+5V)      // 21
const uint8_t DSP_PIN_LDK = 44; // Black-light cathode (0v)     // 22
//
// Data pins
const uint8_t DSP_PIN_DT1 = 35;
const uint8_t DSP_PIN_DT2 = 34;
const uint8_t DSP_PIN_DT3 = 37;
const uint8_t DSP_PIN_DT4 = 36;
const uint8_t DSP_PIN_DT5 = 39;
const uint8_t DSP_PIN_DT6 = 38;
const uint8_t DSP_PIN_DT7 = 41;
const uint8_t DSP_PIN_DT8 = 40;


class LcdPwr {
private:
    uint8_t pwr[5];
    uint8_t adt[2];
    bool additional = false;

/**
 * Quicker Frequency for back light
 */
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
        cli();
        TCCR5B = (TCCR5B & 0xF8) | 0x01;
        sei();
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
        for (uint8_t i = 0; i <= 4; ++i) {
            pinMode(pwr[i], OUTPUT);
        }

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

    ~LcdPwr() { };
};


LcdPwr lcdPwr = LcdPwr(DSP_PIN_GD1, DSP_PIN_VCC, DSP_PIN_LDK, DSP_PIN_LDA, DSP_PIN_RD, DSP_PIN_FS, DSP_PIN_GD2);

U8G2_T6963_240X64_F_8080
        u8g2(U8G2_R0,
             DSP_PIN_DT1,
             DSP_PIN_DT2,
             DSP_PIN_DT3,
             DSP_PIN_DT4,
             DSP_PIN_DT5,
             DSP_PIN_DT6,
             DSP_PIN_DT7,
             DSP_PIN_DT8,
        /*WR*/ DSP_PIN_WR, /*CE*/ DSP_PIN_CE, /*dc8*/DSP_PIN_CD, /*RST*/ DSP_PIN_RST);


#endif //ARDUINOMID_SCREEN_H_H
