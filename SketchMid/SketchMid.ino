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
// Adding utils emulator
#include "lib/MID.h"


//
//
const int controlLedGr = 22;
const int controlLedRd = 23;

//
// Menu cursor
int cursorMenu = 0;

//
//
const int temperaturePin = A0;
//
// Define button pins for steering controller
const int buttonPinUp = 8;
const int buttonPinDw = 9;
//
// Variables will change:
int buttonStateUp = 0;   // current state of the button
int buttonStateDw = 0;   // previous state of the button

MID mid();
//
// Adding menu source
#include "lib/MidMenu.h"
//
// Adding Alphine emulator
#include "lib/EmlAlpine.h"
//
// Read inside temperature
#include "lib/ReadInnterTemp.h"

//
// Setup the code...
void setup() {
    //
    //
    pinMode(controlLedGr, OUTPUT);
    pinMode(controlLedRd, OUTPUT);
    //
    // Pin button mode
    pinMode(buttonPinUp, INPUT);
    pinMode(buttonPinDw, INPUT);

    //
    // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display }
    lcd.begin(16, 2);
    //
    // main mid class setup
    MID setup();
    //
    // Define Alpine Pin
//    pinMode(alpinePin, OUTPUT);
    lcd.setCursor(0, 0);
    lcd.print("Welcome to Astra");
    delay(1000);
    lcd.setCursor(0, 1);
    lcd.print("Nice driving! ");
    delay(500);
    lcd.print((char) 0);
    delay(1000);

    setupMenu ();

}

void loop() {

    //
    //  Read buttons
    readButtons ();
    //
    // Handle navigation
    navigateMenus ();
    //
    // Switch menu from  cursor
    switch (cursorMenu) {
        case 1:
            readInnerTemp();
            break;

        case 2:

            break;

        case 3:

            break;
        default:
            break;
    }

    delay(1); // if some issues appears
    
}




