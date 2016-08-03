
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
#define MILLIS_PER_HR    3600000UL // Hour
#define MILLIS_PER_MN    60000L    // Minute
#define MILLIS_PER_SC    1000UL    // Second
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
//
// MID plug pins definition over Arduino
//
// Define button pins for steering controller
const int BTN_PIN_UP = 8;
const int BTN_PIN_DW = 9;
//
// Engine pins
const int RPM_SNS_PIN = 18; // the crankshaft speed sensor [attachInterrupt]
const int SPD_SNS_PIN = 12; // Speed sensor hub [attachInterrupt]
const int ECU_SGN_PIN = 13; // ECU signal

//
//
const int sensorTempPin_1 = A0;
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



/* Todo

#include <Wire.h>
#include "lib/lcd/LiquidCrystal_I2C.h"
*/
//  LiquidCrystal lcd(7,8,9,A2,A1,A0)
// Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(15, 14, 34, 35, 36, 37);

//
// Menu cursor
int cursorMenu = 0;
//
// Global interval
const int SNS_INTERVAL_TIME = 2000;

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
// Read inside temperature
#include "lib/DisplayInfo.h"

static void playWelcomeScreen();
//
// Setup the code...
void setup() {
    //
    // Debug serial
    Serial.begin(9600);
    //
    // Engine pin mode as input
    pinMode(SPD_SNS_PIN, INPUT);
    pinMode(ECU_SGN_PIN, INPUT);
    pinMode(RPM_SNS_PIN, INPUT);
    //
    // main setup
    setupMain();
    //
    // Initializes the interface to the LCD screen
    lcd.begin(16, 2);\
    lcd.clear();
    //
    //
    setupLcdChar();

    setupVSS();
    //
    // Define Alpine Pin
    /*pinMode(alpinePin, OUTPUT);*/
    //
    // Show welcome from car
    //playWelcomeScreen();
    //
    // Set MID menu
    setupMenu();
    // SerialDebugger.begin(9600);
    // SerialDebugger.enable(NOTIFICATION);
}

void loop() {
    //
    //  Read main buttons
    readButtons(BTN_PIN_UP, BTN_PIN_DW);
    //
    // Handle navigation
    navigateMenu();
    //
    // Switch menu from cursor
    switch (cursorMenu) {
        //
        // Runs first menu
        case 1:

            readInnerTemp();
            displayEngRPM();
            displayCarKMH();
            displayCarECU();
            break;
        case 4:
        case 5:
            break;
        case 2:

            break;

        case 3:

            break;

    }

    //delay(1); // if some issues appears

}

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


