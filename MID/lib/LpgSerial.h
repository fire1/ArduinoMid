//
// Created by Angel Zaprianov on 25.7.2017 г..
//

#ifndef ARDUINO_MID_LPG_SERIAL_H
#define ARDUINO_MID_LPG_SERIAL_H


#include "../MID.h"
#include "CmdSerial.h"
#include "../glob.h"
#include "InitObj.h"
#include "AmpTime.h"
#include "CarSens.h"
#include "LpgSens.h"


#if defined(ADT_FUEL_SYSTEM_SERIAL)

/**
 *  ArduinoMid tutorial to providing LPG switch signal.
 * In order to capture signals from LPG ECU to digital"Switch Unit"
 * check 4 wires for voltages with multimeter.
 * Two of them needs to provide power supply for switch,
 * other two needs to  provide data and button press sensing.
 *
 * Data signal wire will have some voltage drop over time cycle (max voltage needs to be 5V),
 * when you find this wire run this script with enabled (uncomment) <LPG_TIME_SENS> to
 * determinate data pulse time size.
 *
 * Pulse sizes need to be two types (one for logic "1" and one for logic "0").
 * Define size only for "logic 1" pulse in maximum tolerance.
 *
 *  Example:
 *
        0
        0
        3982 // logic 1
        1993 // logic 0
        1993
        1993
        1994
        3978 // logic 1
        1993
        1993
        3978
        1994
        1993
        1995
        3979 // logic 1
        3978 // logic 1
        0
        0
        0
        ...
 *
 *     Since logic 0 have maximum pulse time of ~2000
 *     for logic 1 pulse time will be from 3500 to 4500
 *
 *
 *     Test result from europe gas
 *
 *      Fuel level one dot
 *      10011000100001 - LPG is disabled (BNZ only)
 *      10010100100001 - LPG is active and wait to start
 *      10010000100001 - Engine is running on LPG
 *      10001000100001 - LPG consuming
 *
 */

//#define LPG_TIME_SENS           // Uncomment to see pulses width in microseconds

//
// Basic settings
#ifndef LPG_INPUT
#define LPG_INPUT 15            // 15/17 Input pin
#endif
#ifndef LPG_TIMEOUT
#define LPG_TIMEOUT  10000     // Timeout between data signals in micros
#endif
#ifndef LPG_BITS
#define LPG_BITS 14             // Bits to capture
#endif
#ifndef LPG_BYTE_TS
#define LPG_BYTE_TS 100         // Tolerance micro time +/-
#endif
#ifndef LPG_BYTE_B0
#define LPG_BYTE_B0 2000        // HIGH state length for 0 bit
#endif
#ifndef LPG_BYTE_B1
#define LPG_BYTE_B1 4000        //  HIGH state length 1 bit
#endif
#ifndef LPG_PIN_STATE
#define LPG_PIN_STATE HIGH       //  Pin state to capture
#endif

/**
 *
 */
class LpgSerial : public LpgFuel {


private:
    LpgSens lpg;
    AmpTime *amp;
    CarSens *car;
    boolean lpgUse = false;


    void detection() {
        if (lpg.isAvailable()) {
            //
            // bnz
            if (isSwitchBnz(0) || isSwitchBnz(1)) {
                if (lpgUse) { // checks for opposite
                    car->passMelodyClass()->play(6);
                }
                lpgUse = false;
                if (cmdLive(DBG_SR_LPG)) dump_txt("BNZ active")
            }

            //
            // lpg
            if (isSwitchLPG(0) || isSwitchLPG(1)) {
                if (!lpgUse) { // checks for opposite
                    car->passMelodyClass()->play(1);
                }
                lpgUse = true;
                if (cmdLive(DBG_SR_LPG)) dump_txt("LPG active")
            }

            //
            // Debug info
            if (cmdLive(DBG_SR_LPG)) {
                dump_bin("LPG1:", lpg.getData(0))
                dump_bin("LPG2:", lpg.getData(1))
            }

//            Serial.println(lpg.getData(0),BIN);

            lpg.setReceived();
        }
    }

/**
 * Comparator for BNZ
 * @param index
 * @return
 */
    boolean isSwitchBnz(uint8_t index) {
        return bitRead(lpg.getData(index), 9) == 1 && bitRead(lpg.getData(index), 10) == 1;
    }

/**
 * Comparator for LPG
 * @param index
 * @return
 */
    boolean isSwitchLPG(uint8_t index) {
        return bitRead(lpg.getData(index), 9) == 1 && bitRead(lpg.getData(index), 10) == 0;
    }


public:

    LpgSerial(AmpTime &ampTime, CarSens &carSens) : amp(&ampTime), car(&carSens) {
        lpg = LpgSens();
    }

    void begin() {
        lpg.begin();
    }

    void listener() override {
        lpg.listener();
        detection();
    }


    uint8_t getFuelTankLiters() {
        return 0;
    }

    /**
 *
 */
    uint8_t getCurrentValue() override {
        return lpg.getData(1);
    }

    void setDynamic() {

    }

    /**
 *  Is additional fuel active
 */
    boolean isLPG() override {
        return lpgUse;
    }

/**
 *  Is default fuel active
 */
    inline boolean isBNZ() override {
        return (!lpgUse) ? true : false;
    }

};


#elif defined(LPG_SWTC_PIN)


class LpgSwitch : public LpgFuel {

    AmpTime *amp;
    boolean state = false;

public:
    LpgSwitch(AmpTime &ampTime) : amp(&ampTime) {

    }

    void begin(void){
        pinMode(LPG_SWTC_PIN, INPUT);
        digitalWrite(LPG_SWTC_PIN, LOW);
    }

    void listener() {
        if (amp->isSecond()) {
            state = (digitalRead(LPG_SWTC_PIN) == LPG_SWTC_STT) ? true : false;
        }
    }

    uint8_t getCurrentValue() {
        return 0;
    };

    virtual uint8_t getFuelTankLiters() {
        return 0;
    };

    boolean isLPG() {
        return state;
    }

    inline boolean isBNZ() {
        return !state;
    }

};

#endif

#endif //ARDUINOMID_LPGSERIAL_H
