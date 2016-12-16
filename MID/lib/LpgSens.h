//
// Created by Angel Zaprianov on 5.12.2016 г..
//

#ifndef ARDUINO_I2C_FUEL_DETECTION_SWITCH
#define ARDUINO_I2C_FUEL_DETECTION_SWITCH


#include "CarSens.h"

#define LPG_RECEIVE_BUFFER_LENGTH 8


volatile uint8_t lpgSens_clockState, lpgSens_dataState;

static void LpgSens_interruptClockChange();

static void LpgSens_interruptDataChange();

/**
 *
 */
class LpgSens {

//    CarSens *_car;
protected:
    static uint8_t _pinClock, _pinData;

    static boolean transmissionBegin;
    static int receiveBufferIndex;
    static uint8_t receiveBuffer;
    static int receiveData;
    static uint8_t receivedValue;
public:
    LpgSens::LpgSens(void/*CarSens *carSens*/);

    void setup(uint8_t dataPin, uint8_t clockPin);

    static void interruptClock(void);

    static void _interruptData(void);

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
static uint8_t LpgSens::receiveData = 1;
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
static void LpgSens::interruptClock(void) {


    //
    // Just shorting state var.
    boolean trans = LpgSens::transmissionBegin;

    int dataState = digitalRead(_pinClock);
    int clockState = LpgSens::receiveData;
    if (trans) {
        // get each successive byte on each call
        if (LpgSens::receiveBufferIndex < LPG_RECEIVE_BUFFER_LENGTH && dataState == HIGH) {

            LpgSens::receiveBuffer |= LpgSens::receiveData << LpgSens::receiveBufferIndex;
            ++LpgSens::receiveBufferIndex;
        }
    }

    if (trans && LpgSens::receiveBufferIndex >= LPG_RECEIVE_BUFFER_LENGTH) {
        LpgSens::transmissionBegin = false;
        LpgSens::receivedValue = LpgSens::receiveBuffer;
        LpgSens::receiveBuffer = 0;
    }


    if (!trans && LpgSens::receiveBufferIndex < LPG_RECEIVE_BUFFER_LENGTH && clockState == LOW && dataState == LOW) {
        LpgSens::transmissionBegin = true;
    }
}

/**
 * Detection HIGH
 */
static void LpgSens::_interruptData(void) {
    LpgSens::receiveData = digitalRead(_pinData);
}


/**
 * Sets pins into class
 * @param dataPin
 * @param clockPin
 */
void LpgSens::setup(uint8_t dataPin, uint8_t clockPin) {

    pinMode(clockPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(clockPin), LpgSens::_interruptData, CHANGE);

    pinMode(dataPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(dataPin), LpgSens::interruptClock, CHANGE);

    _pinClock = dataPin;
    _pinData = clockPin;
}

#endif //ARDUINOMID_LPGSENS_H
