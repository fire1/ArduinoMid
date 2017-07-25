//
// Created by Angel Zaprianov on 25.7.2017 г..
//

#ifndef ARDUINO_MID_LPG_SERIAL_H
#define ARDUINO_MID_LPG_SERIAL_H

#include <Arduino.h>
//
// Marker for start transmitting
#ifndef LPG_SERIAL_T_ST
#define LPG_SERIAL_T_ST 0x8C
#endif
//
// Switching to default fuel
#ifndef LPG_SERIAL_T_FA
#define LPG_SERIAL_T_FA 0x9A
#endif
//
// Switching to additional fuel (LPG)
#ifndef LPG_SERIAL_T_FB
#define LPG_SERIAL_T_FB 0x9B
#endif
//
// Switching to additional fuel A
#ifndef LPG_SERIAL_T_FBA
#define LPG_SERIAL_T_FBA 0x63
#endif
//
// Switching to additional fuel B
#ifndef LPG_SERIAL_T_FBB
#define LPG_SERIAL_T_FBB 0x62
#endif

class LpgSerial {

    Serial_ *com;

private:
    boolean stateStart = false;
    int trans;
    int history;


protected:


public:
    LpgSerial(Serial_ &_com) : com(&_com) { }

    void begin(unsigned long baud = 245) {
        com->begin(baud);
    }


    void listener(void) {
        if (trans != LPG_SERIAL_T_ST)
            history = trans;

        if (com->available() > 0) {
            trans = com->read();
        }

    }

    /**
     *
     */
    boolean isTransmiting() {
        if (trans == LPG_SERIAL_T_ST)
            stateStart = true;

        return stateStart;

    }

    boolean isSwitchLPG() {
        if (trans == LPG_SERIAL_T_FB || trans == LPG_SERIAL_T_FBB && history == LPG_SERIAL_T_FBA) {
            stateStart = false;
            return true;
        }
        return false;
    }

};


#endif //ARDUINOMID_LPGSERIAL_H
