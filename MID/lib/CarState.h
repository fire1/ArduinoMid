//
// Created by Angel Zaprianov on 29.11.2016 г..
//

#ifndef ARDUINO_MID_CAR_STATE_H
#define ARDUINO_MID_CAR_STATE_H

#include "IntAmp.h"

struct Diagnostic {
    boolean oil;
    boolean cnt;
    boolean win;
    boolean brk;
    boolean vol;
};

/**
 *
 */
class CarState {

private:
    boolean stateOil = 0;
    boolean stateCnt = 0;
    boolean stateWin = 0;
    boolean stateBrk = 0;
    boolean stateVol = 0;

    Diagnostic result;

    IntAmp *_amp;

    uint8_t pinOil, pinCnt, pinWin, pinBrk, pinVol;
public:
    /**
     *
     * @param amp
     */
    CarState(IntAmp *amp);

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
    if (_amp->is10Seconds()) {
        result.oil = (boolean) digitalRead(pinOil);
        result.brk = (boolean) digitalRead(pinBrk);
        result.cnt = (boolean) digitalRead(pinCnt);
        result.win = (boolean) digitalRead(pinWin);
        result.vol = (analogRead(pinVol) > 1020);

//        result.oil = stateOil;
//        result.brk = stateBrk;
//        result.cnt = stateCnt;
//        result.win = stateWin;
//        result.vol = stateVol;

    }
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
    return boolean(analogRead(pinVol) > 1020);
}

/**
 * Gets diagnostic result
 * @return Diagnostic
 */
Diagnostic CarState::getResult() {
    return result;
}

#endif //ARDUINOMID_CARSTAT_H
