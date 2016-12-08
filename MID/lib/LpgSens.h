//
// Created by Angel Zaprianov on 5.12.2016 г..
//

#ifndef ARDUINO_I2C_FUEL_DETECTION_SWITCH
#define ARDUINO_I2C_FUEL_DETECTION_SWITCH


#include "CarSens.h"

#define LPG_RECEIVE_BUFFER_LENGTH 8

/**
 *
 */
class LpgSens {

//    CarSens *_car;
protected:
    uint8_t pinData, pinClock;

    static boolean transmissionBegin;
    static int receiveBufferIndex;
    static uint8_t receiveBuffer;
    static uint8_t receiveClockState;
    static uint8_t receivedValue;
public:
    LpgSens::LpgSens(void/*CarSens *carSens*/);

    void setup(uint8_t dataPin, uint8_t clockPin);

    static void interruptData(void);

    static void interruptClockFalling(void);

    static void interruptClockRising(void);

    /**
     * Gets data from transmition
     * @return
     */
    static uint8_t getTrnsition(void) {
        return LpgSens::receivedValue;
    }

    uint8_t getValue() {
        return LpgSens::receivedValue;
    }

};

static boolean LpgSens::transmissionBegin = false;
static int LpgSens::receiveBufferIndex = 0;
static int LpgSens::receiveBuffer;
static uint8_t LpgSens::receiveClockState = 1;
static uint8_t LpgSens::receivedValue = 0;

/**
 *
 * @param carSens
 */
LpgSens::LpgSens(void /*CarSens *carSens*/) {
//    _car = carSens;
}

/**
 *
 */
static void LpgSens::interruptData(void) {


    //
    // Just shorting state var.
    boolean trans = LpgSens::transmissionBegin;

    if (trans) {
        // get each successive byte on each call
        if (LpgSens::receiveBufferIndex < LPG_RECEIVE_BUFFER_LENGTH) {

            LpgSens::receiveBuffer |= LpgSens::receiveClockState << LpgSens::receiveBufferIndex;
            ++LpgSens::receiveBufferIndex;
        }
    }

    if (trans && LpgSens::receiveBufferIndex >= LPG_RECEIVE_BUFFER_LENGTH) {
        LpgSens::receivedValue = LpgSens::receiveBuffer;
        LpgSens::receiveBuffer = 0;
    }


    if (!trans && LpgSens::receiveBufferIndex < LPG_RECEIVE_BUFFER_LENGTH && LpgSens::receiveClockState == LOW) {
        LpgSens::transmissionBegin = true;
    }
}

/**
 * Detection HIGH
 */
static void LpgSens::interruptClockFalling(void) {
    LpgSens::receiveClockState = LOW;
}

/**
 * Detection LOW
 */
static void LpgSens::interruptClockRising(void) {
    LpgSens::receiveClockState = HIGH;
}

/**
 * Sets pins into class
 * @param dataPin
 * @param clockPin
 */
void LpgSens::setup(uint8_t dataPin, uint8_t clockPin) {

    pinMode(clockPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(clockPin), LpgSens::interruptClockFalling, FALLING);
    attachInterrupt(digitalPinToInterrupt(clockPin), LpgSens::interruptClockRising, RISING);

    pinMode(dataPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(dataPin), LpgSens::interruptData, CHANGE);

    pinData = dataPin;
    pinClock = clockPin;
}

#endif //ARDUINOMID_LPGSENS_H
