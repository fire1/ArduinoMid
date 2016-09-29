//
// Created by Angel Zaprianov on 27.6.2016 г..
//
#include <Arduino.h>

#ifndef MID_MID_H
#define MID_MID_H

#include <LiquidCrystal.h>

/*
bool isSensorReadMid();

//
// Sensors timing containers
long int SNS_LAST_RUN_TIME = 0; // Global sensor method timing

//
// Sensor timing handler
bool isSensorReadMid() {
    if (millis() >= SNS_LAST_RUN_TIME + SNS_INTERVAL_TIME_MID) {
        SNS_LAST_RUN_TIME = millis();
        return true;
    }
    return false;
}

int long SNS_LAST_SEC_TIME = 0;
//
// Sensor timing handler
bool isSensorReadSec() {
    if (millis() >= SNS_LAST_SEC_TIME + 1000) {
        SNS_LAST_SEC_TIME = millis();
        return true;
    }
    return false;
}

int long SNS_LOW_RUN_TIME = 0;
//
// Sensor timing handler
bool isSensorReadLow() {
    if (millis() >= SNS_LAST_RUN_TIME + SNS_INTERVAL_TIME_LOW) {
        SNS_LOW_RUN_TIME = millis();
        return true;
    }
    return false;
}

int long SNS_MIN_RUN_TIME = 0;
//
// Sensor timing handler
bool isSensorReadMin() {
    if (millis() >= SNS_MIN_RUN_TIME + 50) {
        SNS_MIN_RUN_TIME = millis();
        return true;
    }
    return false;
}
*/
//
// Setup the mid
void setupMain() {
    //
    // Pin button mode
    pinMode(BTN_PIN_UP, INPUT);
    pinMode(BTN_PIN_DW, INPUT);

}

/**
 * Limits display floats
 */
char displayFloat(float value, char *output) {

    int dig1 = int(value) * 10; // 210
    int dig2 = int((value * 10) - dig1);

    dig1 = dig1 / 10;
    if (dig2 < 0) {
        dig2 = dig2 * -1;
    }

    sprintf(output, "%02d.%1d", dig1, dig2);

}

void setupTimer2(){

    /* First disable the timer overflow interrupt while we're configuring */
    TIMSK2 &= ~(1 << TOIE2);

    /* Configure timer2 in normal mode (pure counting, no PWM etc.) */
    TCCR2A &= ~((1 << WGM21) | (1 << WGM20));
    TCCR2B &= ~(1 << WGM22);

    /* Select clock source: internal I/O clock */
    ASSR &= ~(1 << AS2);

    /* Disable Compare Match A interrupt enable (only want overflow) */
    TIMSK2 &= ~(1 << OCIE2A);

    /* Now configure the prescaler to CPU clock divided by 128 */
    TCCR2B |= (1 << CS22) | (1 << CS20); // Set bits
    TCCR2B &= ~(1 << CS21);             // Clear bit

    /* We need to calculate a proper value to load the timer counter.
     * The following loads the value 131 into the Timer 2 counter register
     * The math behind this is:
     * (CPU frequency) / (prescaler value) = 125000 Hz = 8us.
     * (desired period) / 8us = 125.
     * MAX(uint8) + 1 - 125 = 131;
     */
    /* Save value globally for later reload in ISR */
    //tcnt2 = 131;

    /* Finally load end enable the timer */
    //TCNT2 = tcnt2;
    TCNT2 = 0;
    TIMSK2 |= (1 << TOIE2);

    // set timer 2 prescale factor to 64
    sbi(TCCR2B, CS22);
    // configure timer 2 for phase correct pwm (8-bit)
    sbi(TCCR2A, WGM20);
}

#endif //ARDUINOMID_UTILS_H




