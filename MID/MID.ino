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
// Data container
//#include <EEPROM.h>
//
// Includes Libraries
#include <LiquidCrystal.h>
//
// Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd (1, 2, 4, 5, 6, 7);
//
// Data handler lib
//
// Main Sensor handler
#include "lib/MainFunc.h"
//
// Menu cursor
int cursorMenu = 0;
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
// Setup the code...
void setup ()
{
  //
  // main setup
  setupMain ();
  //
  // Initializes the interface to the LCD screen
  lcd.begin (16, 2);
  //
  //
  setupLcdChar ();
  //
  // Define Alpine Pin
  /*pinMode(alpinePin, OUTPUT);*/
  //
  // Show welcome from car
  playWelcomeScreen ();
  //
  // Set MID menu
  setupMenu ();

}

void loop ()
{

  //
  //  Read main buttons
  readButtons (BTN_PIN_UP, BTN_PIN_DW);
  //
  // Handle navigation
  navigateMenu ();
  //
  // Switch menu from cursor
  switch (cursorMenu)
    {
      //
      // Runs first menu
      case 1:
        readInnerTemp ();
      getEcuSignalAmplitude ();
      break;
      case 4:
      case 5:
        break;
      case 2:

        break;

      case 3:

        break;

    }

  delay (1); // if some issues appears

}

void playWelcomeScreen ()
{
  lcd.setCursor (0, 0);
  lcd.print ("Welcome to Astra");
  delay (1000);
  lcd.setCursor (0, 1);
  lcd.print ("Nice driving! ");
  delay (500);
  lcd.print ((char) 0);
  delay (1500);
}

