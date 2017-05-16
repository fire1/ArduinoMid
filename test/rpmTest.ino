#include <TimerThree.h>

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
// This example uses the timer interrupt to blink an LED
// and also demonstrates how to share a variable between
// the interrupt and the main program.

unsigned volatile int  rpmHitsCount;
void EngSens_catchRpmHits() {
    rpmHitsCount++;
}
void setup(void)
{
    cli(); // stop interrupts
    TCCR3A = 0; // set entire TCCR2A register to 0
    TCCR3B = 0; // same for TCCR2B
    TCNT3  = 0; // initialize counter value to 0
// set compare match register for 100000 Hz increments
    OCR3A = 249; /*249*/ // = 16000000 / (8 * 10000) - 1 (must be <256)
// turn on CTC mode
    TCCR3B |= (1 << WGM31);
// Set CS22, CS21 and CS20 bits for 8 prescaler
    TCCR3B |= (0 << CS32) | (1 << CS31) | (0 << CS30);
// enable timer compare interrupt
//TIMSK3 |= (1 << OCIE3A);
    sei(); // allow interrupts

    attachInterrupt(digitalPinToInterrupt (2), EngSens_catchRpmHits, FALLING);


    Serial.begin(115200);
}




// The main program will print the blink count
// to the Arduino Serial Monitor
void loop(void)
{
    unsigned long blinkCopy;  // holds a copy of the blinkCount

    // to read a variable which the interrupt code writes, we
    // must temporarily disable interrupts, to be sure it will
    // not change while we are reading.  To minimize the time
    // with interrupts off, just quickly make a copy, and then
    // use the copy while allowing the interrupt to keep working.
    noInterrupts();
    blinkCopy = rpmHitsCount;
    rpmHitsCount = 0;
    interrupts();

    Serial.print("blinkCount =  ");
    Serial.println(blinkCopy);
    delay(200);
}