//
// Created by Angel Zaprianov on 27.6.2016 г..
//
#include <Arduino.h>

#ifndef MID_MID_H
#define MID_MID_H

#include <LiquidCrystal.h>


double getAirFlowRation(int engTemp) {
    // CFM = (Input BTU x thermal efficiency) / (1.08 x DT)

    return 1.084;
}

//
// http://skodaclub.bg/forum/index.php?/topic/45846-%D1%80%D0%B0%D0%B7%D1%85%D0%BE%D0%B4-%D0%BD%D0%B0-%D0%B3%D0%BE%D1%80%D0%B8%D0%B2%D0%BE-%D0%BD%D0%B0-%D0%BF%D1%80%D0%B0%D0%B7%D0%B5%D0%BD-%D1%85%D0%BE%D0%B4-%D0%BB%D0%B8%D1%82%D1%80%D0%B8-%D0%BD%D0%B0-%D1%87%D0%B0%D1%81/
//
int calConsumption(int engineRpm) {
    int oneEngineTurnLiter;
    int oneEngineTurnBurns;
    int consumptionForHour;
    int consumptionCc;
    int consumptionCubicKg;
    int engineTemperature = 50;


    oneEngineTurnBurns = (CON_ENG_CC / 2) / 1000 /* for coverts to liters */;
    consumptionForHour = (engineRpm * oneEngineTurnBurns) /* result for 1 min */  * 60 /* to one hour */;

    if (THROTTLE_UP)
        consumptionCc = consumptionForHour / FLW_MTR_FR;  /* in case throttle is not positive  */
    else
        consumptionCc = consumptionForHour;


    consumptionCubicKg =
            ((consumptionCc / 1000) * getAirFlowRation(engineTemperature)) / AIR_FUL_RT; // Convert to kg per hour

    return consumptionCubicKg / 81;
}


// argument is time in milliseconds
void printTime(unsigned long t_milli) {
    char buffer[20];
    int days, hours, mins, secs;
    int fractime;
    unsigned long inttime;

    inttime = t_milli / 1000;
    fractime = t_milli % 1000;
    // inttime is the total number of number of seconds
    // fractimeis the number of thousandths of a second

    // number of days is total number of seconds divided by 24 divided by 3600
    days = inttime / (24 * 3600);
    inttime = inttime % (24 * 3600);

    // Now, inttime is the remainder after subtracting the number of seconds
    // in the number of days
    hours = inttime / 3600;
    inttime = inttime % 3600;

    // Now, inttime is the remainder after subtracting the number of seconds
    // in the number of days and hours
    mins = inttime / 60;
    inttime = inttime % 60;

    // Now inttime is the number of seconds left after subtracting the number
    // in the number of days, hours and minutes. In other words, it is the
    // number of seconds.
    secs = inttime;

    // Don't bother to print days
    sprintf(buffer, "%02d:%02d:%02d.%03d", hours, mins, secs, fractime);
    lcd.print(buffer);
}

//
// Engine read values
unsigned int thcSnsCount = 0;
unsigned int spdSnsCount = 0;


//
// Define public method
static void setupMid();

static void initMenu();

static void runMenu();

bool isSensorReadMid();

int getSensorAmplitudeRead(int PinTarget, int TotalContainerState);

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




