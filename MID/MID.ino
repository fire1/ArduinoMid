#include <Arduino.h>
/*
---------------------------------------------------
    Arduino MID

     Author: Angel Zaprianov <me@fire1.eu>
     Version: 0.1 [pre-alpha]
     Start Date: 09.06.2016

     For the full copyright and license information, please view the LICENSE
     file that was distributed with this source code.
---------------------------------------------------

*/

//
// Time information
#define MILLIS_PER_HR    3600000 // Hour
#define MILLIS_PER_MN    60000    // Minute
#define MILLIS_PER_SC    1000    // Second
//
//
bool THROTTLE_UP = false; // Is open throttle  (acceleration)
const double CON_ENG_CC = 1.796; // Engine capacity [X18XE1]
const int CON_ENG_CL = 4; // Cylinders
const double FLW_MTR_FR = 1.414; // Flowmeter factor (revers-pressure)
const double AIR_FUL_RT = 14.70; // 14.7(oxygen) : 1(fuel) for burning
const double VEC_FUL_RT = 1.0;    // 14.7(oxygen) : 1(fuel) for burning
//double AirFuelRatio = 14.70;  // константа расхода 14,7 воздуха к 1 литра бензина, у дизеля своя, у газа своя
double FuelDensityGramsPerLiter = 750.0;   // константа - грамм бензина в 1 литре бензина
double AirFuelRatio = 14.70;  // константа расхода 14,7 воздуха к 1 литра бензина, у дизеля своя, у газа своя

//
// MID plug pins definition over Arduino
//
// Define button pins for steering controller
const int BTN_PIN_UP = 8;
const int BTN_PIN_DW = 9;
//
// Engine pins
const int RPM_SNS_PIN = 10;    // MID6 RPM [attachInterrupt]
const int SPD_SNS_PIN = 3;    // MID12 Speed sensor hub [attachInterrupt]
const int ECU_SGN_PIN = 19; // ECU signal
//
// Display dim pins
const int DIM_PIN_VAL = A10; // MID7 input Dim of display
const int DIM_PIN_OUT = 46; // output dim of display
//
// Temperatures
const int TMP_PIN_OUT = A9;

/*
#include <SerialDebug.h>
#define DEBUG true
*/
//
//  MenuBackend library - copyright by Alexander Brevig
// Import it from:
// https://github.com/WiringProject/Wiring/tree/master/framework/libraries/MenuBackend
#include <MenuBackend.h>
//
//
#include <SoftwareSerial.h>
//
// Includes Libraries
#include <LiquidCrystal.h>


#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
/* Todo
#include <Wire.h>
*/

//
// Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(32, 33, 34, 35, 36, 37);

//
// Menu cursor
int cursorMenu = 0;
//
// Global interval
const int SNS_INTERVAL_TIME_LOW = 150; // Low sensor interval
const int SNS_INTERVAL_TIME_MID = 2000; // Mid sensor inter
int showerCounter = 0;



//
// Main Sensor handler
#include "lib/MainFunc.h"
//
//
#include "lib/LcdChar.h"
//
// Adding menu source
#include "lib/MidMenu.h"
//
// Adding Alphine emulator
#include "lib/EmlAlpine.h"
//
// Adding sensors
#include "lib/SensInit.h"

static void playWelcomeScreen();

//
// Setup the code...
void setup() {
    //
    // Debug serial
    Serial.begin(9600);
    //
    // Display back-light handler
    setupBackLight(DIM_PIN_VAL, DIM_PIN_OUT);
    //
    // Engine sensors pin mode input
    setupRpmSens(RPM_SNS_PIN); // Engine RPM
    setupVssSens(SPD_SNS_PIN);    // Vehicle Speed Sensor
    setupEcuSens(ECU_SGN_PIN); // Signal from engine ECU  
    //
    // Initializes the interface to the LCD screen
    lcd.begin(16, 2);
    lcd.clear();
    //
    // main setup
    setupMain();
    //
    // Adding custom characters to LCD
    setupLcdChar();
    //
    // Define Alpine Pin
    /*pinMode(alpinePin, OUTPUT);*/
    //
    // Show welcome from car
    //playWelcomeScreen();
    //
    // Set MID menu
    setupMenu();



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

int LastRpm = LOW;
/* timer 2 (controls pin 10, 9)
 * Install the Interrupt Service Routine (ISR) for Timer2 overflow.
 * This is normally done by writing the address of the ISR in the
 * interrupt vector table but conveniently done by using ISR()  */
ISR(TIMER2_OVF_vect) {
   int current =  digitalRead(RPM_SNS_PIN);
   if(current == HIGH && LastRpm == LOW)
     rpmHitsCount++;
  

   LastRpm=current;
}

void loop() {
    //
    // Sensors
    sensorsInit();
    //
    //  Read main buttons
    readButtons(BTN_PIN_UP, BTN_PIN_DW);
    //
    // Handle navigation
    navigateMenu();
    getTravelDistanceMeters();
    //
    // Switch menu from cursor
    switch (cursorMenu) {
        //
        // Main / first menu
        case 1:
            displayOutTmp();
            displayEngRPM();
            displayCarKMH();
            displayCarECU();
            break;
        case 5:

        case 4:
            displayAverage();
            break;
            //
            // Travel menu
        case 2:
            displayDistance();
            break;
        case 3:
            displayConsumption();
            break;

    }
}

/**
 * Welcome screen ...
 */
static void playWelcomeScreen() {
    lcd.setCursor(0, 0);
    lcd.print("Welcome to Astra");
    delay(1000);
    lcd.setCursor(0, 1);
    lcd.print("Nice driving! ");
    delay(500);
    lcd.print((char) 0);
    delay(1500);
}


