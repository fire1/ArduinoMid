/*
    Arduino MID

     Author: Angel Zaprianov <me@fire1.eu>
     Version: 0.1 [pre-alpha]
     Start Date: 09.06.2016

     For the full copyright and license information, please view the LICENSE
     file that was distributed with this source code.

*/

//
// Display configs
#include <LiquidCrystal.h> // includes the LiquidCrystal Library
//
// Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(1, 2, 4, 5, 6, 7);
//
// Defines min/max menu amplitude
const int maxMenuNumber = 2;
const int minMenuNumber = 1;
//
// Define button pins for steering controller
const int buttonPinUp = 8;
const int buttonPinDw = 9;
//
// Variables will change:
int buttonStateUp = 0;   // current state of the button
int buttonStateDw = 0;   // previous state of the button
//
// Navigation state value
int navigationState = 0;


void setup() {
    //
    // Pin button mode
    pinMode(buttonPinUp, INPUT);
    pinMode(buttonPinDw, INPUT);
    //
    // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display }
    lcd.begin(16,2);
}

void loop() {
    //
    // Get navigation states
    buttonStateUp = digitalRead(buttonPinUp);
    buttonStateDw = digitalRead(buttonPinDw);
    //
    // Handle navigation buttons
    handleButtonNavigation(buttonStateUp,buttonStateDw);
    //
    // Switch menu
    switch ( navigationState ) {
        case 1:

          break;
        case 2:

          break;
        //
        // MID Introduction ...
        case 0:
        default:
         lcd.print("Welcome to "); // Prints "Arduino" on the LCD
         delay(3000); // 3 seconds delay
         lcd.setCursor(11,0); // Sets the location at which subsequent text written to the LCD will be displayed
         lcd.print("Opel");
         delay(3000);
         lcd.setCursor(1,2);
         lcd.print("Astra");
         delay(3000);
         lcd.clear(); // Clears the display
         lcd.blink(); //Displays the blinking LCD cursor
         delay(4000);
         lcd.setCursor(2,1);
         lcd.print("Loading ...");
         delay(3000);
         lcd.noBlink(); // Turns off the blinking LCD cursor
         lcd.cursor(); // Displays an underscore (line) at the position to which the next character will be written
         delay(4000);
         lcd.noCursor(); // Hides the LCD cursor
         lcd.clear(); // Clears the LCD screen
        break;
      }



}

/**
 *
 */
int handleButtonNavigation(int stateUp,int stateDw){



    if (buttonStateUp == HIGH) {
        navigationState++;
    }

     if (buttonStateDw == HIGH) {
        navigationState--;
    }



}

/**
 *
 */
char handleDriveMode(){

}
