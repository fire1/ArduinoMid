//
// Created by Angel Zaprianov on 27.6.2016 г..
//


#ifndef MID_FUNCTION_H
#define MID_FUNCTION_H

#include <Arduino.h>
#include <wiring.c>
#include <avr/interrupt.h>
#include "core/MidMath.h"


char pgmBuffer[64];

/**
 * Loads data from flash memory
 */
const char *getPGM(uint8_t i) {
    strcpy_P(pgmBuffer, (char *) pgm_read_word(&(pgmDataTable[i])));
    return pgmBuffer;
}


#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))


/*
 TCCRx - Timer/Counter Control Register. The pre-scaler can be configured here.
TCNTx - Timer/Counter Register. The actual timer value is stored here.
 OCRx - Output Compare Register
 ICRx - Input Capture Register (only for 16bit timer)
TIMSKx - Timer/Counter Interrupt Mask Register. To enable/disable timer interrupts.
TIFRx - Timer/Counter Interrupt Flag Register. Indicates a pending timer interrupt.

CSn2    CSn1    CSn0    Description
0       0       0       No clock source.    (Timer/Counter stopped)
0       0       1       clkI/O/1            (No prescaling
0       1       0       clkI/O/8            (From prescaler)
0       1       1       clkI/O/64           (From prescaler)
1       0       0       clkI/O/256          (From prescaler)
1       0       1       clkI/O/1024         (From prescaler)

 */




// https://gist.github.com/anbara/e7e371d4fbdff896e8703fdb000fdaeb
// https://sites.google.com/site/qeewiki/books/avr-guide/timers-on-the-atmega328
// http://forum.arduino.cc/index.php?topic=19385.msg141920#msg141920
void setupTimer3() {
    cli();//stop interrupts

    // ICNC3 - noise filter
    // OCR3B = 24999;
    // PORTD
    TIMSK3 &= ~(1 << TOIE3);
    /* Configure timer3 in normal mode (pure counting, no PWM etc.) */
    TCCR3A &= ~((1 << WGM31) | (1 << WGM30));
    TCCR3B &= ~(1 << WGM32);

    TCCR3A = TCCR3A | (1 << COM3A1) | (1 << COM3B1) | (1 << COM3C1);

    /* Disable Compare Match A interrupt enable (only want overflow) */
    TIMSK3 &= ~(1 << OCIE3A);

    /* Now configure the prescaler to CPU clock divided by 128 */
    TCCR3B |= (1 << CS32) | (1 << CS30);    // Set bits
    TCCR3B &= ~(1 << CS31);                 // Clear bit

    sbi(TCCR3B, CS32);        // CS31 set prescaler to 256 and start the timer
    sbi(TCCR3A, WGM30);       // put timer 3 in 8-bit phase correct pwm mode

    sei();//allow interrupts
}

/**
 *
 * @param startMicroSeconds
 * @return
 */
unsigned long elapsedMicroseconds(unsigned long startMicroSeconds) {
    return elapsedMicroseconds(startMicroSeconds, micros());
}

/**
 *
 * @param startMicroSeconds
 * @return
 */
unsigned long elapsedMilliseconds(unsigned long startMicroSeconds) {
    return elapsedMilliseconds(startMicroSeconds, millis());
}


/**
 * What getFreeRam() is actually reporting is the space between the heap and the stack.
 * it does not report any de-allocated memory that is buried in the heap.
 * Buried heap space is not usable by the stack,
 * and may be fragmented enough that it is not usable for many heap allocations either.
 * The space between the heap and the stack is what you really need to monitor
 * if you are trying to avoid stack crashes.
 */
int getFreeRam() {
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}


#endif //ARDUINOMID_UTILS_H
