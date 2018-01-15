//
// Created by Angel Zaprianov on 25.7.2017 г..
//

#ifndef ARDUINO_MID_LPG_SERIAL_H
#define ARDUINO_MID_LPG_SERIAL_H

#include <Arduino.h>
#include "../MID.h"
#include "CmdSerial.h"
//
// Marker for start transmitting
#ifndef LPG_SERIAL_T_ST
#define LPG_SERIAL_T_ST 140
#endif
//
// Switching to default fuel
#ifndef LPG_SERIAL_T_FA
#define LPG_SERIAL_T_FA 218 // 219
#endif
//
// Switching to additional fuel (LPG)
#ifndef LPG_SERIAL_T_FBA
#define LPG_SERIAL_T_FBA 15
#endif
//
// Switching to additional fuel B
#ifndef LPG_SERIAL_T_FBB
#define LPG_SERIAL_T_FBB 100
#endif

//
// All buttons up - 18
class LpgSerial : public LpgFuel {

    AmpTime *amp;

private:
    boolean transStart = false;
    boolean stateStart = false;
    uint8_t fuelTankAverage = 0;
    uint8_t capture = 0;
    uint8_t trans;
    uint8_t history;
    uint8_t lpg = 0;
    uint8_t index = 0;

    uint16_t fuelTankIndex = 0;
    uint32_t fuelTankCollector = 0;

    uint8_t data[4] = {};

public:
    LpgSerial(AmpTime &ampTime) : amp(&ampTime) {

    }

    void begin(void) {
        Serial2.begin(245);
        Serial1.begin(128);
    }

//#define DEBUG_SR2

    void listener(void) {


        if (lpg != 0) {
            lpg = 0;
        }

        if (Serial1.available() > 0 /*&& Serial1.read() > 0*/) {
//            debug_fast(amp, F("lpg1"), Serial1.read());
        }

        if (Serial2.available() > 0) {
            if (trans != LPG_SERIAL_T_ST) {
                history = trans;
                //
                // Calculate averages
                if (fuelTankIndex > 10) {
                    fuelTankAverage = fuelTankCollector / fuelTankIndex;
                    fuelTankCollector = 0;
                    fuelTankIndex = 0;
                }
                stateStart = true;
            }
            uint8_t val = uint8_t(Serial2.read());

            if (history == 0) {
                index = 0;
            }

            data[index] = val;
            index++;


            if (index > 3) {
                index = 0;
            }


            if (val > 40 && val <255) {
                capture = history;
                trans = val;

            }

            //
            // Skip action
            if(data[0] == 255 || data[1] == 255 || data[3] == 255){
                trans = capture;
            }


            //
            // 100 - 154 almost empty tank (one green dot)

#if defined(DEBUG)&& defined(DEBUG_SR2)
            Serial.print("DATA: ");
            Serial.print(data[0]);
            Serial.print(" / ");
            Serial.print(data[1]);
            Serial.print(" / ");
            Serial.print(data[3]);
            Serial.println();
            Serial.print("Recorded trans: ");
            Serial.println(trans);

#endif


            //
            // Older version
//            if (val == 146) {
//                lpg = 1;
//            }
//
//            if (val == 148) {
//                lpg = 2;
//            }

//            if (val == 146) {
//                transStart = true;
//            }
//
//            if (val == 255) {
//                transStart = false;
//            }

            //
            // Agg to average
            fuelTankCollector = fuelTankCollector + trans;
            fuelTankIndex++;


            if (fuelTankAverage == 0) {
                fuelTankAverage = trans;
            }

        }


    }

    /**
     *
     */
    uint8_t getCurrentValue() {
        if (trans < 100 && trans > 10) {
            return trans;
        }
        return 0;
    }

    /**
     * Gets fuel tank level
     */
    uint8_t getFuelTankLiters() {

        if (fuelTankAverage > 140) {
            return (uint8_t) map(fuelTankAverage, 215, 145, 65, 15);
        }
        return (uint8_t) map(fuelTankAverage, 75, 15, 5, 30);
    }

/**
 *  Is additional fuel active
 */
    boolean isLPG() {
//        return (history < 140 && history > 27 || trans < 140 && trans > 27 || lpg == 1) ? true : false;
        history = trans;
        return (trans < 147) ? true : false;
    }

/**
 *  Is default fuel active
 */
    inline boolean isBNZ() {
//        return (trans > 140 && trans == history || trans == 27 || stateStart == false || lpg == 2) ? true : false;
        history = trans;
        return (trans > 147) ? true : false;
    }

};


#endif //ARDUINOMID_LPGSERIAL_H
