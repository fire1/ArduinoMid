//
// Created by Angel Zaprianov on 27.6.2016 г..
//
#include <Arduino.h>

#ifndef MID_MID_H
#define MID_MID_H

#include <wiring.c>

#include <LiquidCrystal.h>

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

/**
 * Limits display floats
 */
void separateFloat(float value,int arrOutput[]) {

//    out1 = 0;
//    out2 = 0;
    int dig1 = int(value) * 100; // 210
    int dig2 = int((value * 100) - dig1);

    dig1 = dig1 / 100;
    if (dig2 < 0) {
        dig2 = dig2 * -1;
    }

    arrOutput[0]= dig1;
    arrOutput[1]= dig2;
}

float restoreFloat(int a, int b) {

    float bf = b * 0.010;

    float c = a+bf;
    return c;
}


void setupUseTimer2() {

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

void setupUseTimer3() {

    TIMSK3 &= ~(1 << TOIE3);

    /* Configure timer3 in normal mode (pure counting, no PWM etc.) */
    TCCR3A &= ~((1 << WGM31) | (1 << WGM30));
    TCCR3B &= ~(1 << WGM32);



    TCCR3A = TCCR3A|(1<<COM3A1)|(1<<COM3B1)|(1<<COM3C1);
    /* Select clock source: internal I/O clock */
    ASSR &= ~(1 << AS3);

    /* Disable Compare Match A interrupt enable (only want overflow) */
    TIMSK3 &= ~(1 << OCIE3A);

    /* Now configure the prescaler to CPU clock divided by 128 */
    TCCR3B |= (1 << CS32) | (1 << CS30); // Set bits
    TCCR3B &= ~(1 << CS31);             // Clear bit


    sbi(TCCR3B, CS31);		// set timer 3 prescale factor to 64
    sbi(TCCR3B, CS30);
    sbi(TCCR3A, WGM30);		// put timer 3 in 8-bit phase correct pwm mode
}

#endif //ARDUINOMID_UTILS_H




