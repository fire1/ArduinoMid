//
// Created by Angel Zaprianov on 25.7.2017 г..
//

#ifndef ARDUINO_MID_LPG_SERIAL_H
#define ARDUINO_MID_LPG_SERIAL_H

#include <Arduino.h>
#include "../MID.h"
#include "CmdSerial.h"
#include "../glob.h"
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


#if defined(ADT_FUEL_SYSTEM_SERIAL)

//
// All buttons up - 18
class LpgSerial : public LpgFuel {

    AmpTime *amp;

private:
    uint8_t fuelTankAverage = 0;
    uint8_t capture = 0;
    uint8_t trans;
    uint8_t history;
    uint8_t compare = 0;
    uint8_t index = 0;

    uint16_t fuelTankIndex = 0;
    uint32_t fuelTankCollector = 0;

    uint8_t data[3] = {};

private:
    void setTrans(uint8_t val) {
        /*
         99 = 18 - almost empty BNZ
         20 / 27 / 18 - bnz
         108 - switch
         99 = 100 switched
         148 - bnz
         100 - lpg
         99 = 219 full lpg
         99 = 18 full lpg LPG

         99 == 20 switch to lpg
         */
        if (val >= 34 && val < 255 && val != 99 && val != 98 && val != 100) {
            capture = history;
            trans = val;
        }

        //
        // Fixture 1
//        if(val > 17 && val < 28){
//            trans = 148;
//        }
    }

public:
    LpgSerial(AmpTime &ampTime) : amp(&ampTime) {

    }

    void begin(void) {
        Serial2.begin(245);
//        Serial1.begin(128);
    }

#define DEBUG_SR2

    void listener(void) {




//        if (Serial1.available() > 0 /*&& Serial1.read() > 0*/) {
//        }

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

            }
            uint8_t val = uint8_t(Serial2.read());
#if defined(DEBUG) && defined(DEBUG_SR2)
            Serial.println();
            Serial.print("Current val: ");
            Serial.println(val);
#endif

            data[index] = val;
            index++;
            val = data[0];
            setTrans(val);
            val = data[1];
            setTrans(val);



            //
            // Skip action
            if (data[0] == 255 || data[1] == 255 || data[2] == 255) {
                trans = capture; // Return to 3 steps back
            }

            if (data[0] == 99 && data[1] == 20 ) {
                trans = 146;
            }

            //
            // 100 - 154 almost empty tank (one green dot)

#if defined(DEBUG) && defined(DEBUG_SR2)
            Serial.print("DATA: ");
            Serial.print(data[0]);
            Serial.print(" / ");
            Serial.print(data[1]);
            Serial.print(" / ");
            Serial.print(data[2]);

            Serial.println();
            Serial.print("Recorded trans: ");
            Serial.println(trans);
#endif

            if (index >= 2) {
                index = 0;
                data[0] = 0;
                data[1] = 0;
                data[2] = 0;
            }


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

#elif defined(LPG_SWTC_PIN)


class LpgSwitch : public LpgFuel {

    AmpTime *amp;
    boolean state = false;

public:
    LpgSwitch(AmpTime &ampTime) : amp(&ampTime) {

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
