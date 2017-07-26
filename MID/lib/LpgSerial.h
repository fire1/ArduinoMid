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
#ifndef LPG_SERIAL_T_FB
#define LPG_SERIAL_T_FB 0x9B
#endif
//
// Switching to additional fuel B
#ifndef LPG_SERIAL_T_FBB
#define LPG_SERIAL_T_FBB 18
#endif

//
// All buttons up - 18
class LpgSerial : public LpgFuel {


private:
    boolean stateStart = false;
    int trans;
    int history;


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
            }
            trans = Serial2.read();
            Serial.print("LPG trans ");
            Serial.println(trans);
        }


    }

    uint8_t getCurrentValue() {
        if (trans < 100 && trans > 10) {
            return trans;
        }
    }

/**
 *  Is additional fuel active
 */
    boolean isLPG() {

        return (history < 100 && history > 10 || trans < 100 && trans > 10 == LPG_SERIAL_T_FBB) ? true : false;
    }

/**
 *  Is default fuel active
 */
    inline boolean isBNZ() {
        return (history == LPG_SERIAL_T_FA || trans == LPG_SERIAL_T_FA) ? true : false;
    }

};


#endif //ARDUINOMID_LPGSERIAL_H
