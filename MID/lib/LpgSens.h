//
// Created by Angel Zaprianov on 17.11.2016 г..
//

#ifndef ARDUINOMID_LPGSENS_H
#define ARDUINOMID_LPGSENS_H

#include <SoftwareSerial.h>
#include "CarSens.h"

class LpgSens {
    CarSens *_car;
    SoftwareSerial *_lpg;

public:
    LpgSens(CarSens *carSens);

    void setup(int rate, uint8_t pinRx, uint8_t pinTx);

    void listener();


};

LpgSens::LpgSens(CarSens *carSens) {
    _car = carSens;
}

void LpgSens::setup(int rate, uint8_t pinRx, uint8_t pinTx) {
    _lpg = new SoftwareSerial(pinRx, pinTx);
    _lpg->begin(rate);
}

void LpgSens::listener() {


    /*if (_car->isRunEng())*/ if (_lpg->available()) {
        Serial.write(_lpg->read());
    }
}


#endif //ARDUINOMID_LPGSENS_H
