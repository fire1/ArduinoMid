//
// Created by Angel Zaprianov on 25.7.2017 г..
//

#ifndef ARDUINO_MID_LPG_SERIAL_H
#define ARDUINO_MID_LPG_SERIAL_H

#include <Arduino.h>
#include "../MID.h"
//
// Marker for start transmitting
#ifndef LPG_SERIAL_T_ST
#define LPG_SERIAL_T_ST 140
#endif
//
// Switching to default fuel
#ifndef LPG_SERIAL_T_FA
#define LPG_SERIAL_T_FA 218
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


private:
    boolean transStart = false;
    boolean stateStart = false;
    uint8_t fuelTankAverage = 0;
    uint8_t trans;
    uint8_t history;


    uint16_t fuelTankIndex = 0;
    uint32_t fuelTankCollector = 0;


public:
    LpgSerial(void) {

    }

    void begin(void) {
        Serial2.begin(246);
    }


    void listener(void) {
        if (Serial2.available() > 0) {
            if (trans != LPG_SERIAL_T_ST) {
                history = trans;
                //
                // Calculate averages
                if (fuelTankIndex > 10) {
                    fuelTankAverage = fuelTankCollector / fuelTankIndex;
                }
                stateStart = true;
            }
            uint8_t  val = uint8_t(Serial2.read());

            if(transStart) {
                trans = val;
                transStart = false;
            }

            if(val == 140){
                transStart = true;
            }


            //
            // Agg to average
            fuelTankCollector = fuelTankCollector + trans;
            fuelTankIndex++;


            if (fuelTankAverage == 0) {
                fuelTankAverage = trans;
            }

            Serial.print("LPG transmition: ");
            Serial.println(trans);
        }


    }

    /**
     *
     */
    uint8_t getCurrentValue() {
        if (trans < 100 && trans > 10) {
            return trans;
        }
    }

    /**
     * Gets fuel tank level
     */
    uint8_t getFuelTankLiters() {

        if(fuelTankAverage > 140){
            return (uint8_t) map(fuelTankAverage, 65, 15, 225, 175);
        }
        return (uint8_t) map(fuelTankAverage, 65, 15, 0, 30);
    }

/**
 *  Is additional fuel active
 */
    boolean isLPG() {
        return (history < 140 && history > 10 || trans < 140 && trans > 10) ? true : false;
    }

/**
 *  Is default fuel active
 */
    inline boolean isBNZ() {
        return (trans > 140 || stateStart == false) ? true : false;
    }

};


#endif //ARDUINOMID_LPGSERIAL_H
