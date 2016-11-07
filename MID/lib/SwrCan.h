//
// Created by Angel Zaprianov on 11/6/2016.
//

#ifndef ARDUINOM_ID_SWR_CAN_H
#define ARDUINO_MID_SWR_CAN_H

#include <Arduino.h>

String ecuPreview;

void ISR() {


    //
    //
    if (digitalRead() = LOW) {
        ecuPreview += " 0";
    }
    if (digitalRead() = HIGH) {
        ecuPreview += " 1";
    }
}

volatile static pFunc int0Func;
ISR(INT0_vect) { //processInjOpen by default
    int0Func ();
}

volatile static pFunc int1Func;
ISR(INT1_vect) {//processInjClosed
    int1Func ();
}

//
// Single-Wire CAN
class SwrCan {

};


#endif //ARDUINOMID_SAECAN_H
