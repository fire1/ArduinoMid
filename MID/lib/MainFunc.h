//
// Created by Angel Zaprianov on 27.6.2016 г..
//
#include <Arduino.h>

#ifndef MID_MID_H
#define MID_MID_H

#include <wiring.c>
#include <LiquidCrystal.h>


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
void separateFloat(float value, int arrOutput[2]) {

//    out1 = 0;
//    out2 = 0;
    int dig1 = int(value) * 100; // 210
    int dig2 = int((value * 100) - dig1);

    dig1 = dig1 / 100;
    if (dig2 < 0) {
        dig2 = dig2 * -1;
    }

    arrOutput[0] = dig1;
    arrOutput[1] = dig2;
}

/**
 * Create float from two integers
 */
float restoreFloat(int a, int b) {

    float bf = b * 0.01;

    float c = a + bf;
    return c;
}


#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

/**
 * Setup timer for ECU,RPM,VSS input pins
 * Timer 3
 */
void setupTimer3() {
    //
    // https://sites.google.com/site/qeewiki/books/avr-guide/timers-on-the-atmega328
    // http://forum.arduino.cc/index.php?topic=19385.msg141920#msg141920
    TIMSK3 &= ~(1 << TOIE3);
    /* Configure timer3 in normal mode (pure counting, no PWM etc.) */
    //
    //   TCCR3A = 1 << WGM30 | 1 << WGM31;
    //      set timer 3 prescale factor to 64
    //    TCCR3B = 1 << CS32;
    TCCR3A &= ~((1 << WGM31) | (1 << WGM30));
    TCCR3B &= ~(1 << WGM32);

    TCCR3A = TCCR3A | (1 << COM3A1) | (1 << COM3B1) | (1 << COM3C1);
    /* Select clock source: internal I/O clock */
//    ASSR &= ~(1 << AS3);

    /* Disable Compare Match A interrupt enable (only want overflow) */
    TIMSK3 &= ~(1 << OCIE3A);

    /* Now configure the prescaler to CPU clock divided by 128 */
    TCCR3B |= (1 << CS32) | (1 << CS30);    // Set bits
    TCCR3B &= ~(1 << CS31);                 // Clear bit
    //
    // Setting the PWM frequency should be similarly simple..
    // by setting CS32, CS31, CS30, CS42, CS41, CS40, etc.  I'll play with this as well.
    //
    // TEST need to change
    //
    //      set timer 3 prescale factor to 64
    //      TCCR3B = 1 << CS32;
    //
    //      enable timer 3 overflow interrupt
    //      TIMSK3 |= 1 << TOIE3;
    //
    //      disable timer 0 overflow interrupt
    //      TIMSK0 &= !(1 << TOIE0);
    //
//    sbi(TCCR3B, CS31);      // set timer 3 prescale factor to 64
    sbi(TCCR3B, CS32);        // CS31 set prescaler to 256 and start the timer
    sbi(TCCR3A, WGM30);       // put timer 3 in 8-bit phase correct pwm mode
}

#endif //ARDUINOMID_UTILS_H




