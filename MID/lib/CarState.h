//
// Created by Angel Zaprianov on 29.11.2016 г..
//

#ifndef ARDUINO_MID_CAR_STATE_H
#define ARDUINO_MID_CAR_STATE_H

#include "IntAmp.h"

struct Diagnostic {
    boolean all;    // All combined
    boolean oil;    // oil level
    boolean cnt;    // Coolant level
    boolean win;    // Window washer level
    boolean brk;    // Brake ware
    boolean vol;    // Car Voltage
    boolean blt;    // Belt ware change
    boolean och;    // Oil level change
    boolean air;    // Air filter change
    boolean la1;    // 1 Incandescent lamps
    boolean la2;    // 2 incandescent lamps
};

/**
 *
 */
class CarState {

private:
    Diagnostic result;

    IntAmp &amp;

    float workDistance;

    boolean alertState = false;

    uint8_t pinOil, pinCnt, pinWin, pinBrk, pinVol;

    boolean isBadVoltage();

    int lastVoltageValue = 0;

public:

    /**
 * Construction Car State class
 * @param amp
 */
    CarState(IntAmp &_amp) : amp(_amp) {
        
    }

    void setWorkState(float distance);

    boolean isAlert();

    /**
     *
     * @return int Voltage
     */
    int getVoltage(void);

    boolean getLiveOil();

    boolean getLiveCnt();

    boolean getLiveWin();

    boolean getLiveBrk();

    boolean getLiveVol();

/**
 *
 * @param pinO
 * @param pinC
 * @param pinW
 * @param pinB
 * @param pinV
 */
    void setup(uint8_t pinO, uint8_t pinC, uint8_t pinW, uint8_t pinB, uint8_t pinV);

    void listener();

    Diagnostic getResult();
};


/**
 *
 * @param pinO
 * @param pinC
 * @param pinW
 * @param pinB
 * @param pinV
 */
void CarState::setup(uint8_t pinO, uint8_t pinC, uint8_t pinW, uint8_t pinB, uint8_t pinV) {
    //
    // Sets mode
    pinMode(pinO, INPUT_PULLUP);
    pinMode(pinC, INPUT_PULLUP);
    pinMode(pinW, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);
    pinMode(pinV, INPUT);

    pinOil = pinO;
    pinCnt = pinC;
    pinWin = pinW;
    pinBrk = pinB;
    pinVol = pinV;

}

/**
 * Readings states
 */
void CarState::listener() {
    if (amp.is2Seconds()) {
        result.oil = (boolean) digitalRead(pinOil);
        result.brk = (boolean) digitalRead(pinBrk);
        result.cnt = (boolean) digitalRead(pinCnt);
        result.win = (boolean) digitalRead(pinWin);
        result.vol = isBadVoltage();

        if (result.oil || result.brk || result.cnt, result.win || result.vol) {
            alertState = true;
        }
    }
}

/**
 *
 * @return
 */
boolean CarState::isBadVoltage(void) {

    int readingVoltage = analogRead(pinVol);
    //
    // Voltage too high
    if (lastVoltageValue > 0 && lastVoltageValue == readingVoltage && readingVoltage > 985) {
        return true;
    }
    //
    // Voltage too low
    if (lastVoltageValue > 0 && lastVoltageValue == readingVoltage && readingVoltage < 900) {
        return true;
    }
    //
    // Save last reading
    if (readingVoltage > 0 && lastVoltageValue != readingVoltage) {
        lastVoltageValue = readingVoltage;
    }
    //
    // Voltage is good
    return false;
}

/**
 * Sets work distance from EepRom for calculation
 * @param distance
 */
void CarState::setWorkState(float distance) {
    workDistance = distance;
}

/**
 * Gets live Oil level
 * @return boolean
 */
boolean CarState::getLiveOil() {
    return (boolean) digitalRead(pinOil);
}

/**
 * Gets live Window Wash level
 * @return boolean
 */
boolean CarState::getLiveCnt() {
    return (boolean) digitalRead(pinCnt);
}

/**
 * Gets live Window Wash level
 * @return boolean
 */
boolean CarState::getLiveWin() {
    return (boolean) digitalRead(pinWin);
}

/**
 * Gets live Brake status
 * @return boolean
 */
boolean CarState::getLiveBrk() {
    return (boolean) digitalRead(pinBrk);
}

/**
 * Gets live voltage
 * @return boolean
 */
boolean CarState::getLiveVol() {
    return isBadVoltage();
}

/**
 *  Gets car voltage
 * @return integer
 */
int CarState::getVoltage(void) {
    return analogRead(pinVol) / 67;
}

/**
 * Gets diagnostic result
 * @return Diagnostic
 */
Diagnostic CarState::getResult() {
    return result;
}

/**
 * Is car have some issue
 * @return boolean
 */
boolean CarState::isAlert() {
    return alertState;
}

#endif //ARDUINOMID_CARSTAT_H
