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


char pgmBuffer[64];

/**
 * Loads data from flash memory
 */
const char *getPGM(uint8_t i) {
    strcpy_P(pgmBuffer, (char *) pgm_read_word(&(pgmDataTable[i])));
    return pgmBuffer;
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
 * @param currentMicroseconds
 * @return
 */
unsigned long elapsedMicroseconds(unsigned long startMicroSeconds, unsigned long currentMicroseconds) {
    if (currentMicroseconds >= startMicroSeconds)
        return currentMicroseconds - startMicroSeconds;
    return 4294967295 - (startMicroSeconds - currentMicroseconds);
}

/**
 *
 * @param startMicroSeconds
 * @return
 */
unsigned long elapsedMicroseconds(unsigned long startMicroSeconds) {
    return elapsedMicroseconds(startMicroSeconds, micros());
}

unsigned long elapsedMilliseconds(unsigned long startMicroSeconds, unsigned long currentMicroseconds) {
    if (currentMicroseconds >= startMicroSeconds)
        return currentMicroseconds - startMicroSeconds;
    return 4294967295 - (startMicroSeconds - currentMicroseconds);
}

/**
 *
 * @param startMicroSeconds
 * @return
 */
unsigned long elapsedMilliseconds(unsigned long startMicroSeconds) {
    return elapsedMicroseconds(startMicroSeconds, millis());
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

/**
 *
 * @param val
 * @return
 */
float floatPrecision(float val) {
    return ((int) (val * 100.0)) / 100.0F;
}

/**
 * Gets car's Joules power per sec
 * @param carWeight
 * @param travelMeters
 * @param travelTime
 * @return
 */
uint32_t getJoules(uint16_t carWeight, uint16_t travelMeters, uint16_t travelTime) {
/*
 * (1/2)Mv2/time = (1/2) 1500 302/ 5 sec = 135,000 joules per sec = 181 HP
 * Reference https://www.physicsforums.com/threads/calculating-motor-engine-torque-power-using-accelerometer-data.336463/
 */
    return (1 / 2) * carWeight * (travelMeters * travelMeters) / travelTime;
}

/**
 * Converts Joules power to Horsepower
 * @param joules
 * @return
 */
uint16_t getHorsepower(uint32_t joules) {
    return uint16_t(joules * 0.0013404825737265);
}


#endif //ARDUINOMID_UTILS_H




