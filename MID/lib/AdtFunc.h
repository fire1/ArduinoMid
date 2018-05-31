//
// Created by Angel Zaprianov on 31.5.2018 г..
//

#ifndef ARDUINOMID_ADTFUNC_H
#define ARDUINOMID_ADTFUNC_H

#include <Arduino.h>

///////////////////////////////////////////////////////////
//// DRL
///////////////////////////////////////////////////////////
#ifndef DRL_PWM_PIN
#define DRL_PWM_PIN 7
#endif
#ifndef DRL_PWR_PIN
#define DRL_PWR_PIN 10
#endif

boolean flashTgl = false;
boolean flashDrl = false;
uint8_t fadeIDrl = 0;
uint8_t drlVss = 0;

/**
 * Testing method
 * @param amp
 * @param car
 */
void listenerDRL(AmpTime *amp, CarSens *car) {


    //
    // Initial demo
    if (millis() < 3000) {
        analogWrite(DRL_PWR_PIN, 255);
        if (amp->isBig() && fadeIDrl < 255) {
            analogWrite(DRL_PWM_PIN, fadeIDrl);
            fadeIDrl++;
        } else if (fadeIDrl > 254) {
            analogWrite(DRL_PWR_PIN, 0);
        }
    }

    //
    // Listen danger braking to alert with flash
    if (amp->isSens()) {
        uint8_t vss = car->getVss();
        if (vss < drlVss - 20) {
            flashDrl = true;
        } else {
            flashDrl = false;
        }
        drlVss = vss;
    }


    //
    // Regular work
    if (amp->isSecond()) {
        if (car->isRunEng()) {
            if (car->isDimOn()) {
                analogWrite(DRL_PWR_PIN, 0);
                analogWrite(DRL_PWM_PIN, 0);
            } else if (car->getVss() > 1) {
                analogWrite(DRL_PWR_PIN, 255);
                analogWrite(DRL_PWM_PIN, 255);
            } else {
                analogWrite(DRL_PWR_PIN, 255);
                analogWrite(DRL_PWM_PIN, 230);
            }
        } else {
            analogWrite(DRL_PWR_PIN, 255);
            if (amp->is5Seconds()) {
                analogWrite(DRL_PWM_PIN, 255);
            } else if (amp->isBig())
                analogWrite(DRL_PWM_PIN, 0);
        }
    }

    //
    // Flash DRL attention
    if (amp->isSec() && flashDrl) {
        analogWrite(DRL_PWR_PIN, 255);
        if (flashTgl) {
            analogWrite(DRL_PWM_PIN, 0);
            flashTgl = false;
        } else {
            analogWrite(DRL_PWM_PIN, 255);
            flashTgl = true;
        }
    }
}




#endif //ARDUINOMID_ADTFUNC_H
