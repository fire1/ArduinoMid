//
// Created by Admin on 9/11/2016.
//

#ifndef ARDUINOMID_READRPM_H
#define ARDUINOMID_READRPM_H


void setupRpm() {
//    TCCR1A = 1 << WGM20 | 1 << WGM21;
    TCCR1A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
//    And to calculate the PWM frequency, use
    Fast_PWM_frequency = (16000000) / (Prescale_factor * 256);

}

int getRpm() {





}

#endif //ARDUINOMID_READRPM_H
