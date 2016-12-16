//
// Created by Angel Zaprianov on 5.12.2016 г..
//

#ifndef ARDUINO_I2C_FUEL_DETECTION_SWITCH
#define ARDUINO_I2C_FUEL_DETECTION_SWITCH

#include <Arduino.h>
//#include "CarSens.h"

#define LPG_RECEIVE_BUFFER_LENGTH 8


volatile int LpgSens_receiveData;
volatile boolean LpgSens_transmissionBegin;
volatile unsigned int LpgSens_receiveBufferIndex = 0;
volatile uint8_t LpgSens_receiveBuffer;
volatile uint8_t LpgSens_receivedValue[16], LpgSens_receivedIndex;


struct LpgPin {
    uint8_t clc;
    uint8_t dat;
};

LpgPin lpgSensPin;

/**
 * Reads communication between switch and ECU of LPG installation
 */
static void LpgSens_interruptClock(void) {


    //
    // Just shorting state var.
    boolean trans = LpgSens_transmissionBegin;

    int clkState = digitalRead(lpgSensPin.clc);
    int datState = LpgSens_receiveData;

    // get each successive byte on each call
    if (LpgSens_receiveBufferIndex < LPG_RECEIVE_BUFFER_LENGTH && clkState == HIGH) {

        LpgSens_receiveBuffer |= LpgSens_receiveData << LpgSens_receiveBufferIndex;
        ++LpgSens_receiveBufferIndex;
    }


    if (LpgSens_receiveBufferIndex >= LPG_RECEIVE_BUFFER_LENGTH) {
        LpgSens_transmissionBegin = false;
        LpgSens_receivedValue[LpgSens_receivedIndex++] = LpgSens_receiveBuffer;
        LpgSens_receiveBuffer = 0;
        LpgSens_receiveBufferIndex = 0;
    }


    if (!trans && datState == LOW && clkState == LOW) {
        LpgSens_transmissionBegin = true;
    }
}

/**
 * Detection HIGH
 */
static void LpgSens_interruptData(void) {
    LpgSens_receiveData = digitalRead(lpgSensPin.dat);
}


/**
 * Sets pins into class
 * @param dataPin
 * @param clockPin
 */
void LpgSens_setup(uint8_t dataPin, uint8_t clockPin) {

    pinMode(clockPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(clockPin), LpgSens_interruptData, CHANGE);

    pinMode(dataPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(dataPin), LpgSens_interruptClock, CHANGE);

    lpgSensPin.dat = dataPin;
    lpgSensPin.clc = clockPin;
}

boolean LpgSens_isTransmition() {
    if (LpgSens_receivedIndex > 0);
}

uint8_t LpgSens_read() {
    return LpgSens_receivedValue[LpgSens_receivedIndex--];
}


#endif //ARDUINOMID_LPGSENS_H
