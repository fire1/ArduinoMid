//
// Created by Angel Zaprianov on 27.6.2016 г..
//


#ifndef MID_FUNCTION_H
#define MID_FUNCTION_H

#include <Arduino.h>
#include <wiring.c>
#include <avr/interrupt.h>


/**
 * Limits playEntry floats
 */
char displayFloat(float value, char *output) {

    if (value < -99) {
        value = -99;
    }

    int dig1 = int(value) * 10; // 210
    int dig2 = int((value * 10) - dig1);

    dig1 = dig1 / 10;
    if (dig2 < 0) {
        dig2 = dig2 * -1;
    }

    sprintf(output, "%02d.%1d", dig1, dig2);
}

/**
 * Limits playEntry floats
 */
void separateFloat(float value, int arrOutput[2]) {

//    out1 = 0;
//    out2 = 0;
    uint8_t dig1 = uint8_t(value) * 100; // 210
    uint8_t dig2 = uint8_t((value * 100) - dig1);

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
float restoreFloat(uint8_t a, uint8_t b) {

    float bf = b * 0.01;

    float c = a + bf;
    return c;
}


#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

/**
 * Setup timer for ECU,RPM,VSS input pins
 * Timer 3
 */

void setupTimer3N() {
    cli(); // stop interrupts
    TCCR3A = 0; // set entire TCCR2A register to 0
    TCCR3B = 0; // same for TCCR2B
    TCNT3 = 0; // initialize counter value to 0
// set compare match register for 100000 Hz increments
    OCR3A = 249; /*249*/ // = 16000000 / (8 * 10000) - 1 (must be <256)
// turn on CTC mode
    TCCR3B |= (1 << WGM31);
// Set CS22, CS21 and CS20 bits for 8 prescaler
    TCCR3B |= (0 << CS32) | (1 << CS31) | (0 << CS30);
// enable timer compare interrupt
//TIMSK3 |= (1 << OCIE3A);
    sbi(TCCR3B, CS32);        // CS31 set prescaler to 256 and start the timer
    sbi(TCCR3A, WGM30);       // put timer 3 in 8-bit phase correct pwm mode
    sei(); // allow interrupts
}


void setupTimer31() {

    // ICNC3 - noise filter

    // set up timer with prescaler = 64 and CTC mode
    TCCR3B |= (1 << WGM32) | (1 << CS31) | (1 << CS30);
    //
    // With filtering on
    // TCCR3B |= (0 << ICNC3) | (1 << WGM32) | (1 << CS31) | (1 << CS30) | (0 << ICES3);



    //Clear ICF1 (this is done by writing a logical 1 to it apparently in the manual).
    // This clears pending interrupts.  Clear OVERFLOW interrupt flag as well.
    //  I found that for some reason Timer 5 initialized with an OVERFLOW interrupt which was resulting in a different value from Timer 1 and 4.
    //  Clearing this interrupt with the timers stopped solved that problem.
    // TIFR5 = (1 << ICF5) | (1 << TOV5);


    // If I change the line in my timer setup "TIMSK3 =" to:
    // TIMSK3 = (0<<ICIE3)|(0<<TCIE3);
    // Everything works fine.
    // For some reason when the TIMER4 Input Capture Interrupt Service Routine
    //



    // initialize counter
    TCNT3 = 0;
    // initialize compare value
    OCR3A = 24999; // OCR3B
    // enable compare interrupt
    TIMSK3 |= (1 << OCIE1A); // TIMSK3 |= (1 << OCIE3A);

}

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

void pciSetup(byte pin) {
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

extern uint8_t _end;
extern uint8_t __stack;

void StackPaint(void) __attribute__ ((naked)) __attribute__ ((section (".init1")));

void StackPaint(void) {
#if 0
    uint8_t *p = &_end;

    while(p <= &__stack)
    {
        *p = 0xc5;
        p++;
    }
#else
    __asm volatile ("    ldi r30,lo8(_end)\n"
            "    ldi r31,hi8(_end)\n"
            "    ldi r24,lo8(0xc5)\n" /* STACK_CANARY = 0xc5 */
            "    ldi r25,hi8(__stack)\n"
            "    rjmp .cmp\n"
            ".loop:\n"
            "    st Z+,r24\n"
            ".cmp:\n"
            "    cpi r30,lo8(__stack)\n"
            "    cpc r31,r25\n"
            "    brlo .loop\n"
            "    breq .loop"::);
#endif
}


uint16_t StackCount(void) {
    const uint8_t *p = &_end;
    uint16_t c = 0;

    while (*p == 0xc5 && p <= &__stack) {
        p++;
        c++;
    }

    return c;
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


float floatPrecision(float val) {
    return ((int) (val * 100.0)) / 100.0F;
}


#endif //ARDUINOMID_UTILS_H




