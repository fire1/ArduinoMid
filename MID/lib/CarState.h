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

};

/**
 *
 */
class CarState {

private:
    Diagnostic result;

    IntAmp *_amp;

    float workDistance;

    boolean alertSatate = false;

    uint8_t pinOil, pinCnt, pinWin, pinBrk, pinVol;

    boolean isBadVoltage();

public:
    /**
     *
     * @param amp
     */
    CarState(IntAmp *amp);

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
 * Construction Car State class
 * @param amp
 */
CarState::CarState(IntAmp *amp) {
    _amp = amp;
}

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
    if (_amp->is2Seconds()) {
        result.oil = (boolean) digitalRead(pinOil);
        result.brk = (boolean) digitalRead(pinBrk);
        result.cnt = (boolean) digitalRead(pinCnt);
        result.win = (boolean) digitalRead(pinWin);
        result.vol = isBadVoltage();

        if (result.oil || result.brk || result.cnt, result.win || result.vol) {
            alertSatate = true;
        }
    }
}

/**
 *
 * @return
 */
boolean CarState::isBadVoltage(void) {
    return !((analogRead(pinVol) < 1020) && boolean(analogRead(pinVol) > 950));
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
    return isBadVoltage();
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
    return alertSatate;
}

#endif //ARDUINOMID_CARSTAT_H
