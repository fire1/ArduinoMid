//
// Created by Angel Zaprianov on 27.6.2016 г..
//

#ifndef ARDUINOMID_VssSens_H
#define ARDUINOMID_VssSens_H

#include <USBAPI.h>

//
// Sensor configs
const bool VssSensDebug = 0;
const bool VssSAlarmSpeed = 0;
const int VssAlarmCitySpeed = 60;
//
// Correction of VSS
const float VssCorrection = 1; // One mile 1.621371192 [changed from int to float]
//const double VssCorrection = 1.621371192; // One mile 1.621371192
const int VssLoopLength = 223; // old 250, best 220
//
// Rpm Container
int CUR_VSS = 0;
//
// Working vars
int vssHitsCount = 0;
int long vssTimerStart = 0, vssTimerEnds = 0;
int vssTimeHits = 0;


/**
 * Callback attachInterrupt
 */
void catchVssHits() {
    vssHitsCount++;
}

/**
  * Setup Vss
 */
void setupVssSens(int pinTarget) {
    pinMode(pinTarget, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt (pinTarget), catchVssHits, FALLING);

//    /* First disable the timer overflow interrupt while we're configuring */
//    TIMSK3 &= ~(1 << TOIE2);
//
//    /* Configure timer2 in normal mode (pure counting, no PWM etc.) */
//    TCCR3A &= ~((1 << WGM21) | (1 << WGM20));
//    TCCR3B &= ~(1 << WGM22);
//
//    /* Select clock source: internal I/O clock */
//    ASSR &= ~(1 << AS2);
//
//    /* Disable Compare Match A interrupt enable (only want overflow) */
//    TIMSK3 &= ~(1 << OCIE2A);
//
//    /* Now configure the prescaler to CPU clock divided by 128 */
//    TCCR3B |= (1 << CS22) | (1 << CS20); // Set bits
//    TCCR3B &= ~(1 << CS21);             // Clear bit
//
//    /* We need to calculate a proper value to load the timer counter.
//     * The following loads the value 131 into the Timer 2 counter register
//     * The math behind this is:
//     * (CPU frequency) / (prescaler value) = 125000 Hz = 8us.
//     * (desired period) / 8us = 125.
//     * MAX(uint8) + 1 - 125 = 131;
//     */
//    /* Save value globally for later reload in ISR */
//    //tcnt2 = 131;
//
//    /* Finally load end enable the timer */
//    //TCNT2 = tcnt2;
//    TCNT3 = 0;
//    TIMSK3 |= (1 << TOIE3);
//
//    // set timer 2 prescale factor to 64
//    sbi(TCCR3B, CS22);
//    // configure timer 2 for phase correct pwm (8-bit)
//    sbi(TCCR3A, WGM20);
}

int long LastVss = 0;

ISR(TIMER3_OVF_vect) {
    int current = digitalRead(SPD_SNS_PIN);
    if (current == HIGH && LastVss == LOW)
        vssHitsCount++;

    LastVss = current;
}

/**
 * Gets current Vss
 */
int getVssSens() {
    return CUR_VSS;
}

/**
 * Detect Vss
 */
void sensVss() {

    vssTimerEnds = millis();
    if (vssTimerEnds >= (vssTimerStart + VssLoopLength)) {
        //
        // Handle cycles
        vssTimerStart = vssTimerEnds;
        //
        // Pass vss to global
        CUR_VSS = int(vssHitsCount * VssCorrection);
        //
        // debug info
        if (VssSensDebug) {
            Serial.print("\n");
            Serial.print(" vss count:  \t");
            Serial.print(vssHitsCount);
            Serial.print(" vss is:  \t");
            Serial.print(vssHitsCount * VssCorrection);
            Serial.print("\n");
        }

        //
        // Alarm speeding at city
        if (isSensorReadMid() && VssSAlarmSpeed && CUR_VSS > VssAlarmCitySpeed) {
            tone(ADT_ALR_PIN, 4000, 1000);
        }
//        if (isSensorReadMid()  && VssSAlarmSpeed && CUR_VSS > 105) {
//            tone(ADT_ALR_PIN, 4000, 1000);
//        }
        //
        // Reset pulse counter
        vssHitsCount = 0;
    }

}

#endif //ARDUINOMID_VssSens_H
