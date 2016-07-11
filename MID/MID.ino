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
#define MILLIS_PER_HR    3600000L // Hour
#define MILLIS_PER_MN    60000L   // Minute
#define MILLIS_PER_SC    1000L    // Second
//
//
bool THROTTLE_UP = false; // Is open throttle  (acceleration)
const int CON_ENG_CC = 1800; // Обем на двигателя
const int CON_ENG_CL = 4; // Цилидъра
const int FLW_MTR_FR = 1.414; // Flowmeter factor (revers-pressure)
const int BRN_MAS_FW = 14.7; // 14.7(oxygen) : 1(fuel) for burning
//
// MID plug pins definition over Arduino
//
// Define button pins for steering controller
const int BTN_PIN_UP = 8;
const int BTN_PIN_DW = 9;
//
// Engine pins
const int TCH_SNS_PIN = 22; // the crankshaft speed sensor
const int SPD_SNS_PIN = 22; // Speed sensor hub
const int ECU_SGN_PIN = 24; // ECU signal

//
//
const int sensorTempPin_1 = A0;

//
//  MenuBackend library - copyright by Alexander Brevig
// Import it from:
// https://github.com/WiringProject/Wiring/tree/master/framework/libraries/MenuBackend
#include <MenuBackend.h>
//
// Includes Libraries
#include <LiquidCrystal.h>

//
// Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(1, 2, 4, 5, 6, 7);

//
// Menu cursor
int cursorMenu = 0;
//
// Global interval
const int SNS_INTERVAL_TIME = 2000;
//
// Data handler lib
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
#include "lib/ReadSensors.h"
//
// Define intro
static void playWelcomeScreen ();
//
// Setup the code...
void setup() {

    
    //
    // main setup
    setupMain();
    //
    // Initializes the interface to the LCD screen
    lcd.begin(16, 2);
    //
    //
    setupLcdChar();
    //
    // Define Alpine Pin
    /*pinMode(alpinePin, OUTPUT);*/
    //
    // Show welcome from car
    playWelcomeScreen();
    //
    // Set MID menu
    setupMenu();

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
            getEcuSignalAmplitude();
            break;
        case 4:
        case 5:
            break;
        case 2:

            break;

        case 3:

            break;

    }

    delay(1); // if some issues appears

}
/**
 *
 */
static void playWelcomeScreen () {
    lcd.setCursor(0, 0);
    lcd.print("Welcome to Astra");
    delay(1000);
    lcd.setCursor(0, 1);
    lcd.print("Nice driving! ");
    delay(500);
    lcd.print((char) 0);
    delay(1500);
}

